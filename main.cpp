#include <iostream>
#include <random>
#include <algorithm>
#include <deque>
#include "cluster_generator.hpp"
#include "configuration.hpp"


// Some constants
const unsigned LENGTH = 10000;
const unsigned POINTS = 1000;
const unsigned DIST_MIN = 0;
const unsigned DIST_MAX = LENGTH;


//int main(int argc, char **argv)
int main()
{
    clusproc::config::random_generator::uniform generator(DIST_MIN, DIST_MAX);
    clusproc::config::process gen_config(LENGTH, POINTS, generator);
    //clusproc::process process(gen_config);

    //process.run(); 
    return 0;
}

