//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"

#define MLP_SIZE 4

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784}, {64, 128}, {20, 64}, {10, 20}};
const MatrixDims biasDims[]    = {{128, 1}, {64, 1}, {20, 1},  {10, 1}};

// Insert MlpNetwork class here...

#endif // MLPNETWORK_H
