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
	std::cout \
		<< White  << "            Simple Electric Circuits Analysis             \n\n" \
		<< Cyan   << " " \
		<< colorAndRest("Input Specifications:", Green, Cyan) \
		<< "                                    " \
		<< Cyan   << "\n\n   The first line contains two integers " \
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
		<< ")\n   - The two end points of a branch.\n" \
		<< Yellow << "\n   " \
		<< colorAndRest("Caution:", Red, Yellow) \
		<< " The input order of the branches are SORTED\n   from 'a' till the end." \
		<< Green  << "\n\n The Input:\n" << Reset \
		<< std::endl;
}

std::vector <char> getBranchesOrder(
		std::vector <char> orderedTreeBranches,
		std::map <char, std::pair<int, int>> & branchNameToItsNodes)
{
	std::vector <char> orderedBranches;
	orderedBranches.assign(orderedTreeBranches.begin(), orderedTreeBranches.end());

	std::sort(orderedTreeBranches.begin(), orderedTreeBranches.end());

	for(auto it : branchNameToItsNodes)
	{
		if(!std::binary_search(orderedTreeBranches.begin(), orderedTreeBranches.end(), it.first))
			orderedBranches.emplace_back(it.first);
	}

	return orderedBranches;
}

void inputInstructions_B(
		std::vector <char> orderedTreeBranches,
		std::map <char, std::pair<int, int>> & branchNameToItsNodes)
{
	std::cout \
		<< Cyan << "\n   Each of the next " \
		<< colorAndRest("three", Yellow, Cyan) \
		<< " lines contains an array of \n   " \
		<< colorAndRest(std::to_string(branchNameToItsNodes.size()), Yellow, Cyan) \
		<< " values - the voltage sources, the current sources \
		\n   and the resistances on the branches.\n" \
		<< Purple << "\n   The Branches are in the following order:" \
		<< std::endl;

	std::vector <char> orderedBranches = getBranchesOrder(orderedTreeBranches, branchNameToItsNodes);
	std::string values[3] = {"  Voltage Sources: ", "  Current Sources: ", "  Resistances    : "};

	for(int vcr = 0; vcr < 3; ++vcr)
	{
		std::cout << White << "   " << values[vcr];
		for(char ch : orderedBranches)
			std::cout << Yellow << " " << ch;

		std::cout << Reset << std::endl;
	}
	std::cout << std::endl;
}

/** Searching for Tree branches */
void dfs(
		std::vector <std::vector <int>> const & graph,
		std::vector <char> & visited,
		std::vector <char> & orderedTreeBranches,
		std::map <std::pair <int, int>, char> & endNodesToItsBranchName,
		std::unordered_map <char, bool> & sameComponent,
		int source,
		bool _link = true)
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
				orderedTreeBranches.emplace_back(endNodesToItsBranchName[std::make_pair(source, node)]);
			dfs(graph, visited, orderedTreeBranches, endNodesToItsBranchName, sameComponent, node, _link);
		}
		else if(!sameComponent[node] && _link)
		{
			orderedTreeBranches.emplace_back(endNodesToItsBranchName[std::make_pair(source, node)]);
			_link = false;
		}
	}
	visited[source] = 2;
}

/** Forming an A Tree Using DFS Tree */
std::vector <char> findTree(
		std::vector <std::vector <int>> const & graph,
		std::map <std::pair <int, int>, char> & endNodesToItsBranchName,
		int const & nodes,
		int const & branches)
{
	std::vector <char> visited(nodes, 0);
	std::vector <char> orderedTreeBranches;
	std::unordered_map <char, bool> sameComponent;

	for(int node = 0; node < nodes; ++node)
	{
		if(!visited[node])
		{
			sameComponent.clear();
			dfs(graph, visited, orderedTreeBranches, endNodesToItsBranchName, sameComponent, node);
		}
	}

	return orderedTreeBranches;
}

/** Adding new edge and assign name to it according to its input order */
void addEdge(
		std::vector <std::vector <int>> & graph,
		std::map <std::pair <int, int>, char> & endNodesToItsBranchName,
		std::map <char, std::pair <int, int>> & branchNameToItsNodes,
		int & from,
		int & to,
		int branchOrder)
{
	graph[from].emplace_back(to);
	char someBranch = static_cast <char> (branchOrder + 'a');
	endNodesToItsBranchName[std::make_pair(from, to)] = someBranch;
	branchNameToItsNodes[someBranch] = std::make_pair(from, to);
}

/** Reading the graph nodes, branches and the links between each node */
std::vector<std::vector <int>> readDirectedGraph(
		std::map <std::pair <int, int>, char> & endNodesToItsBranchName,
		std::map <char, std::pair <int, int>> & branchNameToItsNodes,
		int & nodes,
		int & branches)
{
	std::cin >> nodes >> branches;

	std::vector<std::vector <int>> graph(nodes);
	int from, to;

	for(int branch = 0; branch < branches; ++branch)
	{
		std::cin >> from >> to;
		--from, --to;
		addEdge(graph, endNodesToItsBranchName, branchNameToItsNodes, from, to, branch);
	}

	return graph;
}
