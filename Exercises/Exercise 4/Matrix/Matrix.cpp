//
// Created by Bar Melinarskiy - 318189982 on 6/17/2020.
//

#include "Matrix.h"

#define ERROR_INVALID_DIM "Error: Non valid Matrix dimensions"
#define ERROR_INVALID_RANGE "Error: out of range"
#define ERROR_INVALID_INSERT "Error: couldn't fill the matrix from the stream"
#define ERROR_INVALID_MULTI "Error: incorrect matrices dimensions in multiplication"
/**
* Set the data of the cell with the given index
* @param r - row index
* @param c - col index
* @param data - data to fill inside the Matrix
*/
Matrix::Matrix(const int rows, const int cols)
{
	if(rows < 0 || cols < 0) //check the dim are valid
	{
		cerr << ERROR_INVALID_DIM << endl;
		exit(EXIT_FAILURE);
	}
	//Setting the params
	_rows = rows;
	_cols = cols;
	_matrix = new float[rows * cols];
	//Init the matrix values
	for(int i = 0; i < _rows; i++)
	{
		for(int j = 0 ; j < _cols; j++)
		{
			_setMatrixCellData(i, j, ZERO);
		}
	}
}
/**
* Default Constructor
* Constructs 1 X 1 Matrix & Inits the single element to 0
*/
 Matrix::Matrix():  _rows(ONE), _cols(ONE),  _matrix(new float[ONE])
{
	//Init the matrix value
	_setMatrixCellData(ZERO, ZERO, ZERO);
}
/**
* Copy Constructor
* Constructs matrix from another Matrix
* @param other - the existing Matrix to copy from
*/
 Matrix::Matrix(const Matrix &other): _rows(other.getRows()), _cols(other.getCols()),
									_matrix(new float[other.getRows() * other.getCols()])
{
	//Init the matrix values
	for(int i = 0; i < _rows; i++)
	{
		for(int j = 0 ; j < _cols; j++)
		{
			_setMatrixCellData(i, j, other(i, j));
		}
	}
}
/**
* Destructor
*/
Matrix::~Matrix()
{
	delete[] _matrix; //delete the matrix array from memory
}
/**
* Get the Metrix's row size
* @return the row size
*/
int Matrix::getRows() const
{
	return _rows;
}
/**
* Get the Metrix's col size
* @return the col size
*/
int Matrix::getCols() const
{
	return _cols;
}
/**
* Transforms a matrix into a column vector
* @return a column vector
*/
Matrix& Matrix::vectorize()
{
	_rows = _cols * _rows;
	_cols = ONE;
	return *this;
}
/**
* Redefine = operator, Assignment
* @return the Matrix after the assignment
*/
Matrix& Matrix::operator = (const Matrix &other)
{
	if(this != &other) //check the received is diff from current Matrix
	{
		delete[] _matrix; //free old array
		//Set params
		_rows = other.getRows();
		_cols = other.getCols();
		_matrix = new float[_rows * _cols];
		//Set cells values
		for(int i = 0; i < _rows; i++)
		{
			for(int j = 0 ; j < _cols; j++)
			{
				_setMatrixCellData(i, j, other(i, j));
			}
		}
	}
	return *this;
}
/**
* Redefine Matrix1 * Matrix2 operator, multiplication
* @param rhsMatrix - the rhs Matrix to multiple with
* @return the Matrix after the multiplication, if the sizes don't match then we exit with
* error code 1
*/
Matrix& Matrix::operator * (const float c)
{
	for(int j = 0; j < _rows; j++)
	{
		for(int i = 0 ; i < _cols; i++)
		{
			float newValue = _matrix[_getIndex(j, i)] * c;
			_setMatrixCellData(i, j, newValue);
		}
	}
	return *this;
}
/**
*  Redefine c * Matrix  operator, Scalar multiplication on the right
* @param c - the scalar to multiple with
* @param rhsMatrix - the rhs Matrix to multiple with
* @return the Matrix after the multiplication
*/
Matrix operator * (const float c, const Matrix &rhsMatrix)
{
	// this will hold our result
	Matrix result = Matrix(rhsMatrix.getRows(), rhsMatrix.getCols());

	for(int j = 0; j < rhsMatrix.getRows(); j++)
	{
		for(int i = 0 ; i < rhsMatrix.getCols(); i++)
		{
			float newValue = rhsMatrix(j, i) * c;
			result(j, i) =  newValue;
		}
	}
	return result;
}
/**
* Set the data of the cell with the given index
* @param r - row index
* @param c - col index
* @param data - data to fill inside the Matrix
*/
void Matrix::_setMatrixCellData(const int r, const int c, const float data)
{
	int index = this->_getIndex(r, c);
	if(index != -1) //check the index is valid - meaning it is in Matrix's range
	{
		_matrix[index] = data;
	}
	else
	{
		exit(EXIT_FAILURE);
	}
}
/**
* Return position in Matrix of cell (r,c);
* @param r - row index
* @param c - col index
* @return the index of the cell, if the given index is out of range then we exist the
* program with return code 1
*/
int Matrix::_getIndex(const int r, const int c) const
{
	if(this->_isValidIndex(r, c))
	{
		return r * _cols + c;
	}
	else
	{
		cout << ERROR_INVALID_RANGE << endl;
		exit(1);
	}
}

