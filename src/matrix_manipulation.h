// Copyright(c) 2011 Andrew Que <http://www.DrQue.net>
// Copyright(c) 2021 Abd El-Twab M. Fakhry <abdeltwab.m.fakhry at protonmail dot com>

/**
	USES:
		Class for matrix math functions.

	LAST UPDATE:
		14/12/2021

	NOTES:
		This unit implements some very basic matrix functions, which include:
 *  + Addition/subtraction
 *  + Transpose
 *  + Determinant
 *  + Dot product
 *  + Matrix product
 *  + Scalar product
 *  + Inversion
 *  + LU factorization/decomposition

	LICENSE:
		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, either version 3 of the License, or
		(at your option) any later version.
**/

#ifndef MATRIX_MANIPULATION_H
#define MATRIX_MANIPULATION_H

#include <climits>
#include <iostream>
#include <vector>
#include <cstddef>

#include <assert.h>

template <class T = long long>
class IdentityMatrix;

template <class T = long long>
class Matrix
{
	protected:
		// Matrix dimensions
		unsigned int rows;
		unsigned int columns;

		// Storage for matrix data
		std::vector <std::vector <T>> matrix;

		// Order sub-index for rows.
		// Use: matrix[order[row]][column].
		std::vector <unsigned int> order;

		template <class ForwardIterator>
		void iota(ForwardIterator first, ForwardIterator last, unsigned int value)
		{
			while(first != last)
			{
				*first = value;
				++first;
				++value;
			}
		}

		/** Allocate memory for matrix data */
		void allocate(unsigned int numberOfRows, unsigned int numberOfColumns)
		{
			rows = numberOfRows;
			columns = numberOfColumns;

			T const ZERO = static_cast <T>(0);

			matrix.assign(rows, std::vector <T>(columns, ZERO));
			order.resize(rows);

			iota(order.begin(), order.end(), 0);
		}

		/** Return the number of leading zeros in the given row */
		unsigned int getLeadingZeros(unsigned int row) const
		{
			assert(row < rows);

			T ZERO = static_cast <T>(0);
			unsigned int column = 0;
			while(column < columns && matrix[row][column] == ZERO)
			{
				++column;
			}

			return column;
		}

		/** Reorder the matrix according to the number of leading zeros, descendingly */
		void reorder()
		{
			unsigned int* zeros = new unsigned int[rows];
			for(std::size_t row = 0; row < rows; ++row)
			{
				zeros[row] = getLeadingZeros(row);
			}

			// Bubble sort
			for(std::size_t row = rows - 1; row >= 0; --row)
			{
				unsigned int indexOfMax = row;
				for(std::size_t subRow = row - 1; subRow >= 0; --subRow)
				{
					if(zeros[subRow] > zeros[indexOfMax])
					{
						indexOfMax = subRow;
					}
				}

				if(indexOfMax != row)
				{
					std::swap(order[indexOfMax], order[row]);
				}
			}
			delete [] zeros;
		}

		/** Divide a row by given value.  An elementary row operation. */
		void divideRow(unsigned int row, T const & divisor)
		{
			for(std::size_t column = 0; column < columns; ++column)
			{
				matrix[row][column] /= divisor;
			}
		}

		/** Modify a row by adding a scaled row. An elementary row operation. */
		void rowOperation(unsigned int row, unsigned int addRow, T const & scale)
		{
			for(std::size_t column = 0; column < columns; ++column)
			{
				matrix[row][column] += matrix[addRow][column] * scale;
			}
		}

	public:
		/** Used for matrix concatenation. */
		typedef enum
		{
			TO_RIGHT,
			TO_BOTTOM
		} Position;

		// Constructor using rows and columns.
		Matrix(unsigned int numberOfRows, unsigned int numberOfColumns)
		{
			allocate(numberOfRows, numberOfColumns);
		}

		/**	Constructor for empty matrix.
				Only useful if matrix is being assigned(i.e. copied) from
				somewhere else sometime after construction.
		*/
		Matrix() :
			rows(0),
			columns(0)
		{
			allocate(0, 0);
		}

