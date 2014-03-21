#include <iostream>
#include <random>
#include <algorithm>
#include <deque>
#include "cluster_generator.hpp"


// Some constants
const unsigned LENGTH = 10000;
const unsigned POINTS = 1000;

//int main(int argc, char **argv)
int main()
{
    clusproc::config::generator gen_config;
    gen_config.point_count = POINTS;
    gen_config.lenth = LENGTH;
    clusproc::generator gen(gen_config);

    gen.run(); 
    return 0;
}

