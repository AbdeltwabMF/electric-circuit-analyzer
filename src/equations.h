#ifndef EQUATIONS_H
#define EQUATIONS_H

#include "inputs.h"
#include "matrix_manipulation.h"

#include <vector>

template <class T = long double>
Matrix <T> getMatrixA(
		std::vector <char> treeBranches,
		std::map <char, std::pair <int, int>> & invBranchName,
		int nodes,
		int branches)
{
	std::vector <char> branchesOrder = getBranchesOrder(treeBranches, invBranchName);

	Matrix <T> A(nodes, branches);

	for(int branch = 0; branch < branches; ++branch)
	{
		A.setElement(invBranchName[branchesOrder[branch]].first,  branch,  1.0L);
		A.setElement(invBranchName[branchesOrder[branch]].second, branch, -1.0L);
	}

	return A;
}

template <class T = long double>
Matrix <T> getMatrixATree(
		std::vector <char> treeBranches,
		std::map <char, std::pair <int, int>> & invBranchName,
		int nodes,
		int branches)
{
	Matrix <T> A = getMatrixA(treeBranches, invBranchName, nodes, branches);
	Matrix <T> matrixATree(A, 0, nodes - 2, 0, (int)treeBranches.size() - 1);

	return matrixATree;
}

template <class T = long double>
Matrix <T> getMatrixALink(
		std::vector <char> treeBranches,
		std::map <char, std::pair <int, int>> & invBranchName,
		int nodes,
		int branches)
{
	Matrix <T> A = getMatrixA(treeBranches, invBranchName, nodes, branches);
	Matrix <T> matrixALink(A, 0, nodes - 2, (int)treeBranches.size(), branches - 1);

	return matrixALink;
}

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
Matrix <T> getMatrixImpedence(std::vector <T> const & resistances)
{
	int resistancesSize = resistances.size();
	Matrix <T> matrixImpedence(resistancesSize, resistancesSize);

	for(int row = 0; row < matrixImpedence.getRows(); ++row)
			matrixImpedence.setElement(row, row, resistances[row]);

	return matrixImpedence * IdentityMatrix <T> (matrixImpedence.getRows(), matrixImpedence.getRows());
}

template <class T = long double>
Matrix <T> getMatrixCurrentSource(std::vector <T> const & currentSources)
{
	Matrix <T> matrixCurrentSource((int)currentSources.size(), 1);
	for(int row = 0; row < matrixCurrentSource.getRows(); ++row)
		matrixCurrentSource.setElement(row, 0, currentSources[row]);

	return matrixCurrentSource;
}

template <class T = long double>
Matrix <T> getMatrixVoltageSource(std::vector <T> const & voltageSources)
{
	Matrix <T> matrixVoltageSource((int)voltageSources.size(), 1);
	for(int row = 0; row < matrixVoltageSource.getRows(); ++row)
		matrixVoltageSource.setElement(row, 0, voltageSources[row]);

	return matrixVoltageSource;
}

template <class T = long double>
Matrix <T> getMatrixILoop(
		Matrix <T> const & B,
		Matrix <T> const & matrixImpedence,
		Matrix <T> const & matrixCurrentSource,
		Matrix <T> const & matrixVoltageSource)
{
	Matrix <T> rightHandSide = (B * matrixVoltageSource) - (B * (matrixImpedence * matrixCurrentSource));
	Matrix <T> leftHandSide = B * (matrixImpedence * B.getTranspose());

	return leftHandSide.getInverse() * rightHandSide;
}

template <class T = long double>
Matrix <T> getMatrixJBranch (Matrix <T> const & iLoop, Matrix <T> const & B)
{
	return B.getTranspose() * iLoop;
}

template <class T = long double>
Matrix <T> getMatrixVBranch (
		Matrix <T> const & jBranch,
		Matrix <T> const & matrixImpedence,
		Matrix <T> const & matrixCurrentSource,
		Matrix <T> const & matrixVoltageSource)
{
	Matrix <long double> vBranch = matrixImpedence * (jBranch + matrixCurrentSource) - matrixVoltageSource;

	return vBranch;
}

#endif // EQUATIONS_H

