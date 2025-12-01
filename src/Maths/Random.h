#pragma once
#include "vec.h"
#include <random>
#include <chrono>

namespace Gum {
namespace Random {
    template<typename T>
    static T normal(T mean, T sigma)
    {
      std::normal_distribution<T> dist(mean, sigma);
      return dist(std::mt19937(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())));
    }

    template<typename T>
    static T uniform(T min, T max)
    {
      std::uniform_real_distribution<T> dist(min, max);
      return dist(std::mt19937(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())));
    }

    template<typename T>
    static T uniformInt(T min, T max)
    {
      std::uniform_int_distribution<T> dist(min, max);
      return dist(std::mt19937(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())));
    }

}}