		/**	This constructor will allow the creation of a matrix based off
				an other matrix.  It can copy the matrix entirely, or omitted a
				row/column.
		*/
		Matrix(Matrix const & copyMatrix, unsigned int omittedRow = INT_MAX, unsigned int omittedColumn = INT_MAX)
		{
			// Start with the number of rows/columns from matrix to be copied.
			rows    = copyMatrix.getRows();
			columns = copyMatrix.getColumns();

			// If a row is omitted, then there is one less row.
			if(INT_MAX != omittedRow)
				--rows;

			// If a column is omitted, then there is one less column.
			if(INT_MAX != omittedColumn)
				--columns;

			// Allocate memory for new matrix.
			allocate(rows, columns);

			unsigned int rowIndex = 0;
			for(std::size_t row = 0; row < rows; ++row)
			{
				// If this row is to be skipped...
				if(rowIndex == omittedRow)
					rowIndex++;

				// Set default order.
				order[row] = row;

				unsigned int columnIndex = 0;
				for(std::size_t column = 0; column < columns; ++column)
				{
					// If this column is to be skipped...
					if(columnIndex == omittedColumn)
						++columnIndex;

					matrix[row][column] = copyMatrix.matrix[rowIndex][columnIndex];
					++columnIndex;
				}
				++rowIndex;
			}
		}

		/**	Constructor to concatenate two matrices.  Concatenation
				can be done to the right, or to the bottom.
				A = [B | C]
		*/
		Matrix(Matrix const & copyMatrixA, Matrix const & copyMatrixB, Position position = TO_RIGHT)
		{
			unsigned int rowOffset    = 0;
			unsigned int columnOffset = 0;

			if(TO_RIGHT == position)
				columnOffset = copyMatrixA.columns;
			else
				rowOffset = copyMatrixA.rows;

			rows    = copyMatrixA.rows    + rowOffset;
			columns = copyMatrixA.columns + columnOffset;

			// Allocate memory for new matrix.
			allocate(rows, columns);

			for(std::size_t row = 0; row < copyMatrixA.rows; ++row)
				for(std::size_t column = 0; column < copyMatrixA.columns; ++column)
					matrix[row][column] = copyMatrixA.matrix[row][column];

			for(std::size_t row = 0; row < copyMatrixB.rows; ++row)
				for(std::size_t column = 0; column < copyMatrixB.columns; ++column)
					matrix[row + rowOffset][column + columnOffset] = copyMatrixB.matrix[row][column];
		}

		/** Return the number of rows in this matrix */
		unsigned int getRows() const
		{
			return rows;
		}

		/** Return the number of columns in this matrix */
		unsigned int getColumns() const
		{
			return columns;
		}

		/** Get an element of the matrix */
		T getElement(unsigned int row, unsigned int column) const
		{
			assert(row < rows);
			assert(column < columns);

			return matrix[row][column];
		}

		/** Set an element in the matrix */
		void setElement(unsigned int row, unsigned int column, T const & value)
		{
			assert(row < rows);
			assert(column < columns);

			matrix[row][column] = value;
		}

		/** Proform LU decomposition. This will create matrices L and U such that A = L x U */
		void LU_Decomposition(Matrix & upper, Matrix & lower) const
		{
			assert(rows == columns);

			T const ZERO = static_cast <T>(0);
			upper = *this;
			lower = *this;

			for(std::size_t row = 0; row < rows; ++row)
				for(std::size_t column = 0; column < columns; ++column)
					lower.matrix[row][column] = ZERO;

			for(std::size_t row = 0; row < rows; ++row)
			{
				T value = upper.matrix[row][row];
				if(ZERO != value)
				{
					upper.divideRow(row, value);
					lower.matrix[row][row] = value;
				}

				for(std::size_t subRow = row + 1; subRow < rows; ++subRow)
				{
					T value = upper.matrix[subRow][row];
					upper.rowOperation(subRow, row, -value);
					lower.matrix[subRow][row] = value;
				}
			}
		}

		/**
			Return part of the matrix.
			NOTE: The end points are the last elements copied.  They can
			be equal to the first element when wanting just a single row
			or column.  However, the span of the total matrix is
(0, rows - 1, 0, columns - 1).
		*/
		Matrix getSubMatrix(unsigned int startRow, unsigned int endRow,
												unsigned int startColumn, unsigned int endColumn,
												unsigned int const* newOrder = nullptr)
		{
			Matrix subMatrix(endRow - startRow + 1, endColumn - startColumn + 1);

			for(std::size_t row = startRow; row <= endRow; ++row)
			{
				unsigned subRow;
				if(newOrder == nullptr)
					subRow = row;
				else
					subRow = newOrder[row];

				for(std::size_t column = startColumn; column <= endColumn; ++column)
					subMatrix.matrix[row - startRow][column - startColumn] = matrix[subRow][column];
			}

			return subMatrix;
		}

		/** Return a single column from the matrix. */
		Matrix getColumn(unsigned int column)
		{
			return getSubMatrix(0, rows - 1, column, column);
		}

		/** Return a single row from the matrix. */
		Matrix getRow(unsigned int row)
		{
			return getSubMatrix(row, row, 0, columns - 1);
		}

