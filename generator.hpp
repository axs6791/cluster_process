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
class Generator
{
public:
    Generator() = default;
    virtual ~Generator() = default;
    virtual point_t operator()() = 0;
};

class Uniform : public Generator
{
public:
    
    Uniform(point_t lower_bound, point_t upper_bound) : Generator()
    {
        std::default_random_engine engine;
        std::uniform_int_distribution<point_t> distribution(lower_bound, upper_bound);
        std::function<point_t()> poissonEngine = std::bind ( distribution, engine );
    }
    
    ~Uniform() override = default;
    
    point_t operator()() override
    { 
        return _generator(); 
    };

private:
    std::function<point_t()> _generator;
};
}

#endif //__GENERATOR_HPP__
