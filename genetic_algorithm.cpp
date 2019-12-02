//
// Created by MDand on 2019-12-01.
//


#include "genetic_algorithm.hpp"

genetic_algorithm::genetic_algorithm() {
    tour master_list;
    vector<tour> population{POPULATION_SIZE};
    vector<tour> cross{POPULATION_SIZE - NUMBER_OF_ELITES};
    double best_distance = 0.0;
    double base_distance = 0.0;

    srand(time(NULL));

    for (int i = 0; i < CITIES_IN_TOUR; ++i) {
        city c{"City#" + to_string(i),
               rand() % (MAP_BOUNDARY + 1),
               rand() % (MAP_BOUNDARY + 1)
        };
        master_list.cities[i] = c;
    }

    for (auto i = 0; i < POPULATION_SIZE; ++i) {
        for (auto j = 0; j < CITIES_IN_TOUR; ++j)
            population[i].cities[j] = master_list.cities[j];
        shuffle_cities(population[i]);
        population[i].fitness = 0.0;
    }

    int best_tour_index = determine_fitness(population, static_cast<int>(POPULATION_SIZE));
    base_distance = SCALAR / population[best_tour_index].fitness;

    cout << "Initial shortest distance: " << base_distance << endl;
    cout << endl;

    int iterations = 0;
    while (iterations++ < ITERATIONS && base_distance / best_distance > IMPROVEMENT_FACTOR) {
        if (best_tour_index != 0) {
            const tour temp_tour = population[0];
            population[0] = population[best_tour_index];
            population[best_tour_index] = temp_tour;
        }

        for (int i = 0; i < (POPULATION_SIZE - NUMBER_OF_ELITES); ++i) {
            vector<tour> parents = select_parents(population);
            cross[i] = crossover(parents);
        }

        for (int i = NUMBER_OF_ELITES; i < POPULATION_SIZE; ++i) {
            population[i] = cross[i - NUMBER_OF_ELITES];
            population[i].fitness = 0.0;
        }

        mutate(population);

        best_tour_index = determine_fitness(population, static_cast<int>(POPULATION_SIZE));
        best_distance = get_tour_distance(population[best_tour_index]);

        if (best_distance < base_distance) {
            base_distance = best_distance;
            print_cities(population[best_tour_index].cities);
            cout << "|| Distance: " << best_distance;
            cout << endl;
        }
    }
    cout << endl;
    cout << "Shortest distance: " << SCALAR / population[best_tour_index].fitness;
}

void genetic_algorithm::mutate(vector<tour> &p) {
    for (int i = 0 + NUMBER_OF_ELITES; i < POPULATION_SIZE; ++i) {
        for (int j = 0; j < CITIES_IN_TOUR; ++j) {
            if ((double) rand() / (double) RAND_MAX <= MUTATION_RATE) {
                swap_cities(j, rand() % CITIES_IN_TOUR, p[i].cities);
            }
        }
    }
}

void genetic_algorithm::swap_cities(int first, int second, vector<city> &cities) {
    city temp = cities[first];
    cities[first] = cities[second];
    cities[second] = temp;
}

void genetic_algorithm::shuffle_cities(tour &t) {
    shuffle(begin(t.cities), end(t.cities), std::mt19937(std::random_device()()));
}

int genetic_algorithm::determine_fitness(vector<tour> &p, int size) {
    int index_of_shortest_tour = 0;
    auto shortest_tour_in_population = (double) RAND_MAX;
    double candidate_distance = 0.0;

    for (int i = 0; i < size; ++i) {
        candidate_distance = get_tour_distance(p[i]);
        p[i].fitness = SCALAR / candidate_distance;
        if (candidate_distance <= shortest_tour_in_population) {
            index_of_shortest_tour = i;
            shortest_tour_in_population = candidate_distance;
        }
    }
    return index_of_shortest_tour;
}

double genetic_algorithm::get_tour_distance(tour &t) {
    double distance = 0.0;
    for (int i = 0; i < CITIES_IN_TOUR; ++i)
        distance += get_distance_between_cities(t.cities[i], t.cities[(i + 1) % CITIES_IN_TOUR]);
    return distance;
}

double genetic_algorithm::get_distance_between_cities(city first, city second) {
    return sqrt(
            pow((double) (first.x - second.y), 2.0) +
            pow((double) (first.y - second.y), 2.0)
    );
}

vector<tour> genetic_algorithm::select_parents(vector<tour> &p) {
    int parent_index = 0;
    vector<tour> parents{NUMBER_OF_PARENTS};
    vector<tour> parent_pool{PARENT_POOL_SIZE};

    for (int i = 0; i < NUMBER_OF_PARENTS; ++i) {
        for (int j = 0; j < PARENT_POOL_SIZE; ++j) {
            int rand_index = static_cast<int>(rand() % POPULATION_SIZE);
            parent_pool[j] = p[rand_index];
        }
        parent_index = determine_fitness(parent_pool, static_cast<int>(PARENT_POOL_SIZE));
        parents[i] = parent_pool[parent_index];
    }
    return parents;
}

tour genetic_algorithm::crossover(vector<tour> &parents) {
    tour child;

    int boundary_index = rand() % CITIES_IN_TOUR;

    for (int i = 0; i < boundary_index; ++i) {
        child.cities[i] = parents[0].cities[i];
    }

    while (boundary_index < CITIES_IN_TOUR) {
        for (int i = 0; i < CITIES_IN_TOUR; ++i) {
            if (!contains_city(child, boundary_index, parents[1].cities[i])) {
                child.cities[boundary_index] = parents[1].cities[i];
                boundary_index++;
            }
        }
    }
    return child;
}

bool genetic_algorithm::contains_city(tour &candidate_tour, int length, city &candidate_city) {
    for (int i = 0; i < length; ++i) {
        if (candidate_tour.cities[i].name == candidate_city.name &&
            candidate_tour.cities[i].x == candidate_city.x &&
            candidate_tour.cities[i].y == candidate_city.y) {
            return true;
        }
    }
    return false;
}

void genetic_algorithm::print_cities(vector<city> &cities) {
    for (auto c : cities) cout << c.name << " ";
}