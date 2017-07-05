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
std::map<std::pair<std::set<int>,int>,int> prevPos;
std::map<std::pair<int,int>,int> edges;
std::map<std::string,int> stringToInt;
std::map<int,std::string> intToString;

int main() {
  std::string fromCity,toCity; int cities,cost,city = 1;
  // Read graph
  std::cin >> cities;
  while (std::cin >> fromCity >> toCity >> cost) {
    if (!stringToInt.count(fromCity)) {
      stringToInt.insert({fromCity,city});
      intToString.insert({city++,fromCity});
    }
    if (!stringToInt.count(toCity)) {
      stringToInt.insert({toCity,city});
      intToString.insert({city++,toCity});
    }
    edges.insert({std::make_pair(stringToInt[fromCity],stringToInt[toCity]),cost});
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
    minPathCost.insert({std::make_pair(tmp,k),edges[{1,k}]});
    prevPos.insert({std::make_pair(tmp,k),1});
  }
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
            int oldPos;

            for (auto it2 = subSet.cbegin(); it2 != subSet.cend(); it2++) {
              if (it2 != it) {
                int tmpCost = minPathCost.at(std::make_pair(tmpSet,*it2)) + edges[std::make_pair(*it2,*it)];
                if (minCost > tmpCost) {
                  oldPos = *it2;
                  minCost = tmpCost;
                }
              }
            }
            minPathCost.insert({std::make_pair(subSet,*it),minCost});
            prevPos.insert({std::make_pair(subSet,*it),oldPos});
            oldSubSets.push_back(subSet);
          }
        }
        skip = [](int num)->int {int tmp = 1; for (int i = 1; i <= num; i++) tmp *= i; return tmp;}(cities-subSize-2);
      }
      --skip;
    } while (std::next_permutation(tot.begin(),tot.end()));
  }

  int minCost = std::numeric_limits<int>::max();
  int oldPos;
  for (auto it = tot.cbegin(); it != tot.cend(); it++) {
    int tmpCost = minPathCost[std::make_pair(totSet,*it)] + edges[std::make_pair(*it,1)];
    if (minCost > tmpCost) {
      oldPos = *it;
      minCost = tmpCost;
    }
  }

  std::vector<int> path;
  path.push_back(1);
  while (totSet.size() != 1) {
    path.push_back(oldPos);
    int tmp = prevPos[std::make_pair(totSet,oldPos)];
    totSet.erase(oldPos);
    oldPos = tmp;
  }
  path.push_back(*(totSet.begin()));

  std::cout << "The minimum cost to travel is " << minCost << std::endl;
  std::cout << "The path is: ";
  for (auto it = path.cbegin(); it != path.cend(); it++) {
    std::cout << intToString[*it] << " -> ";
  }
  std::cout << intToString[1] << std::endl;
  return 0;
}
