/**
 * Ex3 - Bar Melinarskiy - 318189982
 * Exercise 3 - Tree of Vector implementation
 */
#ifndef STRUCTS_C
#define STRUCTS_C

#include "RBTree.h"
#include "Structs.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LESS (-1)
#define EQUAL (0)
#define GREATER (1)
#define SUCCESS_CODE 1
#define ERROR_CODE 0

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
	char* string1 = (char*)a;
	char* string2 = (char*)b;
	int res = strcmp(string1, string2);

	if(res == 0)
	{
		return EQUAL; //strings are the same
	}
	else if(res < 0)
	{
		return LESS; //string 1 is smaller
	}
	else
	{
		return GREATER; //string 1 is greater
	}
}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
	const char* NEW_LINE = "\n";
	if(word == NULL || pConcatenated == NULL) //check the given pointers are valid
	{
		return ERROR_CODE;
	}

	char* string = (char*) word;
	strcat(pConcatenated, string); //add the given word
	strcat(pConcatenated, NEW_LINE); //add the end-line char
	return  SUCCESS_CODE;
}

/**
 * FreeFunc for strings
 * @param s - string to free
 */
void freeString(void *s)
{
	if(s == NULL)
	{
		return;
	}
	free(s);
	s = NULL;
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
	Vector* vector1 = (Vector*) a;
	Vector* vector2 = (Vector*) b;
	int length = vector1->len;
	//Check which vector is longer
	if(vector2->len < vector1->len)
	{
		length = vector2->len;
	}
	int i = 0;
	//loop through the vector's double array until we find a diff or reach the end of one of them
	while(i < length)
	{
		double vector1Element = (double)vector1->vector[i];
		double vector2Element = (double)vector2->vector[i];
		if (vector1Element < vector2Element)
		{
			return LESS; //the first vector is smaller
		}
		else if (vector1Element > vector2Element)
		{
			return GREATER; //the first vector is bigger
		}
		i++;
	}
	//if we finished the loop & the vectors same up until now and have the same length
	// then they are equal
	if(vector2->len == vector1->len)
	{
		return EQUAL;
	}
	else if(vector2->len < vector1->len)
	{
		return GREATER;
	}
	return LESS;
}
/**
 * FreeFunc for vectors
 * @param pVector - vector to free
 */
void freeVector(void *pVector)
{
	if(pVector == NULL)
	{
		return;
	}
	Vector* vector = (Vector*) pVector;
	free(vector->vector);
	free(pVector);
	pVector = NULL;
}
/**
 * Calc the given vector's norm and return it
 * @param v - the given vector to calc it's norm
 * @return the norm of the given vector
 */
double calcNorm(Vector* v)
{
	const double ZERO = 0.0;
	const double POWER_OF_TWO = 2;
	if(v == NULL || v->vector == NULL) //check the given vector is valid
	{
		return ZERO;
	}
	int i = 0;
	double sum = 0;
	// add up the (elements)^2 of the vector
	while(i < v->len)
	{
		sum += pow(v->vector[i], POWER_OF_TWO);
		i++;
	}
	double norm = 0;
	if(sum > 0)
	{
		norm = sqrt(sum); // calc the norm
	}
	return  norm;
}
/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompareNorm(const void *a, const void *b)
{
	Vector* vector1 = (Vector*) a;
	Vector* vector2 = (Vector*) b;
	//Calc each vector's norm
	double norm1 = calcNorm(vector1);
	double norm2 = calcNorm(vector2);
	if(norm1 < norm2)
	{
		return  LESS;
	}
	else if(norm1 > norm2)
	{
		return  GREATER;
	}
	return EQUAL;
}
/**
 * Deep copy one vector to another
 * @param source - the vector to copy from
 * @param dest - the vector to copy to
 * @return 0 on failure, other on success
 */
int copyVectorToAnother(const Vector* source, Vector* dest)
{
	if(source == NULL || source->vector == NULL || source->len <= 0) //check valid inputs
	{
		return ERROR_CODE;
	}

	size_t  newSize = sizeof(double) * source->len; //calc the nre needed size
	//Check if dest vector already exist or not
	if(dest->vector == NULL)
	{
		dest->vector =  (double *) malloc(newSize); //new vector so ask for first time memory
	}
	else
	{
		//existing vector, resize the dynamic memory
		dest->vector =  (double *) realloc(dest->vector, newSize);
	}

	if(dest->vector == NULL) //check if the memory allocation was successful
	{
		return ERROR_CODE;
	}

	dest->len = source->len; //update the vector's length
	//Copy the element's from the source to the dest
	for(int i = 0; i < source->len; i++)
	{
		dest->vector[i] = source->vector[i];
	}
	return SUCCESS_CODE;
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector that will hold a copy of the data of pVector.
 * @return 1 on success, 0 on failure (if pVector == NULL || pMaxVector==NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
	if(pVector == NULL || pMaxVector == NULL) //check the pointers are valid
	{
		return ERROR_CODE;
	}
	int ok = SUCCESS_CODE;
	Vector* compareVector = (Vector*) pVector;
	Vector* maxVector = (Vector*) pMaxVector;
	//  2. pMaxVector->vector == NULL.
	if(maxVector->vector == NULL)
	{
		ok = copyVectorToAnother(compareVector, maxVector);
	}
	else
	{
		if(calcNorm(maxVector) < calcNorm(compareVector))
		{
			//1. The norm of pVector is greater then the norm of pMaxVector.
			ok = copyVectorToAnother(compareVector, maxVector);
		}
	}
	return ok;
}

/**
 * This function allocates memory it does not free.
 * @param tree: a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm), NULL on failure.
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
	if(tree == NULL || tree->root == NULL)
	{
		return NULL;
	}
	//Create the maxVector pointer
	Vector* maxVector = malloc(sizeof(Vector));
	if(maxVector == NULL)
	{
		return NULL;
	}
	maxVector->vector = NULL;
	maxVector->len = 0;
	//Loop through the tree finding the max vector in it
	if(forEachRBTree(tree, copyIfNormIsLarger, maxVector) == SUCCESS_CODE)
	{
		return  maxVector;
	}
	return NULL;
}

#endif // STRUCTS_C
