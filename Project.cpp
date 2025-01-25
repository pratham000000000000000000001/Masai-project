#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

const int NUM_CITIES = 10; // Number of cities
const double INITIAL_TEMPERATURE = 1000.0;
const double COOLING_RATE = 0.95;

// Structure to represent a city
struct City {
    double x;
    double y;
};

// Function to calculate the distance between two cities
double distance(const City& city1, const City& city2) {
    return sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2));
}

// Function to generate a random initial tour
vector<int> generateRandomTour() {
    vector<int> tour(NUM_CITIES);
    for (int i = 0; i < NUM_CITIES; ++i) {
        tour[i] = i;
    }
    random_shuffle(tour.begin(), tour.end());
    return tour;
}

// Function to calculate the total tour length
double calculateTourLength(const vector<City>& cities, const vector<int>& tour) {
    double totalDistance = 0.0;
    for (int i = 0; i < NUM_CITIES - 1; ++i) {
        totalDistance += distance(cities[tour[i]], cities[tour[i + 1]]);
    }
    totalDistance += distance(cities[tour[NUM_CITIES - 1]], cities[tour[0]]);
    return totalDistance;
}

// Function to perform a 2-opt neighborhood move
void twoOptSwap(vector<int>& tour, int i, int j) {
    reverse(tour.begin() + i, tour.begin() + j + 1);
}

// Simulated Annealing algorithm
vector<int> simulatedAnnealing(const vector<City>& cities) {
    double temperature = INITIAL_TEMPERATURE;
    vector<int> currentTour = generateRandomTour();
    double currentLength = calculateTourLength(cities, currentTour);
    vector<int> bestTour = currentTour;
    double bestLength = currentLength;

    while (temperature > 1.0) {
        // Generate a neighbor solution
        int i = rand() % NUM_CITIES;
        int j = rand() % NUM_CITIES;
        while (i == j) {
            j = rand() % NUM_CITIES;
        }
        vector<int> neighborTour = currentTour;
        twoOptSwap(neighborTour, i, j);
        double neighborLength = calculateTourLength(cities, neighborTour);

        // Calculate the change in tour length
        double deltaLength = neighborLength - currentLength;

        // Accept the neighbor solution based on Metropolis criterion
        if (deltaLength < 0 || exp(-deltaLength / temperature) > (double)rand() / RAND_MAX) {
            currentTour = neighborTour;
            currentLength = neighborLength;
        }

        // Update best tour if necessary
        if (currentLength < bestLength) {
            bestTour = currentTour;
            bestLength = currentLength;
        }

        // Cool down the temperature
        temperature *= COOLING_RATE;
    }

    return bestTour;
}

int main() {
    // Generate random city coordinates
    vector<City> cities(NUM_CITIES);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 100.0);
    for (int i = 0; i < NUM_CITIES; ++i) {
        cities[i].x = dis(gen);
        cities[i].y = dis(gen);
    }

    // Find the optimal tour using Simulated Annealing
    vector<int> bestTour = simulatedAnnealing(cities);

    // Print the best tour
    cout << "Best Tour: ";
    for (int i = 0; i < NUM_CITIES; ++i) {
        cout << bestTour[i] << " ";
    }
    cout << endl;

    // Print the total tour length
    cout << "Total Tour Length: " << calculateTourLength(cities, bestTour) << endl;

    return 0;
}