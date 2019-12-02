//
// Created by MDand on 2019-12-01.
//

#include "tour.hpp"

using namespace std;

tour::tour() : fitness(0.0) {
    city temp{"temp", 0, 0};
    for (int i = 0; i < CITIES_IN_TOUR; ++i) {
        cities.push_back(temp);
    }
}

tour &tour::operator=(tour t) {
    swap(cities, t.cities);
    swap(fitness, t.fitness);
    return *this;
}
