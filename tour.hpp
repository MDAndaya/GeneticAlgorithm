//
// Created by MDand on 2019-12-01.
//

#ifndef GENETICALGORITHM_TOUR_HPP
#define GENETICALGORITHM_TOUR_HPP

#include <vector>
#include "city.hpp"

using namespace std;

class tour {
public:
    const int CITIES_IN_TOUR = 32;

    vector<city> cities;
    double fitness;

    tour();

    tour &operator=(tour t);
};


#endif //GENETICALGORITHM_TOUR_HPP
