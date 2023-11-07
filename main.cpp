#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

struct Item{
    int weight;
    int value;
};

struct Knapsack {
    int capacity;
    vector<int> selectedItems;
    int totalValue;
    int totalWeight;
};
double randomBetweenZeroAndOne() {
    random_device rd;  // Initialize the random device
    mt19937 gen(rd()); // Mersenne Twister pseudo-random generator
    uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(gen);
}
int randomBetweenZeroAndNum(int value) {

    random_device rd;  // Initialize the random device
    mt19937 gen(rd()); // Mersenne Twister pseudo-random generator
    uniform_int_distribution<int> distribution(0, value);
    return distribution(gen);
}
vector<vector<bool>> initializePopulation(int populationSize, int numItems) {
    vector<vector<bool>> population(populationSize, vector<bool>(numItems));
    for (int i = 0; i < populationSize; i++) {
        for (int j = 0; j < numItems; j++) {
            population[i][j] = randomBetweenZeroAndNum(2);
        }
    }
    return population;
}

vector<int> evaluateFitness(const vector<vector<bool>>& population,
                            const vector<Item>& items,int knapsackCapacity) {
    vector<int> fitness(population.size(), 0);
    for (int i = 0; i < population.size(); i++) {
        int totalValue = 0;
        int totalWeight = 0;
        for (int j = 0; j < items.size(); j++) {
            if (population[i][j]) {
                totalValue += items[j].value;
                totalWeight += items[j].weight;
            }
        }
        if(totalWeight <= knapsackCapacity) {
            fitness[i] = totalValue;
        }
    }
    return fitness;
}
vector<vector<bool>> rankSelection(const vector<vector<bool>> &population,const vector<int>&fitness) {

    vector<std::pair<int,int>>rankedPopulation(population.size()); // Pair of (fitness, index)
 for (int i = 0; i < population.size(); i++) {
     rankedPopulation[i].first = fitness[i];
     rankedPopulation[i].second = i;
 }

  sort(rankedPopulation.begin(), rankedPopulation.end());

    int n = (int)(population.size() * (population.size() + 1)/2);
    double cumulativeProbability = 0.0;
    vector<double> selectionProbabilities(population.size());
    for (int i = 0; i < population.size(); i++) {
        cumulativeProbability += (double)(i + 1) / n;
        selectionProbabilities[i] = cumulativeProbability;

    }

    vector<vector<bool>> selectedPopulation;
    for (int i = 0; i < population.size(); i++) {
        auto randomValue = randomBetweenZeroAndOne();
        for (int j = 1; j < population.size(); j++) {
            if (selectionProbabilities[j] >= randomValue && selectionProbabilities[j - 1] < randomValue) {
                selectedPopulation.push_back(population[rankedPopulation[j].second]);
                break;
            }
        }
    }

    return selectedPopulation;
}

vector<vector<bool>> crossover(const vector<vector<bool>>& selectedPopulation) {
    vector<vector<bool>> newPopulation;
    for (size_t i = 0; i < selectedPopulation.size(); i += 2) {
        int crossoverPoint = randomBetweenZeroAndNum(selectedPopulation[0].size());
        vector<bool> child1, child2;
        for (int j = 0; j < crossoverPoint; j++) {
            child1.push_back(selectedPopulation[i][j]);
            child2.push_back(selectedPopulation[i + 1][j]);
        }
        for (int j = crossoverPoint; j < selectedPopulation[0].size(); j++) {
            child1.push_back(selectedPopulation[i + 1][j]);
            child2.push_back(selectedPopulation[i][j]);
        }
        newPopulation.push_back(child1);
        newPopulation.push_back(child2);
    }
    return newPopulation;
}
void mutate(vector<vector<bool>>& population, double mutationRate) {
    for (int i = 0; i < population.size(); i++) {
        for (int j = 0; j < population[i].size(); j++) {
            double randomValue = randomBetweenZeroAndOne();
            if (randomValue < mutationRate) {
                population[i][j] = !population[i][j];
            }
        }
    }
}

void start(){
    int knapsackCapacity; cin >> knapsackCapacity;
    int numItems; cin >> numItems;
    int populationSize = 200;
    double mutationRate = 0.02;
    int numberOfGenerations = 20;
    vector<Item>items(numItems);
    vector<vector<bool>> population = initializePopulation(populationSize, numItems);
    vector<int> fitness;
    for(int i = 0; i < numItems;i++){
        cin >> items[i].weight >> items[i].value;
    }
    while(numberOfGenerations--) {
        fitness= evaluateFitness(population, items, knapsackCapacity);
        rankSelection(population, fitness);
        crossover(population).size();
        mutate(population, mutationRate);

    }
    int bestFitness = -1, maxIndex = -1;
    for(int i = 0; i < fitness.size();++i){
        if(fitness[i] > bestFitness){
            bestFitness = fitness[i];
            maxIndex = i;
        }
    }

    int totalWeight = 0;
    int numberOfSelectedItems =0;
    for (int i = 0; i < population[maxIndex].size(); i++) {
        if (population[maxIndex][i]) {
            totalWeight += items[i].weight;
            numberOfSelectedItems++;
            cout << "Selected Item " << i + 1 << ": Weight = " << items[i].weight << ", Value = " << items[i].value << "\n";
        }
    }
    cout << "Number of Selected Items: " << numberOfSelectedItems << "\n";
    cout << "Total Weight: " << totalWeight<<'\n';
    cout << "Total Value: " << bestFitness << "\n\n";

}



int main() {

    freopen("knapsack_input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int t; cin >> t;
    int i = 1;
    //int t = 1;
    while(t--){
        cout << "Test Case Number: " <<  i << endl;
        start();
        i++;
    }


    return 0;
}

