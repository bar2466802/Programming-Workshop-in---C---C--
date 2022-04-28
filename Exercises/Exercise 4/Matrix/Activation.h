//
// Created by Bar Melinarskiy - 318189982 on 6/17/2020.
//

#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
	Relu,
	Softmax
};
/**
 * @class Activation Class
 * @property _actType - activation function to call
 */
class Activation
{
	private:
			ActivationType _actType;
			/**
			* Relu activation function
			* @param m - matrix to activate
			*/
			Matrix _relu(const Matrix& m) const;
			/**
			* Softmax activation function
			* @param m - matrix to activate
			*/
			Matrix _softmax(const Matrix& m) const;
	public:
			/**
			* Constructor
			* Constructs Activation object with given actType
			* @param actType - activation type Relu / Softmax
			*/
			Activation(ActivationType actType);

			/**
			* Get the Activation's type
			* @return the actType
			*/
			ActivationType getActivationType() const;

			/**
			*  Redefine Parenthesis operator
			* @param m - the matrix to activate
			* @return new matrix after activation
			*/
			Matrix operator () (const Matrix& m) const;
};

#endif //ACTIVATION_H
