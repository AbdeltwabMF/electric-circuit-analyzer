#ifndef INPUTS_H
#define INPUTS_H

#include "matrix_manipulation.h"
#include "colors.h"

#include <ios>
#include <ostream>
#include <vector>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

std::string colorAndRest(std::string _str, std::string _new, std::string _old);

void inputInstructions_A();

std::vector<char> getBranchesOrder(
		std::vector<char> matrixATree,
		std::map <char, std::pair<int, int>> & invBranchName);

void inputInstructions_B(std::vector<char> matrixATree,
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

/** Reading The voltage sources, current sources and the resistances of the branches */
template <class T = long double>
std::vector<std::vector<T>> readValues(int branches)
{
	std::vector<std::vector<T>> ret(3, std::vector<T> (branches));

	for(int vcr = 0; vcr < 3; ++vcr)
		for(int branch = 0; branch < branches; ++branch)
			std::cin >> ret[vcr][branch];

	std::cout << std::endl << colorAndRest(" The Process:\n", Green, Reset) << std::endl;
	return ret;
}

template <class T = long double>
void formatResult(
		Matrix <T> & vBranch,
		Matrix <T> & jBranch,
		std::vector <char> const & branchesOrder)
{
	std::cout << std::fixed << std::setprecision(8);
	std::cout << Green  << " The Answer:" << Reset << std::endl;
	std::cout << Yellow << "                Voltage(V)  \t Current(A)" << Reset << std::endl;
	std::cout << Yellow << "   ----------   ----------- \t ----------" << Reset << std::endl;

	for(int branch = 0; branch < branchesOrder.size(); ++branch)
	{
		std::cout << colorAndRest("   Branch: ", Cyan, White) << "'" << branchesOrder[branch] << "'  " \
			<< Purple << vBranch.getElement(branch, 0) << " \t " \
			<< Blue << jBranch.getElement(branch, 0) << std::endl;
	}
}

template<class T = long double>
void formatMatrix(Matrix <T> & X, std::string name)
{
	std::cout << White << "   Matrix: " << colorAndRest(name, Yellow, Cyan) << std::endl;
	for(int row = 0; row < X.getRows(); ++row)
	{
		std::cout << "     ";
		for(int column = 0; column < X.getColumns(); ++column)
			std::cout << X.getElement(row, column) << " \n"[column == X.getColumns() - 1];
	}
	std::cout << std::endl;
}

#endif // INPUTS_H

