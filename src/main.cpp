/**
 * @file main.cpp
 * @Brief Cluster generator implementation  
 * @author Alfredo Santana
 * @version 0.1
 * @date 2014-05-06
 * @notes see TODO comments for set of enhancements
 *        to functionality and statbility.
 */

#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include "generator.hpp"

/**
 * @Brief Single line printing macro  
 */
#define PRINT(msg) std::cout<<msg<<std::endl; 
/**
 * @Brief Collection printing macro
 * @Param coll is an std compatible collection
 *        with elements that have ostream operators
 */
#define PRINT_COLL(coll)            \
    for(auto val : coll)            \
    {                               \
        std::cout<<val << " ";      \
    }                               \
    std::cout<<std::endl;

/**
 * @name Constansts to be made into configuratble
 *       values
 * @{ */

const unsigned LENGTH = 5000; /**Maximum point value*/
const unsigned POINTS = 200;  /**Maximum number of seed points*/
/**  @} */

 
/**
 * @Brief This structure represents a cluster of points    
 *        generated on a give distribution.
 *        The points in the cluster are sorted (generated
 *        that way) and are stored in a file. 
  */
struct Cluster
{
    std::string                   _fileName = "";
    std::shared_ptr<std::fstream> _stream;
    point_t                       _value    = 0;

    Cluster() = default;
    Cluster(Cluster const & src)
        : _fileName(src._fileName)
        , _stream(src._stream)
        , _value(src._value)
    {} 
    Cluster & operator = (Cluster const & src)
    {
        _fileName = src._fileName;
        _stream   = src._stream;
        _value    = src._value;
        return *this;
    }
};

/**
 * @Brief Helper method to remove a file  
 */
void RemoveFile(std::string const & fileName);

typedef std::vector<Cluster> Processes;

int main(int , char **)
{
    ////////////////////////////////////////////////////
    //
    // TODO: Add here configuration functionality
    // ------------------------------------------
    //
    //     //#include <boost/program_options.hpp>
    //     //namespace po = boost::program_options;
    //     //po::options_description desc("Allowed options");
    //     //desc.add_options()
    //     //    ("help", "produce help message");
    //         //("length", po::value<int>(), "set cluster length");
    //     //po::variables_map vm;
    //     //po::store(po::parse_command_line(argc, argv, desc), vm);
    //     //po::notify(vm);
    // 
    //     //if(vm.count("help"))
    //     //{
    //     //    std::cout << desc << std::endl;
    //     //    return 1;
    //     //}
    // 
    ////////////////////////////////////////////////////

    // Cunrrently there is a single generator implemented
    // Next step is to provide a family of generators that
    // represent Poisson, and Paretto distributions to start.

    namespace unif = ClusterGenerator::Uniform;
    // generator enngine the cluster seeds
    unif::Params params;
    unif::Dist point_gtor(unif::Params(0,LENGTH));
   
    // generator engine for the cluster points 
    unif::Dist cluster_gtor(unif::Params(1, LENGTH/2));
    
    // generator engine to determines the maximum
    // number of points in the cluster
    unif::Dist range_gtor(unif::Params(1, LENGTH));

    // generate some points
    std::vector<point_t> points(POINTS);
    std::generate(points.begin(), points.end(), [&] () {return point_gtor();});
 
    PRINT("Cluster points:");
    PRINT_COLL(points);
    PRINT("Generating clusters");

    // for each point generate a cluster 
    Processes processes;
    for(auto & point: points)
    {
        static int fileIndex = 0;            // unique identifier for file names
        bool       inRange   = true;         // controls that the cluster is stil in range
        unsigned   ptCt      = 0;            // counter of points in the cluster
        unsigned   maxCt     = range_gtor(); // maximum number of points in the cluster
        auto       newPoint  = point;        // new cluster point

        Cluster newCluster;
        newCluster._fileName = "file"+std::to_string(fileIndex++)+".txt";
        newCluster._stream.reset(new std::fstream);
        auto & stream = *(newCluster._stream);

        try
        {
            stream.open(newCluster._fileName, std::fstream::out | std::fstream::binary);
        }
        catch(std::exception const & e)
        {
            std::cerr << "Unable to create cluster file " << newCluster._fileName << std::endl;
            for(auto const & cluster : processes)
            {
                RemoveFile(cluster._fileName);
            }

            return 1; 
        }
        newCluster._value = point;
        while(inRange)
        {
            // each cluster is generated here.
            // this is done in a sorted fashion by
            // genereating random deltas between
            // each point.
            newPoint += cluster_gtor();
            if(newPoint > LENGTH || ++ptCt > maxCt)
                inRange = false;  
            else stream << newPoint <<  std::endl;
        }
        stream.close();
        processes.push_back(newCluster);
        assert(processes.back()._value==point);
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

    typedef std::ofstream Results;
    Results result;
    try
    {
        result.open("Results.txt", std::fstream::out | std::fstream::binary);
    }
    catch(std::exception const & e)
    {
        std::cerr << "Unable to create results file " << std::endl;
        for(auto const & cluster : processes)
        {
            RemoveFile(cluster._fileName);
        }

        return 1; 
    }

    while(!processes.empty())
    {
        std::pop_heap(processes.begin() , processes.end(), Comp);
        auto & cluster = processes.back();
       
        if(!cluster._stream->is_open())
        {
            try{
                cluster._stream->open(cluster._fileName, std::fstream::in|std::fstream::binary);
            }
            catch(std::exception const & e)
            {
                std::cerr << "Unable to open luster file " << cluster._fileName << std::endl;
                for(auto const & cluster : processes)
                {
                    RemoveFile(cluster._fileName);
                }

                return 1; 
            }
        }
        
        result << std::to_string(cluster._value) << std::endl;
        
        if( *cluster._stream >> cluster._value )       
        {
            std::push_heap(processes.begin(), processes.end(), Comp);
        }
        else
        {
            cluster._stream->close();
            if(system(("rm " + cluster._fileName).c_str()) != 0)
            {
                PRINT("Error removing cluster file" + cluster._fileName);
            }
            processes.pop_back();
            continue;
        }
    }

    result.close();
    
    return 0;
}

void RemoveFile(std::string const & fileName)
{
    if(system(("rm " + fileName).c_str()) != 0)
    {
        PRINT("Error removing cluster file" + fileName);
    }
}
