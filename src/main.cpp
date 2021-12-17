#include "equations.h"

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
	Matrix <long double> matrixALink = getMatrixALink(ATree, invBranchName, nodes, branches);
	Matrix <long double> matrixA = getMatrixA(ATree, invBranchName, nodes, branches);
	std::cout << matrixA << std::endl;

	Matrix <long double> matrixB = getMatrixB(matrixATree, matrixALink);
	std::cout << matrixB << std::endl;

	Matrix <long double> matrixC = getMatrixC(matrixATree, matrixALink);
	std::cout << matrixC << std::endl;
}
