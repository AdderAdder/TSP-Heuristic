/*
 * An implementation of the Held-Karp algorithm
 * that uses dynamic programming to solve
 * TSP in time complexity O(2^n * n ^2).
 * The implementation is based on the pseudocode
 * found at https://en.wikipedia.org/wiki/Held–Karp_algorithm
 */

#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>
#include <unordered_set>

//std::unordered_map<std::pair<std::unordered_set<int>,int>,int> minPathCost;
std::unordered_map< std::pair<int,int> ,int > edges;
std::unordered_map<std::string,int> dictionary;

int main() {
  std::string fromCity,toCity; int cities,cost,city = 0;
  // Read graph
  std::cin >> cities;
  while (std::cin >> fromCity >> toCity >> cost) {
    if (!dictionary.count(fromCity)) {
      dictionary.insert({fromCity,city++});
    }
    if (!dictionary.count(toCity)) {
      dictionary.insert({toCity,city++});
    }
    edges.insert({std::make_pair(dictionary[fromCity],dictionary[toCity]),cost});
  }

  //Held-Karp algorithm starts here
  //Based on pseudocode from https://en.wikipedia.org/wiki/Held–Karp_algorithm

  return 0;
}
