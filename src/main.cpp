#include "matrix_manipulation.h"
#include "equations.h"
#include "inputs.h"
#include "colors.h"

#include <vector>

int main(int argc, char* argv[])
{
	inputInstructions_A();

	int nodes;
	int branches;

	std::map <std::pair<int, int>, char> branchName;
	std::map <char, std::pair<int, int>> invBranchName;

	std::vector<std::vector<int>> graph = readGraph(branchName, invBranchName, nodes, branches);

	std::vector<char> ATree = findTree(graph, branchName, nodes, branches);

	inputInstructions_B(ATree, invBranchName);

	std::vector<std::vector<long double>> values = readValues(branches);

	Matrix <long double> matrixATree = getMatrixATree(ATree, invBranchName, nodes, branches);

	std::cout << matrixATree << std::endl;
}
