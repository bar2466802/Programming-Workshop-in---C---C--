//
// Created by Bar Melinarskiy - 318189982 on 6/17/2020.
//

#include "Activation.h"
/**
* Constructor
* Constructs Activation object with given actType
* @param actType - activation type Relu / Softmax
*/
Activation::Activation(ActivationType actType): _actType(actType)
{

}
/**
* Get the Activation's type
* @return the actType
*/
ActivationType Activation::getActivationType() const
{
	return _actType;
}
/**
* Relu activation function
* @param m - matrix to activate
*/
Matrix Activation::_relu(const Matrix &m) const
{
	Matrix newMatrix = Matrix(m);

	for(int i = 0 ; i < m.getRows(); i++)
	{
		if(newMatrix[i] < 0)
		{
			newMatrix[i] = 0;
		}
	}

	return newMatrix;
}
/**
* Softmax activation function
* @param m - matrix to activate
*/
Matrix Activation::_softmax(const Matrix &m) const
{
	Matrix newMatrix = Matrix(m);
	for(int i = 0 ; i < m.getRows(); i++)
	{
		newMatrix[i] = exp(newMatrix[i]);
	}

	float leftScalar = 0;
	for(int i = 0 ; i < newMatrix.getRows(); i++)
	{
		leftScalar += newMatrix[i];
	}

	if(leftScalar != 0)
	{
		leftScalar = 1 / leftScalar;
	}

	return leftScalar * newMatrix;
}
/**
*  Redefine Parenthesis operator
* @param m - the matrix to activate
* @return new matrix after activation
*/
Matrix Activation::operator () (const Matrix& m) const
{
	Matrix newMatrix = Matrix(m);
	newMatrix.vectorize();
	if(_actType == Relu)
	{
		newMatrix = _relu(newMatrix);
	}
	else
	{
		newMatrix = _softmax(newMatrix);
	}

	return newMatrix;
}
