#ifndef EQUATIONS_H
#define EQUATIONS_H

#include "inputs.h"
#include "matrix_manipulation.h"

template <class T = long double>
Matrix <T> getMatrixCLink(Matrix <T> const & matrixATree, Matrix <T> const & matrixALink)
{
	Matrix <T> matrixATreeInverse = matrixATree.getInverse();

	return (matrixATreeInverse * matrixALink);
}

template <class T = long double>
Matrix <T> getMatrixBTree(Matrix <T> const & matrixATree, Matrix <T> const & matrixALink)
{
	Matrix <T> matrixCLink = getMatrixCLink(matrixATree, matrixALink);

	return (matrixCLink.getTranspose() * -1.0L);
}

template <class T = long double>
Matrix <T> getMatrixC(Matrix <T> const & matrixATree, Matrix <T> const & matrixALink)
{
	Matrix <T> matrixCLink = getMatrixCLink(matrixATree, matrixALink);
	IdentityMatrix <T> matrixCTree(matrixCLink.getRows(), matrixCLink.getRows());

	Matrix <T> matrixC(matrixCTree, matrixCLink, Matrix<T>::Position::TO_RIGHT);

	return matrixC;
}

template <class T = long double>
Matrix <T> getMatrixB(Matrix <T> const & matrixATree, Matrix <T> const & matrixALink)
{
	Matrix <T> matrixBTree = getMatrixBTree(matrixATree, matrixALink);
	IdentityMatrix <T> matrixBLink(matrixBTree.getRows(), matrixBTree.getRows());

	Matrix <T> matrixB(matrixBTree, matrixBLink, Matrix<T>::Position::TO_RIGHT);

	return matrixB;
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

