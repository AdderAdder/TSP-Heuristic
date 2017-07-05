/*
 * An implementation of the Held-Karp algorithm
 * that uses dynamic programming to solve
 * TSP. The implementation is based on the pseudocode
 * found at https://en.wikipedia.org/wiki/Held–Karp_algorithm
 */

//Define macros for debugging
#ifdef DEBUG
#define print(x) std::cout << x << std::endl
#else
#define print(x)
#endif

#include <iostream>
#include <string>
#include <utility>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <limits>

std::map<std::pair<std::set<int>,int>,int> minPathCost;
std::map<std::pair<int,int>,int> edges;
std::map<std::string,int> dictionary;

int main() {
  std::string fromCity,toCity; int cities,cost,city = 1;
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
  std::vector<int> tot;
  std::set<int> totSet;
  tot.reserve(cities-2);
  for (int k = 2; k <= cities; k++) {
    std::set<int> tmp = {k};
    tot.push_back(k);
    totSet.insert(k);
    minPathCost.insert({std::make_pair(tmp,k),edges[{k,1}]});
    print("----- Min cost path from node 1 to k is");
    print(k);
    int z = edges[{k,1}];
    print(z);
  }
  print("-----");
  for (int subSize = 2; subSize < cities; subSize++) {
    int skip = 0;
    std::vector<std::set<int>> oldSubSets;
    do {

      if (skip == 0) {
        std::set<int> subSet;
        for (int taken = 0; taken < subSize; taken++) {
          subSet.insert(tot[taken]);
        }

        bool alreadyDone = false;
        for (auto it = oldSubSets.cbegin(); it != oldSubSets.cend(); it++) {
          if (subSet == *it) {
            alreadyDone = true;
            break;
          }
        }

        if (!alreadyDone) {
          for (auto it = subSet.cbegin(); it != subSet.cend(); it++) {
            int minCost = std::numeric_limits<int>::max();
            std::set<int> tmpSet (subSet);
            tmpSet.erase(*it);

            for (auto it2 = subSet.cbegin(); it2 != subSet.cend(); it2++) {
              if (it2 != it) {
                int tmpCost = minPathCost.at(std::make_pair(tmpSet,*it2)) + edges[std::make_pair(*it,*it2)];
                if (minCost > tmpCost) {
                  minCost = tmpCost;
                }
              }
            }
            minPathCost.insert({std::make_pair(subSet,*it),minCost});
            oldSubSets.push_back(subSet);
          }
        }
        skip = [](int num)->int {int tmp = 1; for (int i = 1; i <= num; i++) tmp *= i; return tmp;}(cities-subSize-2);
      }
      --skip;
    } while (std::next_permutation(tot.begin(),tot.end()));
  }

  int minCost = std::numeric_limits<int>::max();
  for (auto it = tot.cbegin(); it != tot.cend(); it++) {
    int tmpCost = minPathCost[std::make_pair(totSet,*it)] + edges[std::make_pair(1,*it)];
    if (minCost > tmpCost) {
      minCost = tmpCost;
    }
  }

  std::cout << "The minimum cost to travel is " << minCost << std::endl;

  return 0;
}
