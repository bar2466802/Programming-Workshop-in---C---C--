//
// Created by Bar Melinarskiy - 318189982 on 6/17/2020.
//

#ifndef DENSE_H
#define DENSE_H

#include "Matrix.h"
#include "Activation.h"
/**
* @class Dense Class
* @property _bias - matrix array of bias
* @property _weights - matrix array of weights
* @property _act - the network, layers array
*/
class Dense
{
	private:
		Matrix _bias;
		Matrix _weights;
		Activation _act;
	public:
	/**
	* Constructor
	* Constructs Dense object with given 2 Matrixes and actType
	* @param w - weights of this layer
	* @param bias - bias of this layer
	* @param actType - activation type Relu / Softmax
	*/
	Dense(const Matrix w, const Matrix bias, const ActivationType actType);

	/**
	* Get the Activation object
	* @return the act object
	*/
	Activation getActivation() const;

	/**
	* Get the bias of this layer
	* @return bias of this layer
	*/
	Matrix getBias() const;

	/**
	* Get the weights of this layer
	* @return weights of this layer
	*/
	Matrix getWeights() const;

	/**
	*  Redefine Parenthesis operator
	* @param m - the matrix to activate
	* @return new matrix after activation
	*/
	Matrix operator () (const Matrix& r) const;

};


#endif //DENSE_H
