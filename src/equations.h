#ifndef EQUATIONS_H
#define EQUATIONS_H

#include "inputs.h"
#include "matrix_manipulation.h"

#include <vector>

template <class T = long double>
Matrix <T> getA(
		std::vector <char> orderedTreeBranches,
		std::map <char, std::pair <int, int>> & branchNameToItsNodes,
		int & nodes,
		int & branches)
{
	std::vector <char> orderedBranches = getBranchesOrder(orderedTreeBranches, branchNameToItsNodes);

	Matrix <T> a(nodes, branches);

	for(int branch = 0; branch < branches; ++branch)
	{
		a.setElement(branchNameToItsNodes[orderedBranches[branch]].first,  branch,  1.0L);
		a.setElement(branchNameToItsNodes[orderedBranches[branch]].second, branch, -1.0L);
	}

	return a;
}

template <class T = long double>
Matrix <T> getATree(
		std::vector <char> orderedTreeBranches,
		std::map <char, std::pair <int, int>> & branchNameToItsNodes,
		int nodes,
		int branches)
{
	Matrix <T> a = getA(orderedTreeBranches, branchNameToItsNodes, nodes, branches);
	Matrix <T> aTree(a, 0, nodes - 2, 0, (int)orderedTreeBranches.size() - 1);

	return aTree;
}

template <class T = long double>
Matrix <T> getALink(
		std::vector <char> orderedTreeBranches,
		std::map <char, std::pair <int, int>> & branchNameToItsNodes,
		int nodes,
		int branches)
{
	Matrix <T> a = getA(orderedTreeBranches, branchNameToItsNodes, nodes, branches);
	Matrix <T> aLink(a, 0, nodes - 2, (int)orderedTreeBranches.size(), branches - 1);

	return aLink;
}

template <class T = long double>
Matrix <T> getCLink(Matrix <T> const & aTree, Matrix <T> const & aLink)
{
	Matrix <T> aTreeInverse = aTree.getInverse();

	return (aTreeInverse * aLink);
}

template <class T = long double>
Matrix <T> getBTree(Matrix <T> const & aTree, Matrix <T> const & aLink)
{
	Matrix <T> cLink = getCLink(aTree, aLink);

	return (cLink.getTranspose() * -1.0L);
}

template <class T = long double>
Matrix <T> getC(Matrix <T> const & aTree, Matrix <T> const & aLink)
{
	Matrix <T> cLink = getCLink(aTree, aLink);
	IdentityMatrix <T> cTree(cLink.getRows(), cLink.getRows());

	Matrix <T> c(cTree, cLink, Matrix<T>::Position::TO_RIGHT);

	return c;
}

template <class T = long double>
Matrix <T> getB(Matrix <T> const & aTree, Matrix <T> const & aLink)
{
	Matrix <T> bTree = getBTree(aTree, aLink);
	IdentityMatrix <T> bLink(bTree.getRows(), bTree.getRows());

	Matrix <T> b(bTree, bLink, Matrix<T>::Position::TO_RIGHT);

	return b;
}

template <class T = long double>
Matrix <T> getImpedence(std::vector <T> const & resistors)
{
	int resistorsSize = resistors.size();
	Matrix <T> impedence(resistorsSize, resistorsSize);

	for(int row = 0; row < impedence.getRows(); ++row)
			impedence.setElement(row, row, resistors[row]);

	return impedence * IdentityMatrix <T> (impedence.getRows(), impedence.getRows());
}

template <class T = long double>
Matrix <T> getCurrentSource(std::vector <T> const & currentSources)
{
	Matrix <T> currentSource((int)currentSources.size(), 1);
	for(int row = 0; row < currentSource.getRows(); ++row)
		currentSource.setElement(row, 0, currentSources[row]);

	return currentSource;
}

template <class T = long double>
Matrix <T> getVoltageSource(std::vector <T> const & voltageSources)
{
	Matrix <T> voltageSource((int)voltageSources.size(), 1);
	for(int row = 0; row < voltageSource.getRows(); ++row)
		voltageSource.setElement(row, 0, voltageSources[row]);

	return voltageSource;
}

template <class T = long double>
Matrix <T> getILoop(
		Matrix <T> const & b,
		Matrix <T> const & impedence,
		Matrix <T> const & currentSource,
		Matrix <T> const & voltageSource)
{
	Matrix <T> rightHandSide = (b * voltageSource) - (b * (impedence * currentSource));
	Matrix <T> leftHandSide = b * (impedence * b.getTranspose());

	return leftHandSide.getInverse() * rightHandSide;
}

template <class T = long double>
Matrix <T> getJBranch(Matrix <T> const & iLoop, Matrix <T> const & b)
{
	return b.getTranspose() * iLoop;
}

template <class T = long double>
Matrix <T> getVBranch(
		Matrix <T> const & jBranch,
		Matrix <T> const & impedence,
		Matrix <T> const & currentSource,
		Matrix <T> const & voltageSource)
{
	Matrix <long double> vBranch = impedence * (jBranch + currentSource) - voltageSource;

	return vBranch;
}

#endif // EQUATIONS_H

