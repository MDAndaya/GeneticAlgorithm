cmake_minimum_required(VERSION 3.14)
project(GeneticAlgorithm)

set(CMAKE_CXX_STANDARD 14)

add_executable(GeneticAlgorithm main.cpp city.hpp tour.hpp tour.cpp genetic_algorithm.hpp genetic_algorithm.cpp)