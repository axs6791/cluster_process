/**
 * @file generator.hpp
 * @Brief  
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
 * @Brief Interface for all generetar classes  
 */
class Generator
{
public:
    Generator() = default;
    virtual ~Generator() = default;
    virtual point_t operator()() = 0;
};

/**
 * @Brief Uniform distriburion generator 
 */
class Uniform : public Generator
{
public:
    
    /**
     * @Brief ctor
     * @Param lower_bound defines the lower bound in the distribution
     * @Param upper_bound defines the upper bound in the distribution
     */
    Uniform(point_t lower_bound, point_t upper_bound) : Generator()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<point_t> distribution(lower_bound, upper_bound);
        _generator = std::bind (distribution, gen);
    }
    
    ~Uniform() override = default;
    
    point_t operator()() override
    { 
        return _generator(); 
    };

private:
    std::function<point_t()> _generator; /** bound generator object */
};
}

#endif //__GENERATOR_HPP__
