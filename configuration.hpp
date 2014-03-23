#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

#include <deque>
#include <vector>

namespace clusproc
{

typedef unsigned point_t;
typedef std::deque<point_t> Cluster;
typedef std::vector<Cluster> Processes;

/**
 * @Brief supported distiburion ids  
 */
enum class distribution_t
{
    undef,
    uniform,
    normal,
    paretto,
    poisson
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//TODO: For now use a hardcoded configruation and process     //
//      type, but in the future this should be driven         //
//      from a configuration source into a kit pattern.       //
//      A sample configuration might entail having a poisson  //
//      distribution for the generator points, and a paretto  //
//      distribution for the clusters. Other sample           //
//      configuration might be a normal, normal; or a normal, //
//      poisson, etc.                                         //
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
/**
 * @Brief Configuration namespace  
 */
///////////////////////////////////////////////////////////////
namespace config
{
namespace random_generator
{
struct generator
{
    distribution_t type = distribution_t::undef;

    generator() = default;
    generator(generator const &) = default;
    generator(distribution_t val) : type(val) {}
};

struct uniform : public generator
{
    uniform() : generator(distribution_t::uniform) {}
    uniform(uniform const &) = default;
    uniform(point_t dist_min, point_t dist_max) :
        generator(distribution_t::uniform),
        min(dist_min),
        max(dist_max) 
    {
    }
    
    point_t min = 0;
    point_t max = 1;
};
}

/**
 * @Brief configuration for the generator class  
 */
class process
{
public:
    process(){}
    process(point_t length,
            std::size_t count, 
            random_generator::uniform const & generator) :
        _length(length),
        _point_count(count),
        _generator(generator) 
    {
    } 
    
    point_t         length()      const { return _length; }
    std::size_t     point_count() const { return _point_count; }
    random_generator::uniform const & gen_config()  const { return _generator; }

private:
    point_t                   _length       = 0;
    std::size_t               _point_count  = 0;
    random_generator::uniform _generator;
};
}

/////////////////////////////////////////////////////////////////
///**
// * @Brief Random number generator namespace 
// */
/////////////////////////////////////////////////////////////////
//namespace random_generator
//{
//class generator
//{
//public:
//    virtual ~generator() { }
//    distribution_t get_type() const { return type; }
//    virtual point_t operator() () = 0;
//protected:
//    generator() = delete;
//    generator(config::random_generator::generator const & config)
//        : type(config.type)
//    {}
//private:    
//    distribution_t type;
//};
//
//class uniform : public generator
//{
//public:
//    uniform(config::random_generator::uniform const & config)
//        : generator(config)
//    {     
//        std::uniform_int_distribution<point_t> distribution(config.min, config.max);
//    }
//
//    ~uniform() override {}
//
//    point_t operator() () override
//    {
//        return _distribution(_generator);
//    }
//private:
//    std::uniform_int_distribution<point_t> _distribution;
//    std::default_random_engine             _generator;
//};
//
//}
}
#endif




