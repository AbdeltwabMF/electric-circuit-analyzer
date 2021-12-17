#ifndef INPUTS_H
#define INPUTS_H

#include "colors.h"
#include "matrix_manipulation.h"
#include <vector>
#include <map>
#include <string>
#include <unordered_map>

std::string colorAndRest(std::string _str, std::string _new, std::string _old);

void inputInstructions_A();

std::vector<char> getBranchesOrder(std::vector<char> ATree,
		std::map <char, std::pair<int, int>> & invBranchName);

void inputInstructions_B(std::vector<char> ATree,
		std::map <char, std::pair<int, int>> & invBranchName);

void dfs(std::vector<std::vector<int>> const & graph,
		std::vector<char> & visited,
		std::vector<char> & treeBranches,
		std::map <std::pair<int, int>, char> & branchName,
		std::unordered_map<char, bool> & sameComponent,
		int & source, bool _link = true);

std::vector<char> findTree(std::vector<std::vector<int>> const & graph,
		std::map <std::pair<int, int>, char> & branchName,
		int const & nodes, int const & branches);

void addEdge(std::vector<std::vector<int>> & graph,
		std::map <std::pair<int, int>, char> & branchName,
		std::map <char, std::pair<int, int>> & invBranchName,
		int & from, int & to, int branchOrder);

std::vector<std::vector<int>> readGraph(std::map <std::pair<int, int>, char> & branchName,
		std::map <char, std::pair<int, int>> & invBranchName,
		int & nodes, int & branches);

std::vector<std::vector<long double>> readValues(int branches);

#endif // INPUTS_H

