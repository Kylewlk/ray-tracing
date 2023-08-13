//
// Created by wlk12 on 2023/8/13.
//
#include "rtweekend.h"

double random_double() {
    thread_local static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    thread_local static std::mt19937 generator;
    return distribution(generator);
}