/**
* Check the position in Matrix exist;
* @param r - row index
* @param c - col index
* @return true if the position is valid, false otherwise
*/
bool Matrix::_isValidIndex(const int r, const int c) const
{
	if(r >= 0 && r <= _rows && c >= 0 && c <= _cols)
	{
		return true;
	}
	return false;
}
/**
*  Matrix addition accumulation
* @param rhs - the rhs Matrix to sum with
* @return the Matrix after the addition
*/
Matrix& Matrix::operator += (const Matrix &rhs)
{
	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _cols; j++)
		{
			float newValue = (*this)(i, j) + rhs(i, j);
			_setMatrixCellData(i, j, newValue);
		}
	}
	return *this;
}
/**
*  Matrix addition
* @param lhs - the lhs Matrix to sum with
* @param rhs - the rhs Matrix to sum with
* @return the Matrix after the addition
*/
Matrix operator + (const Matrix &lhs, const Matrix &rhs)
{
	Matrix result = Matrix(lhs);
	result += rhs; //reuse +=
	return result;
}
/**
*  Parenthesis indexing
* @param r - the row index
* @param c - the col index
* @return the r,c element in the matrix
*/
float& Matrix::operator() (const int r, const int c) const
{
	int index = _getIndex(r, c);
	return _matrix[index];
}
/**
*  Brackets indexing
* @param i - the element's index
* @return the i’th element in the matrix
*/
float& Matrix::operator [] (const int i) const
{
	return _matrix[i];
}
/**
* Prints matrix elements
* @return the col size
*/
void Matrix::plainPrint() const
{
	const string SPACE = " ";
	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _cols; j++)
		{
			cout << (*this)(i, j) << SPACE;
		}
		cout << endl;
	}
}
/**
*  Redefine << - print matrix to stream
* @param os - stream ref
* @param m - matrix to print
* @return the stream
*/
ostream &operator << (ostream &os, const Matrix &m)
{
	const float COMP_VALUE = 0.1f;
	const string DONT_MARK = "  ", MARK = "**";
	for (int i = 0; i < m.getRows(); i++)
	{
		for (int j = 0; j < m.getCols(); j++)
		{
			if(m(i, j) <= COMP_VALUE)
			{
				os << DONT_MARK;
			}
			else
			{
				os << MARK;
			}
		}
		cout << endl;
	}
	return os;
}
/**
*  Redefine >> - insert values into matrix from stream
* @param os - stream ref
* @param m - matrix to feel
* @return the stream
*/
istream& operator >> (istream& inStream, Matrix& m)
{
	const streamsize FLOAT_SIZE = sizeof(float);
	inStream.read((char*)m._matrix, m._rows * m._cols * FLOAT_SIZE);

	if (inStream.fail() || inStream.peek() != EOF)
	{
		cerr << ERROR_INVALID_INSERT << std::endl;
		exit(EXIT_FAILURE);
	}
	return inStream;
}
/**
* Redefine Matrix1 * Matrix2 operator, multiplication
* @param rhsMatrix - the rhs Matrix to multiple with
* @return the Matrix after the multiplication, if the sizes don't match then we exit with
* error code 1
*/
Matrix operator * (const Matrix &lhs, const Matrix &rhs)
{
	Matrix result; // this will hold our result
	if(lhs.getCols() == rhs.getRows())
	{
		result = Matrix(lhs.getRows(), rhs.getCols());
		for(int i = 0; i < lhs.getRows(); i++)
		{
			for(int j = 0 ; j < rhs.getCols(); j++)
			{
				for(int k = 0 ; k < lhs.getCols(); k++)
				{
					result(i, j) += lhs(i, k) * rhs(k, j);
				}
			}
		}
	}
	else
	{
		cerr << ERROR_INVALID_MULTI << endl;
	}

	return(result);
}
/**
* Set the data from given array
* @param arr - array of data to fill inside the Matrix
*/
void Matrix::_setMatrixData(const float *arr, const int length)
{
	for(int i = 0 ; i < length; i++)
	{
		(*this)[i] = arr[i];
	}
}
