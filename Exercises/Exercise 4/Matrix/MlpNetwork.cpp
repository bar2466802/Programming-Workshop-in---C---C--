//
// Created by Bar Melinarskiy - 318189982 on 6/17/2020.
//

#include "MlpNetwork.h"
/**
* Constructor
* Constructs a Network from the given data
* @param weights - array of weights
* @param biases - array of biases
*/
MlpNetwork::MlpNetwork(Matrix *weights, Matrix *biases)
{
	for(int i = 0 ; i < MLP_SIZE; i++)
	{
		ActivationType actType = Relu;
		if(i == FOURTH)
		{
			actType = Softmax;
		}
		_layers[i] = Dense(weights[i], biases[i], actType);
	}
}
/**
*  Parenthesis, active the network on the given image
* @param imgVec - image to check
* @return the digit info
*/
Digit MlpNetwork::operator()(Matrix imgVec) const
{
	Matrix resultVector = imgVec;
	for(int i = 0 ; i < MLP_SIZE; i++)
	{
		Matrix inputMat = resultVector;
		resultVector = _layers[i](inputMat);
	}
	Digit d = {0 , 0};
	for(int j = 0; j < resultVector.getRows(); j++)
	{
		float value = resultVector[j];
		if(value > d.probability)
		{
			d.probability = value;
			d.value = j;
		}
	}
	return d;
}
