
#ifndef THREADING_CPP_RANDOM_GEN_HPP
#define THREADING_CPP_RANDOM_GEN_HPP

#include <random>

namespace ais2203 {

    int rand(int min, int max) {
        static thread_local std::random_device rd;
        static thread_local std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(gen);
    }

}// namespace ais2203

#endif//THREADING_CPP_RANDOM_GEN_HPP
