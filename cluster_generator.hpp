#ifndef __CLUSTER_PROCESS_HPP__
#define __CLUSTER_PROCESS_HPP__

#include <iostream>
#include <random>
#include <algorithm>
#include <deque>
#include <vector>

#define PRINT(msg) std::cout<<msg<<std::endl; 
#ifndef NDEBUG
#define PRINT_COLL(coll)            \
    for(auto val : coll)            \
    {                               \
        std::cout<<val << " ";      \
    }                               \
    std::cout<<std::endl;
#else
#define PRINT_COLL(coll)
#endif

namespace clusproc
{

typedef unsigned point_t;
typedef std::deque<point_t> Cluster;
typedef std::vector<Cluster> Processes;

namespace config
{

/**
 * @Brief configuration for the generator class  
 */
struct generator
{
    point_t     lenth;
    std::size_t point_count;
};
}

class generator
{
public:
    generator() = delete;
    generator(config::generator const & config)
    : _config(config)
    {
    }

    void run()
    {
        std::default_random_engine generator;
        std::uniform_int_distribution<point_t> distribution(0, _config.lenth);
        auto poissonEngine = std::bind ( distribution, generator );

        // generate some points
        std::vector<point_t> points(_config.point_count);
        std::generate(points.begin(), points.end(), [&](){return poissonEngine(0, _config.lenth);});

        PRINT("Generating clusters");

        // for each point generate a cluster 
        Processes processes(_config.point_count);
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
                newPoint += poissonEngine(1, 250);
                if(newPoint > _config.lenth) inRange = false;
                else                         newCluster.push_back(newPoint);   
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

            PRINT("//TopCluster//"); 
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
    }
private:
    config::generator _config;
    
};
}

#endif // __CLUSTER_PROCESS_HPP__