		/** Place matrix in reduced row echelon form. */
		void reducedRowEcholon()
		{
			T const ZERO = static_cast <T>(0);

			// For each row...
			for(std::size_t rowIndex = 0; rowIndex < rows; ++rowIndex)
			{
				// Reorder the rows.
				reorder();

				unsigned row = order[rowIndex];

				// Divide row down so first term is 1.
				unsigned column = getLeadingZeros(row);
				T divisor = matrix[row][column];
				if(ZERO != divisor)
				{
					divideRow(row, divisor);

					// Subtract this row from all subsequent rows.
					for(std::size_t subRowIndex =(rowIndex + 1); subRowIndex < rows; ++subRowIndex)
					{
						unsigned int subRow = order[subRowIndex];
						if(ZERO != matrix[subRow][column])
							rowOperation(subRow, row, -matrix[subRow][column]);
					}
				}
			}

			// Back substitute all lower rows.
			for(std::size_t rowIndex =(rows - 1); rowIndex > 0; --rowIndex)
			{
				unsigned row = order[rowIndex];
				unsigned column = getLeadingZeros(row);
				for(std::size_t subRowIndex = 0; subRowIndex < rowIndex; ++subRowIndex)
				{
					unsigned subRow = order[subRowIndex];
					rowOperation(subRow, row, -matrix[subRow][column]);
				}
			}
		} // reducedRowEcholon

		/** Return the determinant of the matrix.
				Recursive function.
		*/
		T determinant() const
		{
			T result = static_cast <T>(0);

			// Must have a square matrix to even bother.
			assert(rows == columns);

			if(rows > 2)
			{
				int sign = 1;
				for(std::size_t column = 0; column < columns; ++column)
				{
					T subDeterminant;

					Matrix subMatrix = Matrix(*this, 0, column);

					subDeterminant  = subMatrix.determinant();
					subDeterminant *= matrix[0][column];

					if(sign > 0)
						result += subDeterminant;
					else
						result -= subDeterminant;

					sign = -sign;
				}
			}
			else
			{
				result =(matrix[0][0] * matrix[1][1])
								-(matrix[0][1] * matrix[1][0]);
			}

			return result;
		} // determinant

		/** Calculate a dot product between this and an other matrix. */
		T dotProduct(Matrix const & otherMatrix) const
		{
			// Dimentions of each matrix must be the same.
			assert(rows == otherMatrix.rows);
			assert(columns == otherMatrix.columns);

			T result = static_cast <T>(0);
			for(std::size_t row = 0; row < rows; ++row)
				for(std::size_t column = 0; column < columns; ++column)
				{
					result += matrix[row][column] * 1ll * otherMatrix.matrix[row][column];
				}

			return result;

		} // dotProduct

		/** Return the transpose of the matrix. */
		Matrix const getTranspose() const
		{
			Matrix result(columns, rows);

			/** Transpose the matrix by filling the result's rows will these columns, and vica versa. */
			for(std::size_t row = 0; row < rows; ++row)
				for(std::size_t column = 0; column < columns; ++column)
					result.matrix[column][row] = matrix[row][column];

			return result;
		} // transpose

		/** Transpose the matrix. */
		void transpose()
		{
			*this = getTranspose();
		}

		/** Return inverse matrix. */
		Matrix const getInverse() const
		{
			// Concatenate the identity matrix onto this matrix.
			Matrix inverseMatrix(*this, IdentityMatrix <T>(rows, columns), TO_RIGHT);

			// Row reduce this matrix.  This will result in the identity
			// matrix on the left, and the inverse matrix on the right.
			inverseMatrix.reducedRowEcholon();

			// Copy the inverse matrix data back to this matrix.
			Matrix result(inverseMatrix.getSubMatrix(0, rows - 1, columns, columns + columns - 1, inverseMatrix.order));

			return result;
		} // invert

		/** Invert this matrix. */
		void invert()
		{
			*this = getInverse();
		}

		/** Operators. */
		/** Add by an other matrix. */
		Matrix const operator +(Matrix const & otherMatrix) const
		{
			assert(otherMatrix.rows == rows);
			assert(otherMatrix.columns == columns);

			Matrix result(rows, columns);

			for(std::size_t row = 0; row < rows; ++row)
				for(std::size_t column = 0; column < columns; ++column)
					result.matrix[row][column] = matrix[row][column] + otherMatrix.matrix[row][column];

			return result;
		}

		/** Add self by an other matrix. */
		Matrix const & operator +=(Matrix const & otherMatrix)
		{
			*this = *this + otherMatrix;

			return *this;
		}

