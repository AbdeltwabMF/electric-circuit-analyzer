#ifndef EQUATIONS_H
#define EQUATIONS_H

#include "inputs.h"
#include "matrix_manipulation.h"

template <class T = long double>
Matrix <T> getCLink(Matrix <T> const & matrixATree, Matrix <T> const & matrixALink)
{
	return matrixATree.getInverse() * matrixALink;
}

template <class T = long double>
Matrix <T> getBTree(Matrix <T> const & matrixATree, Matrix <T> const & matrixALink)
{
	return getCLink(matrixATree, matrixALink).getTranspose() * -1;
}

template <class T = long double>
Matrix <T> getMatrixA(std::vector<char> ATree,
		std::map <char, std::pair<int, int>> & invBranchName,
		int nodes, int branches)
{
	std::vector<char> branchesOrder = getBranchesOrder(ATree, invBranchName);

	Matrix <T> A(nodes, branches);

	for(int branch = 0; branch < branches; ++branch)
	{
		A.setElement(invBranchName[branchesOrder[branch]].first, branch, 1.0L);
		A.setElement(invBranchName[branchesOrder[branch]].second, branch, -1.0L);
	}

	return A;
}

template <class T = long double>
Matrix <T> getMatrixATree(std::vector<char> ATree,
		std::map <char, std::pair<int, int>> & invBranchName,
		int nodes, int branches)
{
	Matrix <T> A = getMatrixA(ATree, invBranchName, nodes, branches);
	Matrix <T> matrixATree(A, 0, nodes - 2, 0, nodes - 2);

	return matrixATree;
}

template <class T = long double>
Matrix <T> getMatrixALink(std::vector<char> ATree,
		std::map <char, std::pair<int, int>> & invBranchName,
		int nodes, int branches)
{
	Matrix <T> A = getMatrixA(ATree, invBranchName, nodes, branches);
	Matrix <T> matrixALink(A, 0, nodes - 2, nodes - 1, branches - 1);

	return matrixALink;
}

#endif // EQUATIONS_H

