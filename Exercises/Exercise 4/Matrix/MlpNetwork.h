//
// Created by Bar Melinarskiy - 318189982 on 6/17/2020.
//

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"
#include "Digit.h"
#include "Matrix.h"

#define MLP_SIZE 4
#define FOURTH 3

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784}, {64, 128}, {20, 64}, {10, 20}};
const MatrixDims biasDims[] = {{128, 1}, {64, 1}, {20, 1},  {10, 1}};

/**
* @class MlpNetwork Class
* @property _layers - the network, layers array
*/
class MlpNetwork
{
	private:
	Dense _layers[MLP_SIZE] = {
					Dense(Matrix(weightsDims[0].rows, weightsDims[0].cols),
					Matrix(biasDims[0].rows, biasDims[0].cols), Relu),
					Dense(Matrix(weightsDims[1].rows, weightsDims[1].cols),
						  Matrix(biasDims[1].rows, biasDims[1].cols), Relu),
					Dense(Matrix(weightsDims[2].rows, weightsDims[2].cols),
						  Matrix(biasDims[2].rows, biasDims[2].cols), Relu),
					Dense(Matrix(weightsDims[3].rows, weightsDims[3].cols),
						  Matrix(biasDims[3].rows, biasDims[3].cols), Softmax)
			};
	public:
			/**
			* Constructor
			* Constructs a Network from the given data
			* @param weights - array of weights
			* @param biases - array of biases
			*/
			MlpNetwork(Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE]);

			/**
			*  Parenthesis, active the network on the given image
			* @param imgVec - image to check
			* @return the digit info
			*/
			Digit operator () (const Matrix imgVec) const;

};

#endif //MLPNETWORK_H
