//
// Created by MDand on 2019-12-01.
//

#ifndef GENETICALGORITHM_GENETIC_ALGORITHM_HPP
#define GENETICALGORITHM_GENETIC_ALGORITHM_HPP


#include <random>
#include <iostream>
#include <iomanip>

#include <algorithm>
#include "city.hpp"
#include "tour.hpp"

using namespace std;

class genetic_algorithm {
    int CITIES_IN_TOUR = 32;
    unsigned long POPULATION_SIZE = 32;
    int SHUFFLES = 64;
    // Instructions unclear ^ "The number of times a swap must be effected for a shuffle"
    int ITERATIONS = 1000;
    int MAP_BOUNDARY = 1000;
    unsigned long PARENT_POOL_SIZE = 5;
    double MUTATION_RATE = 0.15;
    unsigned long NUMBER_OF_PARENTS = 2;
    int NUMBER_OF_ELITES = 1;
    double IMPROVEMENT_FACTOR = 1.0;
    double SCALAR = 10000.0;

    void shuffle_cities(tour &tour);
    double get_distance_between_cities(city c1, city c2);
    int determine_fitness(vector<tour> &population, int size);
    double get_tour_distance(tour &tour);
    vector<tour> select_parents(vector<tour> &population);
    tour crossover(vector<tour> &parents);
    bool contains_city(tour &candidate_tour, int length, city &candidate_city);
    void mutate(vector<tour> &population);
    void swap_cities(int c1, int c2, vector<city> &cities);
    void print_cities(vector<city> &cities);

public:
    genetic_algorithm();
};

#endif //GENETICALGORITHM_GENETIC_ALGORITHM_HPP