		/** Subtract by an other matrix. */
		Matrix const operator -(Matrix const & otherMatrix) const
		{
			assert(otherMatrix.rows == rows);
			assert(otherMatrix.columns == columns);

			Matrix result(rows, columns);

			for(std::size_t row = 0; row < rows; ++row)
				for(std::size_t column = 0; column < columns; ++column)
					result.matrix[row][column] = matrix[row][column] - otherMatrix.matrix[row][column];

			return result;
		}

		/** Subtract self by an other matrix. */
		Matrix const & operator -=(Matrix const & otherMatrix)
		{
			*this = *this - otherMatrix;

			return *this;
		}

		/** Matrix multiplication. */
		Matrix const operator *(Matrix const & otherMatrix) const
		{
			T const ZERO = static_cast <T>(0);

			assert(columns == otherMatrix.rows);

			Matrix result(rows, otherMatrix.columns);

			for(std::size_t row = 0; row < rows; ++row)
				for(std::size_t column = 0; column < otherMatrix.columns; ++column)
				{
					result.matrix[row][column] = ZERO;

					for(std::size_t index = 0; index < columns; ++index)
						result.matrix[row][column] += matrix[row][index] * 1ll * otherMatrix.matrix[index][column];
				}

			return result;
		}

		/** Multiply self by matrix. */
		Matrix const & operator *=(Matrix const & otherMatrix)
		{
			*this = *this * otherMatrix;

			return *this;
		}

		/** Multiply by scalar constant. */
		Matrix const operator *(T const & scalar) const
		{
			Matrix result(rows, columns);

			for(std::size_t row = 0; row < rows; ++row)
				for(std::size_t column = 0; column < columns; ++column)
					result.matrix[row][column] = matrix[row][column] * scalar;

			return result;
		}

		/** Multiply self by scalar constant. */
		Matrix const & operator *=(T const & scalar)
		{
			*this = *this * scalar;

			return *this;
		}

		/** Assign matrix. */
		Matrix & operator =(Matrix const & otherMatrix)
		{
			if(this == &otherMatrix)
				return *this;

			allocate(rows, columns);

			for(std::size_t row = 0; row < rows; ++row)
				for(std::size_t column = 0; column < columns; ++column)
					matrix[row][column] = otherMatrix.matrix[row][column];

			return *this;
		}

		/** Copy matrix data from array.
				Although matrix data is two dimensional, this copy function
				assumes the previous row is immediately followed by the next
				row's data.

				Example for 3x2 matrix:
					int const data[3 * 2] =
					{
						1, 2, 3,
						4, 5, 6
					};
					Matrix <int> matrix(3, 2);
					matrix = data;
		*/
		Matrix & operator =(T const * data)
		{
			unsigned int index = 0;

			for(std::size_t row = 0; row < rows; ++row)
				for(std::size_t column = 0; column < columns; ++column)
					matrix[row][column] = data[index++];

			return *this;
		}

		/** Return true if this matrix is the same of parameter. */
		bool operator ==(Matrix const & value) const
		{
			for(std::size_t row = 0; row < rows; ++row)
				for(std::size_t column = 0; column < columns; ++column)
					if(matrix[row][column] != value.matrix[row][column])
						return false;

			return true;
		}

		/** Return true if this matrix is NOT the same of parameter. */
		bool operator !=(Matrix const & value) const
		{
			return !(*this == value);
		}
};

/** Class for identity matrix. */
template <class T>
class IdentityMatrix : public Matrix <T>
{
	public:
		IdentityMatrix(unsigned int rowsParameter, unsigned int columnsParameter) :
			Matrix <T>(rowsParameter, columnsParameter)
		{
			T const ZERO = static_cast <T>(0);
			T const ONE  = static_cast <T>(1);

			for(std::size_t row = 0; row < Matrix <T>::rows; ++row)
			{
				for(std::size_t column = 0; column < Matrix<T>::columns; ++column)
					if(row == column)
						Matrix <T>::matrix[row][column] = ONE;
					else
						Matrix <T>::matrix[row][column] = ZERO;
			}
		}
};

/** Stream operator used to convert matrix class to a string. */
template <class T>
std::ostream & operator <<(std::ostream & stream, Matrix <T> const & matrix)
{
	for(std::size_t row = 0; row < matrix.getRows(); ++row)
		for(std::size_t column = 0; column < matrix.getColumns(); ++column)
			stream << matrix.get(row, column) << " \n"[column == matrix.getColumns() - 1];

	return stream;
}

#endif // MATRIX_MANIPULATION_H

