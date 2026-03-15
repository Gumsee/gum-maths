#pragma once
#include "vec.h"
#include <random>
#include <chrono>

namespace Gum {
namespace Random {
    template<typename T>
    static T normal(T mean, T sigma)
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::normal_distribution<T> dist(mean, sigma);
      return dist(gen);
    }

    template<typename T>
    static T uniform(T min, T max)
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_real_distribution<T> dist(min, max);
      return dist(gen);
    }

    template<typename T>
    static T uniformInt(T min, T max)
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<T> dist(min, max);
      return dist(gen);
    }

}}
