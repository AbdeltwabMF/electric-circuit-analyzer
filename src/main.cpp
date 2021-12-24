#include "equations.h"
#include "inputs.h"
#include <vector>

int main(int argc, char* argv[])
{
	inputInstructions_A();

	int nodes;
	int branches;

	std::map <std::pair <int, int>, char> endNodesToItsBranchName;
	std::map <char, std::pair <int, int>> branchNameToItsNodes;

	std::vector <std::vector <int>> graph	= readDirectedGraph(endNodesToItsBranchName, branchNameToItsNodes, nodes, branches);
	std::vector <char> orderedTreeBranches	= findTree(graph, endNodesToItsBranchName, nodes, branches);

	inputInstructions_B(orderedTreeBranches, branchNameToItsNodes);

	std::vector<std::vector<long double>> values = readCircuitComponents(branches);

	Matrix <long double> matrixaTree = getATree(orderedTreeBranches, branchNameToItsNodes, nodes, branches);
	Matrix <long double> matrixaLink = getALink(orderedTreeBranches, branchNameToItsNodes, nodes, branches);

	Matrix <long double> a = getA(orderedTreeBranches, branchNameToItsNodes, nodes, branches);
	Matrix <long double> b = getB(matrixaTree, matrixaLink);
	Matrix <long double> c = getC(matrixaTree, matrixaLink);

	formatMatrix(a, "Incidence");
	formatMatrix(b, "Tie-set");
	formatMatrix(c, "Cut-set");

	Matrix <long double> voltageSource 	= getVoltageSource(values[0]);
	Matrix <long double> currentSource	= getCurrentSource(values[1]);
	Matrix <long double> impedence			= getImpedence(values[2]);

	Matrix <long double> iLoop = getILoop(b,
		impedence, currentSource, voltageSource);

	Matrix <long double> jBranch = getJBranch(iLoop, b);

	Matrix <long double> vBranch = getVBranch(jBranch,
			impedence, currentSource, voltageSource);

	formatMatrix(iLoop, "I Loop");
	formatMatrix(jBranch, "J Branch");
	formatMatrix(vBranch, "V Branch");

	std::vector <char> orderedBranches = getBranchesOrder(orderedTreeBranches, branchNameToItsNodes);

	formatResult(vBranch, jBranch, orderedBranches);
}
