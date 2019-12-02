//
// Created by MDand on 2019-12-01.
//


#include "genetic_algorithm.hpp"

genetic_algorithm::genetic_algorithm() {
    tour default_tour;
    vector<tour> population{POPULATION_SIZE};
    vector<tour> cross{POPULATION_SIZE - NUMBER_OF_ELITES};
    double best_distance = 0.0;
    double base_distance = 0.0;

    /**
     * Set seed of RNG.
     */

    srand(time(NULL));


    /**
     * Initialize cities.
     */
    for (auto i = 0; i < CITIES_IN_TOUR; ++i) {
        city c{"City#" + to_string(i),
               rand() % (MAP_BOUNDARY + 1),
               rand() % (MAP_BOUNDARY + 1)
        };
        default_tour.cities[i] = c;
    }

    /**
     * Generate tours.
     */
    for (auto i = 0; i < POPULATION_SIZE; ++i) {
        for (auto j = 0; j < CITIES_IN_TOUR; ++j)
            population[i].cities[j] = default_tour.cities[j];
        shuffle_cities(population[i]);
        population[i].fitness = 0.0;
    }

    /**
     * Selection
     */

    int fittest_index = determine_fitness(population, POPULATION_SIZE);
    base_distance = SCALAR / population[fittest_index].fitness;

    cout << "Initial shortest distance: " << base_distance << endl;
    cout << endl;

    int iterations = 0;
    while (iterations++ < ITERATIONS && base_distance / best_distance > IMPROVEMENT_FACTOR) {
        if (fittest_index != 0) {
            const tour temp_tour = population[0];
            population[0] = population[fittest_index];
            population[fittest_index] = temp_tour;
        }

        /**
         * Crossover
         */

        for (auto i = 0; i < (POPULATION_SIZE - NUMBER_OF_ELITES); ++i) {
            vector<tour> parents = select_parents(population);
            cross[i] = crossover(parents);
        }

        for (auto i = NUMBER_OF_ELITES; i < POPULATION_SIZE; ++i) {
            population[i] = cross[i - NUMBER_OF_ELITES];
            population[i].fitness = 0.0;
        }

        /**
         * Mutation
         */
        mutate(population);

        /**
         * Evaluation
         */
        fittest_index = determine_fitness(population, static_cast<int>(POPULATION_SIZE));
        best_distance = get_tour_distance(population[fittest_index]);

        /**
         * Report
         */

        if (best_distance < base_distance) {
            base_distance = best_distance;
            print_cities(population[fittest_index].cities);
            cout << "|| Distance: " << best_distance;
            cout << endl;
        }
    }
    cout << endl;
    cout << "Shortest distance: " << SCALAR / population[fittest_index].fitness;
}

/**
 * Randomly mutates the population by swapping.
 * @param population the population to be mutated.
 */
void genetic_algorithm::mutate(vector<tour> &population) {
    for (int i = 0 + NUMBER_OF_ELITES; i < POPULATION_SIZE; ++i) {
        for (int j = 0; j < CITIES_IN_TOUR; ++j) {
            if ((double) rand() / (double) RAND_MAX <= MUTATION_RATE) {
                swap_cities(j, rand() % CITIES_IN_TOUR, population[i].cities);
            }
        }
    }
}

/**
 * Swaps two city values based on indices.
 * @param c1 first city index
 * @param c2 second city index
 * @param cities the vector of cities they are from
 */
void genetic_algorithm::swap_cities(int c1, int c2, vector<city> &cities) {
    city temp = cities[c1];
    cities[c1] = cities[c2];
    cities[c2] = temp;
}

/**
 * Shuffles cities in a tour SHUFFLES times.
 * @param tour the tour who's cities are to be shuffled.
 */
void genetic_algorithm::shuffle_cities(tour &tour) {
    for (int i = 0; i < SHUFFLES; i++)
        shuffle(begin(tour.cities), end(tour.cities), mt19937(random_device()()));
}

/**
 * Finds the index of the shortest tour.
 * @param population the population to be evaluated.
 * @param size of the population.
 * @return shortest tour (most fit).
 */
int genetic_algorithm::determine_fitness(vector<tour> &population, int size) {
    int index_of_shortest_tour = 0;
    auto shortest_tour_in_population = (double) RAND_MAX;
    double candidate_distance = 0.0;

    for (int i = 0; i < size; ++i) {
        candidate_distance = get_tour_distance(population[i]);
        population[i].fitness = SCALAR / candidate_distance;
        if (candidate_distance <= shortest_tour_in_population) {
            index_of_shortest_tour = i;
            shortest_tour_in_population = candidate_distance;
        }
    }
    return index_of_shortest_tour;
}

/**
 * Finds distance between cities with Pythagoras.
 * @param c1 first city.
 * @param c2 second city.
 * @return the distance between the two points.
 */
double genetic_algorithm::get_distance_between_cities(city c1, city c2) {
    return sqrt(
            pow((double) (c1.x - c2.y), 2.0) +
            pow((double) (c1.y - c2.y), 2.0)
    );
}

/**
 * Finds the total distance of a tour.
 * @param tour the tour to be checked.
 * @return total distance.
 */
double genetic_algorithm::get_tour_distance(tour &tour) {
    double distance = 0.0;
    for (int i = 0; i < CITIES_IN_TOUR; ++i)
        distance += get_distance_between_cities(tour.cities[i], tour.cities[(i + 1) % CITIES_IN_TOUR]);
    return distance;
}

/**
 * Selects parents randomly.
 * @param population the population to have parents taken.
 * @return random vector of tours; the parents;
 */
vector<tour> genetic_algorithm::select_parents(vector<tour> &population) {
    int parent_index = 0;
    vector<tour> parents{NUMBER_OF_PARENTS};
    vector<tour> parent_pool{PARENT_POOL_SIZE};

    for (int i = 0; i < NUMBER_OF_PARENTS; ++i) {
        for (int j = 0; j < PARENT_POOL_SIZE; ++j) {
            int rand_index = static_cast<int>(rand() % POPULATION_SIZE);
            parent_pool[j] = population[rand_index];
        }
        parent_index = determine_fitness(parent_pool, static_cast<int>(PARENT_POOL_SIZE));
        parents[i] = parent_pool[parent_index];
    }
    return parents;
}

/**
 * Performs crossover function to return child of parents.
 * @param parents the parents to be crossed.
 * @return a child of the parents.
 */
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

/**
 * Returns true if the tour contains a city.
 * @param candidate_tour the tour to be checked.
 * @param length the length of the tour.
 * @param candidate_city the city searched for.
 * @return whether or not the city exists in the given tour.
 */
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

/**
 * prints all cities' names.
 * @param cities the vector of cities to be manipulated.
 */
void genetic_algorithm::print_cities(vector<city> &cities) {
    for (auto c : cities) cout << c.name << " ";
}