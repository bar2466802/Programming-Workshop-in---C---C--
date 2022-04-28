//
// Created by Bar Melinarskiy - 318189982 on 6/17/2020.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

#define ZERO 0
#define ONE 1

/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
	int rows, cols;
} MatrixDims;
/**
 * @class Matrix Class
 * @property _rows - rows size
 * @property _cols - cols size
 * @property _matrix - float value array
 */
class Matrix
{
	private:
		int _rows;
		int _cols;
		float* _matrix;
		/**
		* Set the data of the cell with the given index
		* @param r - row index
		* @param c - col index
		* @param data - data to fill inside the Matrix
		*/
		void _setMatrixCellData(const int r, const int c, const float data);

		/**
		* Set the data from given array
		* @param arr - array of data to fill inside the Matrix
		*/
		void _setMatrixData(const float arr [], const int length);

		/**
		* Return position in Matrix of cell (r,c);
		* @param r - row index
		* @param c - col index
		* @return the index of the cell, if the given index is out of range then we exist the
		* program with return code 1
		*/
		int _getIndex(const int r, const int c) const;
		/**
		* Check the position in Matrix exist;
		* @param r - row index
		* @param c - col index
		* @return true if the position is valid, false otherwise
		*/
		bool _isValidIndex(const int r, const int c) const;
	public:
		/**
		* Default Constructor
		* Constructs 1 X 1 Matrix & Inits the single element to 0
		*/
		explicit Matrix();
		/**
		* Constructor
		* Constructs Matrix with size rows X cols & Inits the all the elements to 0
		* @param rows - row size
		* @param cols - col size
		*/
		explicit Matrix(const int rows, const int cols);
		/**
		* Copy Constructor
		* Constructs matrix from another Matrix
		* @param other - the existing Matrix to copy from
		*/
		Matrix(const Matrix &other);

		/**
		* Destructor
		*/
		~Matrix();
		/**
		* Get the Metrix's row size
		* @return the row size
		*/
		int getRows() const;
		/**
		* Get the Metrix's col size
		* @return the col size
		*/
		int getCols() const;

		/**
		* Prints matrix elements
		* @return the col size
		*/
		void plainPrint() const;

		/**
		* Transforms a matrix into a column vector
		* @return a column vector
		*/
		Matrix& vectorize();

		/**
		* Redefine = operator, Assignment
		* @return the Matrix after the assignment
		*/
		Matrix& operator =(const Matrix & other);

		/**
		* Redefine Matrix1 * Matrix2 operator, multiplication
		* @param rhsMatrix - the rhs Matrix to multiple with
		* @return the Matrix after the multiplication, if the sizes don't match then we exit with
		* error code 1
		*/
		 friend Matrix operator * (const Matrix &lhs, const Matrix &rhs);

		/**
		*  Redefine Matrix * c operator, Scalar multiplication on the right
		* @param c - the scalar to multiple with
		* @return the Matrix after the multiplication
		*/
		Matrix& operator *(const float c);

		/**
		*  Redefine c * Matrix  operator, Scalar multiplication on the right
		* @param c - the scalar to multiple with
		* @param rhsMatrix - the rhs Matrix to multiple with
		* @return the Matrix after the multiplication
		*/
		friend Matrix operator *(const float c , const Matrix & rhsMatrix);

		/**
		*  Matrix addition accumulation
		* @param rhs - the rhs Matrix to sum with
		* @return the Matrix after the addition
		*/
		Matrix& operator += (const Matrix& rhs);
		/**
		*  Matrix addition
		* @param lhs - the lhs Matrix to sum with
		* @param rhs - the rhs Matrix to sum with
		* @return the Matrix after the addition
		*/
		friend Matrix operator + (const Matrix &lhs, const Matrix& rhs);
		/**
		*  Parenthesis indexing
		* @param r - the row index
		* @param c - the col index
		* @return the r,c element in the matrix
		*/
		float& operator () (const int r, const int c) const;

		/**
		*  Brackets indexing
		* @param i - the element's index
		* @return the i’th element in the matrix
		*/
		float& operator [] (const int i) const;
		/**
		*  Redefine << - print matrix to stream
		* @param os - stream ref
		* @param m - matrix to print
		* @return the stream
		*/
		friend ostream &operator<<(ostream &os, const Matrix &m);
		/**
		*  Redefine >> - insert values into matrix from stream
		* @param os - stream ref
		* @param m - matrix to feel
		* @return the stream
		*/
		friend istream& operator>>(istream& inStream, Matrix& m);
};


#endif //MATRIX_H
