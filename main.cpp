#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <deque>
#include "generator.hpp"

#define PRINT(msg) std::cout<<msg<<std::endl; 
#define PRINT_COLL(coll)            \
    for(auto val : coll)            \
    {                               \
        std::cout<<val << " ";             \
    }                               \
    std::cout<<std::endl;

// Some constants
const unsigned LENGTH = 500;
const unsigned POINTS = 500;
//typedef unsigned point_t;
typedef std::deque<point_t> Cluster;
typedef std::vector<Cluster> Processes;

//int main(int argc, char **argv)
int main()
{
    ClusterGenerator::Uniform rnd(0, LENGTH);
    std::default_random_engine generator;
    std::uniform_int_distribution<point_t> distribution(0, LENGTH);
    auto poissonEngine = std::bind ( distribution, generator );

    // generate some points
    std::vector<point_t> points(POINTS);
    std::generate(points.begin(), points.end(), [&](){return rnd();});
  
    PRINT("Generating clusters");

    // for each point generate a cluster 
    Processes processes(POINTS);
    for(auto & point: points)
    {
        bool inRange = true;
        auto newPoint = point;
        Cluster newCluster = {point};
        while(inRange)
        {
            // generate each cluster here.
            // this is sorted by genereating random deltas
            // between each point.
            newPoint += rnd(1, 250);
            if(newPoint > LENGTH) inRange = false;
            else                  newCluster.push_back(newPoint);   
        }
        PRINT_COLL(newCluster);
        
        processes.push_back(std::move(newCluster)); 
    }

    // sort all the data from all the clusters
    // --------------------------------------
    //
    // Take each (sorted) cluster and throw it in
    // a min heap based on the top point in the cluster.
    // Pop the top cluster from the heap, pop the top
    // point from the heap and publish it, reinsert the
    // cluster in the heap based on the top new point in
    // the cluster.

    auto Comp = [](Cluster & lhs, Cluster & rhs)
                {
                    return lhs.front() > rhs.front();
                };

    std::make_heap(processes.begin() , processes.end(), Comp);

    PRINT("---Merging Clusters---");
    std::vector<point_t> result;
    while(!processes.empty())
    {
        std::pop_heap(processes.begin() , processes.end(), Comp);
        auto & cluster = processes.back(); 
       
        PRINT("//TopCluster//") 
        PRINT_COLL(cluster); 

        if(!cluster.empty())
        {
            result.push_back(cluster.front());
            cluster.pop_front();
        }

        if (!cluster.empty()) 
            std::push_heap(processes.begin(), processes.end(), Comp);
        else
            processes.pop_back();
    }

    // print out result
    PRINT_COLL(result);
    
    return 0;
}

