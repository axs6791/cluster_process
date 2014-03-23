#ifndef __CLUSTER_PROCESS_HPP__
#define __CLUSTER_PROCESS_HPP__

#include <iostream>
#include <random>
#include <algorithm>
#include <memory>
#include "configuration.hpp"

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


///////////////////////////////////////////////////////////////
// namespace clusproc
// {
// class process
// {
// public:
//     process() = delete;
//     process(config::random_generator::process const & config) :
//         _generator(config.gen_config()),
//         _point_count(config.point_count()),
//         _length(config.length())
//     {
//     }
// 
//     void run()
//     {
//         //random_generator::uniform engine;
//         //std::default_random_engine generator;
//         //std::uniform_int_distribution<point_t> distribution(0, _config.lenth());
//         //auto poissonEngine = std::bind ( distribution, generator );
// 
//         // generate some points
//         std::vector<point_t> points(_point_count);
//         std::generate(points.begin(), points.end(), [&](){return _generator;});
// 
//         PRINT("Generating clusters");
// 
//         //// for each point generate a cluster 
//         Processes processes(_point_count);
//         for(auto & point: points)
//         {
//             bool inRange = true;
//             auto newPoint = point;
//             Cluster newCluster = {point};
//             while(inRange)
//             {
//                 // generate each cluster here.
//                 // this is sorted by genereating random deltas
//                 // between each point.
//                 newPoint += _generator();
//                 if(newPoint > _length) inRange = false;
//                 else                         newCluster.push_back(newPoint);   
//             }
//             PRINT_COLL(newCluster);
// 
//             processes.push_back(std::move(newCluster)); 
//         }
// 
//         // sort all the data from all the clusters
//         // --------------------------------------
//         //
//         // Take each (sorted) cluster and throw it in
//         // a min heap based on the top point in the cluster.
//         // Pop the top cluster from the heap, pop the top
//         // point from the heap and publish it, reinsert the
//         // cluster in the heap based on the top new point in
//         // the cluster.
//         auto Comp = [](Cluster & lhs, Cluster & rhs)
//         {
//             return lhs.front() > rhs.front();
//         };
// 
//         std::make_heap(processes.begin(), processes.end(), Comp);
// 
//         PRINT("---Merging Clusters---");
//         std::vector<point_t> result;
//         while(!processes.empty())
//         {
//             std::pop_heap(processes.begin() , processes.end(), Comp);
//             auto & cluster = processes.back(); 
// 
//             PRINT("//TopCluster//"); 
//             PRINT_COLL(cluster); 
// 
//             if(!cluster.empty())
//             {
//                 result.push_back(cluster.front());
//                 cluster.pop_front();
//             }
// 
//             if (!cluster.empty()) 
//                 std::push_heap(processes.begin(), processes.end(), Comp);
//             else
//                 processes.pop_back();
//         }
// 
//         // print out result
//         PRINT_COLL(result);
//     }
// 
// private:
//     random_generator::uniform _generator;
//     std::size_t               _point_count = 0;
//     point_t                   _length = 0;
//     
// };
// }

#endif // __CLUSTER_PROCESS_HPP__
