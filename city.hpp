//
// Created by MDand on 2019-12-01.
//

#ifndef GENETICALGORITHM_CITY_HPP
#define GENETICALGORITHM_CITY_HPP

#include <string>

using namespace std;

class city {
public:
    string name;
    int x;
    int y;

    city(string name, int x, int y) : name(name), x(x), y(y) {};
};


#endif //GENETICALGORITHM_CITY_HPP
