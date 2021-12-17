#include "equations.h"
#include "inputs.h"
#include <vector>

int main(int argc, char* argv[])
{
	inputInstructions_A();

	int nodes;
	int branches;

	std::map <std::pair<int, int>, char> branchName;
	std::map <char, std::pair<int, int>> invBranchName;

	std::vector<std::vector<int>> graph = readGraph(branchName, invBranchName, nodes, branches);

	std::vector<char> treeBranches = findTree(graph, branchName, nodes, branches);

	inputInstructions_B(treeBranches, invBranchName);

	std::vector<std::vector<long double>> values = readValues(branches);

	Matrix <long double> matrixATree = getMatrixATree(treeBranches, invBranchName, nodes, branches);
	Matrix <long double> matrixALink = getMatrixALink(treeBranches, invBranchName, nodes, branches);
	Matrix <long double> a = getMatrixA(treeBranches, invBranchName, nodes, branches);

	Matrix <long double> b = getMatrixB(matrixATree, matrixALink);

	Matrix <long double> c = getMatrixC(matrixATree, matrixALink);

	Matrix <long double> matrixVoltageSource = getMatrixVoltageSource(values[0]);
	Matrix <long double> matrixCurrentSource = getMatrixCurrentSource(values[1]);
	Matrix <long double> matrixImpedence = getMatrixImpedence(values[2]);

	Matrix <long double> iLoop = getMatrixILoop(b,
		matrixImpedence, matrixCurrentSource, matrixVoltageSource);

	Matrix <long double> jBranch = getMatrixJBranch(iLoop, b);

	Matrix <long double> vBranch = getMatrixVBranch(jBranch,
			matrixImpedence, matrixCurrentSource, matrixVoltageSource);

	std::vector<char> branchesOrder = getBranchesOrder(treeBranches, invBranchName);
	formatResult(vBranch, jBranch, branchesOrder);
}
