#include <iostream>
#include <random>
#include "Randomizer.h"

int generateRandom(const int minValue, const int maxValue) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(minValue, maxValue);
    int randomValue = distribution(generator);
    return randomValue;
}
