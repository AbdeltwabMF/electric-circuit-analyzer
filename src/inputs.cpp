#include "inputs.h"

#include "colors.h"

#include <cstddef>
#include <iterator>
#include <ostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm>

/** Coloring a specific sentence */
std::string colorAndRest(std::string _str, std::string _new, std::string _old)
{
	return _new + _str + _old;
}

/** The input specifications formatted using the ANSI escape code */
void inputInstructions_A()
{
	std::cout << White  << "            Simple Electric Circuits Analysis             \n" << std::endl;
	std::cout << Cyan   << " " \
		<< colorAndRest("Input Specifications:", Green, Cyan) \
		<< "                                    " << std::endl;
	std::cout << Cyan   << "\n   The first line contains two integers " \
		<< colorAndRest("N", Yellow, Cyan) \
		<< ", " \
		<< colorAndRest("M", Yellow, Cyan) \
		<< "\n   - The number of nodes and the number of branches in the \n   " \
		<< colorAndRest("directed graph", Yellow, Cyan) \
		<< ", correspondingly. Each of the following \n   " \
		<< colorAndRest("M", Yellow, Cyan) \
		<< " lines contains two integers " \
		<< colorAndRest("u", Yellow, Cyan) \
		<< " and " \
		<< colorAndRest("v", Yellow, Cyan) \
		<< " (" \
		<< colorAndRest("1 <= u, v <= n", Yellow, Cyan) \
		<< ")\n   - The two end points of a branch." << std::endl;
	std::cout << Yellow << "\n   " \
		<< colorAndRest("Caution:", Red, Yellow) \
		<< " The input order of the branches are SORTED\n   from 'a' till the end." << std::endl;
	std::cout << std::endl << Green  << " The input:" << Reset << std::endl;
}

std::vector<char> getBranchesOrder(
		std::vector<char> treeBranches,
		std::map <char, std::pair<int, int>> & invBranchName)
{
	std::vector<char> branchesOrder;
	branchesOrder.assign(treeBranches.begin(), treeBranches.end());

	std::sort(treeBranches.begin(), treeBranches.end());

	for(auto it : invBranchName)
	{
		if(!std::binary_search(treeBranches.begin(), treeBranches.end(), it.first))
			branchesOrder.emplace_back(it.first);
	}

	return branchesOrder;
}

void inputInstructions_B(
		std::vector<char> treeBranches,
		std::map <char, std::pair<int, int>> & invBranchName)
{
	std::cout << Cyan << " Each of the next " \
		<< colorAndRest("three", Yellow, Cyan) \
		<< " lines contains an array of " \
		<< colorAndRest(std::to_string(invBranchName.size()), Yellow, Cyan) \
		<< " values \
		\n   - the voltage sources, the current sources \
		\n   and the resistances on the branches." << std::endl;
	std::cout << Purple << "\n   The Branches are in the following order:" << std::endl;

	std::vector<char> branchesOrder = getBranchesOrder(treeBranches, invBranchName);
	std::string values[3] = {"\tVoltage Sources: ", "\tCurrent Sources: ", "\tResistances    : "};

	for(int vcr = 0; vcr < 3; ++vcr)
	{
		std::cout << White << values[vcr];
		for(char ch : branchesOrder)
			std::cout << Yellow << " " << ch;

		std::cout << Reset << std::endl;
	}
}

/** Searching for Tree branches */
void dfs(
		std::vector<std::vector<int>> const & graph,
		std::vector<char> & visited,
		std::vector<char> & treeBranches,
		std::map <std::pair<int, int>, char> & branchName,
		std::unordered_map<char, bool> & sameComponent,
		int & source, bool _link)
{
	// Cycle detected
	if(visited[source] == 1)
		return;

	visited[source] = 1;
	sameComponent[source] = true;

	for(int node : graph[source])
	{
		if(visited[node] != 2)
		{
			if(visited[node] != 1)
				treeBranches.emplace_back(branchName[std::make_pair(source, node)]);
			dfs(graph, visited, treeBranches, branchName, sameComponent, node, _link);
		}
		else if(!sameComponent[node] && _link)
		{
			treeBranches.emplace_back(branchName[std::make_pair(source, node)]);
			_link = false;
		}
	}
	visited[source] = 2;
}

/** Forming an A Tree Using DFS Tree */
std::vector<char> findTree(
		std::vector<std::vector<int>> const & graph,
		std::map <std::pair<int, int>, char> & branchName,
		int const & nodes,
		int const & branches)
{
	std::vector<char> visited(nodes, 0);
	std::vector<char> treeBranches;
	std::unordered_map<char, bool> sameComponent;

	for(int node = 0; node < nodes; ++node)
	{
		if(!visited[node])
		{
			sameComponent.clear();
			dfs(graph, visited, treeBranches, branchName, sameComponent, node);
		}
	}

	return treeBranches;
}

/** Adding new edge and assign name to it according to its input order */
void addEdge(
		std::vector<std::vector<int>> & graph,
		std::map <std::pair<int, int>, char> & branchName,
		std::map <char, std::pair<int, int>> & invBranchName,
		int & from,
		int & to,
		int branchOrder)
{
	graph[from].emplace_back(to);
	char someBranch = static_cast <char> (branchOrder + 'a');
	branchName[std::make_pair(from, to)] = someBranch;
	invBranchName[someBranch] = std::make_pair(from, to);
}

/** Reading the graph nodes, branches and the links between each node */
std::vector<std::vector<int>> readGraph(
		std::map <std::pair<int, int>, char> & branchName,
		std::map <char, std::pair<int, int>> & invBranchName,
		int & nodes,
		int & branches)
{
	std::cin >> nodes >> branches;

	std::vector<std::vector<int>> graph(nodes);
	int from, to;

	for(int branch = 0; branch < branches; ++branch)
	{
		std::cin >> from >> to;
		--from, --to;
		addEdge(graph, branchName, invBranchName, from, to, branch);
	}

	return graph;
}
