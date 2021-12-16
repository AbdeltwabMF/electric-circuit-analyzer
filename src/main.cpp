#include "matrix_manipulation.h"
#include "inputs.h"
#include "colors.h"

int main(int argc, char* argv[])
{
	_inputInstructions_A();

	int nodes;
	int branches;

	std::map <std::pair<int, int>, char> branchName;
	std::map <char, std::pair<int, int>> invBranchName;

	std::vector<std::vector<int>> graph = readGraph(branchName, invBranchName, nodes, branches);

	std::vector<char> ATree = findTree(graph, branchName, nodes, branches);

	_inputInstructions_B(ATree, branchName, invBranchName);

	std::vector<std::vector<long double>> values = readValues(branches);
}
