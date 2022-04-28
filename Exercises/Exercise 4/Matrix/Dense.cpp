//
// Created by Bar Melinarskiy - 318189982 on 6/17/2020.
//

#include "Dense.h"
/**
* Constructor
* Constructs Dense object with given 2 Matrixes and actType
* @param w - weights of this layer
* @param bias - bias of this layer
* @param actType - activation type Relu / Softmax
*/
Dense::Dense(const Matrix w, const Matrix bias, const ActivationType actType): _bias(bias),
		_weights(w), _act(actType)
{

}
/**
* Get the Activation object
* @return the act object
*/
Activation Dense::getActivation() const
{
	return _act;
}
/**
* Get the bias of this layer
* @return bias of this layer
*/
Matrix Dense::getBias() const
{
	return _bias;
}
/**
* Get the weights of this layer
* @return weights of this layer
*/
Matrix Dense::getWeights() const
{
	return _weights;
}
/**
*  Redefine Parenthesis operator
* @param m - the matrix to activate
* @return new matrix after activation
*/
Matrix Dense::operator()(const Matrix &r) const
{
	Matrix output = _act(((_weights * r) + _bias));
	return output;
}

