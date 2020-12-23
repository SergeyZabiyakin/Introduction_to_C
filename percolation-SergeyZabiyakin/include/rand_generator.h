#pragma once

#include <functional>
#include <random>
#include <ctime>

double get_rand() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0., 1.);
    return dis(gen);
}
