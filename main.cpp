#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>

//#include <boost/program_options.hpp>

#define PRINT(msg) std::cout<<msg<<std::endl; 
#define PRINT_COLL(coll)            \
    for(auto val : coll)            \
    {                               \
        std::cout<<val << " ";      \
    }                               \
    std::cout<<std::endl;


#include "generator.hpp"

//TODO: All the file streams need to be handled
// for exceptions.

// Some constants
const unsigned LENGTH = 5000;
const unsigned POINTS = 500;

struct Cluster
{
    std::string  _fileName = ""; 
    std::shared_ptr<std::fstream> _stream;
    point_t      _value = 0;
    
    Cluster() = default;
    Cluster(Cluster const & data)
        : _fileName(data._fileName)
        , _value(data._value)
    {}
              
};              

typedef std::vector<Cluster> Processes;

//namespace po = boost::program_options;

int main(int , char **)
{
    //po::options_description desc("Allowed options");
    //desc.add_options()
    //    ("help", "produce help message");
        //("length", po::value<int>(), "set cluster length");
    //po::variables_map vm;
    //po::store(po::parse_command_line(argc, argv, desc), vm);
    //po::notify(vm);

    //if(vm.count("help"))
    //{
    //    std::cout << desc << std::endl;
    //    return 1;
    //}

    // generator enngine the cluster seeds
    ClusterGenerator::Uniform point_gtor(0, LENGTH);
    // generator engine for the cluster points 
    ClusterGenerator::Uniform cluster_gtor(1, LENGTH/2);
    // generator engine to determines the maximum
    // number of points in the cluster
    ClusterGenerator::Uniform range_gtor(1, LENGTH);

    // generate some points
    std::vector<point_t> points(POINTS);
    std::generate(points.begin(), points.end(), [&] () {return point_gtor();});
  
    PRINT("Generating clusters");

    // for each point generate a cluster 
    Processes processes;
    for(auto & point: points)
    {
        static int fileIndex = 0;
        bool     inRange = true;
        unsigned ptCt = 0, maxCt = range_gtor();
        auto     newPoint = point;

        Cluster newCluster;
        newCluster._fileName = "file"+std::to_string(fileIndex++)+".txt";
        newCluster._stream.reset(new std::fstream);
        auto & stream = *(newCluster._stream);
        stream.open(newCluster._fileName, std::fstream::in | std::fstream::binary);
        newCluster._value = point;
        while(inRange)
        {
            // generate each cluster here.
            // this is sorted by genereating random deltas
            // between each point.
            newPoint += cluster_gtor();
            if(newPoint > LENGTH || ++ptCt > maxCt)
                inRange = false;  
            else stream << newPoint << std::endl;
        }
        stream.close();
        processes.push_back(newCluster);
    }

    // sort all the data from all the clusters
    // --------------------------------------
    //
    // Take each (sorted) cluster and throw it in
    // a min heap based on the top point in the cluster.
    // Pop the top cluster from the heap, reinsert the
    // cluster in the heap based on the top new point in
    // the cluster.
    
    auto Comp = [](Cluster & lhs, Cluster & rhs)
         {
             return lhs._value> rhs._value;
         };

    std::make_heap(processes.begin() , processes.end(), Comp);

    typedef std::ifstream Results;
    Results result;
    result.open("Results.txt", std::fstream::out | std::fstream::binary);
    while(!processes.empty())
    {
        std::pop_heap(processes.begin() , processes.end(), Comp);
        auto & cluster = processes.back();
        
        if(!cluster._stream->is_open())
        {
            cluster._stream->open(cluster._fileName, std::fstream::out | std::fstream::binary);
        }
            
        *cluster._stream <<  cluster._value;       
        result >> cluster._value;

        // based on how clusters are distributed, there might be
        // a better approach that poping and pushing in to the heap.
        if (!cluster._stream->eof()) 
            std::push_heap(processes.begin(), processes.end(), Comp);
        else
            // This cluster hace been fully process, so it can be
            // removed.
            processes.pop_back();
    }

    
    return 0;
}
