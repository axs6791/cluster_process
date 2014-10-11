/**
 * @file generator.hpp
 * @brief  
 * @author Alfredo Santana
 * @version 
 * @date 2014-04-03
 */

#ifndef __GENERATOR_HPP__
#define __GENERATOR_HPP__

#include <random>
#include <algorithm>

typedef unsigned point_t;

namespace ClusterGenerator
{
/**
* @brief Interface for genreator classes
* @tparam PARAM is the configuration
*         structure for the generator
*/
template<typename PARAM>
class Generator
{
public:

    typedef PARAM param_t;
    /**
     * @brief  ctor
     * @param config is the 
     */
    Generator(param_t const & param)
        : _param(param) {}

    virtual ~Generator() = default;
    virtual point_t operator()() = 0;
    
protected:
    PARAM _param;
};

namespace Uniform
{

struct Params
{
    Params() = default;
    Params(point_t lowerBound, point_t upperBound)
        : _lb(lowerBound)
        , _ub(upperBound)
    {}

    point_t _lb = 0;
    point_t _ub = 0;
};

/**
 * @brief Uniform distriburion generator 
 */
class Dist : public Generator<Params>
{
public:
    
    /**
     * @brief  ctor
     * @param params for the distribution
     */
    Dist(param_t const & params) : Generator(params)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<point_t> distribution(params._lb, params._ub);
        _generator = std::bind (distribution, gen);
    }
    
    ~Dist() override = default;
    
    point_t operator()() override
    { 
        return _generator(); 
    };

private:
    std::function<point_t()> _generator; /** bound generator object */
};
}

///**
// * @brief Uniform distriburion generator 
// */
//class Poisson: public Generator
//{
//public:
//    
//    /**
//     * @brief ctor
//     * @param lower_bound defines the lower bound in the distribution
//     * @param upper_bound defines the upper bound in the distribution
//     */
//    Poisson(point_t lower_bound, point_t upper_bound) : Generator()
//    {
//        typedef std::poisson_distribution<point_t> distribution_t;
//        std::random_device rd;
//        std::mt19937 gen(rd());
//
//        distribution_t::param distribution();
//        _generator = std::bind (distribution, gen);
//    }
//    
//    ~Poisson() override = default;
//    
//    point_t operator()() override
//    { 
//        return _generator(); 
//    };
//
//private:
//    std::function<point_t()> _generator; /** bound generator object */
//};
}

#endif //__GENERATOR_HPP__
