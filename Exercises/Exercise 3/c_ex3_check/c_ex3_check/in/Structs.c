#ifndef STRUCTS_C
#define STRUCTS_C

#include "RBTree.h"
#include "Structs.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

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
		return EQUAL;
	}
	else if(res < 0)
	{
		return LESS;
	}
	else
	{
		return GREATER;
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
	if(word == NULL || pConcatenated == NULL)
	{
		return ERROR_CODE;
	}

	char* string = (char*) word;

	strcat(pConcatenated, string);
	strcat(pConcatenated, NEW_LINE);
	return  SUCCESS_CODE;
}

/**
 * FreeFunc for strings
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
	if(vector2->len < vector1->len)
	{
		length = vector2->len;
	}
	int i = 0;
	while(i < length)
	{
		double vector1Element = (double)vector1->vector[i];
		double vector2Element = (double)vector2->vector[i];
		if (vector1Element < vector2Element)
		{
			return LESS;
		}
		else if (vector1Element > vector2Element)
		{
			return GREATER;
		}
		i++;
//		printf("len is: %d, index is: %d", length, i);
	}

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

double calcNorm(Vector* v)
{
	const double ZERO = 0.0;
	const double POWER_OF_TWO = 2;
	if(v == NULL || v->vector == NULL)
	{
		return ZERO;
	}
	int i = 0;
	double sum = 0;
	while(i < v->len)
	{
		sum += pow(v->vector[i], POWER_OF_TWO);
		i++;
	}
	double norm = 0;
	if(sum > 0)
	{
		norm = sqrt(sum);
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
int  vectorCompareNorm(const void *a, const void *b)
{
	Vector* vector1 = (Vector*) a;
	Vector* vector2 = (Vector*) b;
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

int printV(const void *a, void *null)
{
	if(a == NULL)
	{
		printf("\t\t[NULL]");
	}

	if (null != NULL)
	{
		return 0;
	}

	Vector *v = (Vector*)a;

	printf("\t\t[");

	for (int i = 0; i < v->len; i++)
	{
		if(i != 0)
		{
			printf(", ");
		}
		printf("%f", (v->vector)[i]);
	}

	printf("] (size: %d)\n", v->len);
	return 1;
}


int copyVectorToAnother(const Vector* source, Vector* dest)
{
	if(source == NULL || source->vector == NULL || source->len <= 0)
	{
		return ERROR_CODE;
	}
//	printV(source, NULL);
//	printV(dest, NULL);

	size_t  newSize = sizeof(double) * source->len;
	if(dest->vector == NULL)
	{
		dest->vector =  (double *) malloc(newSize);
	}
	else
	{
		dest->vector =  (double *) realloc(dest->vector, newSize);
	}

	if(dest->vector == NULL)
	{
		return ERROR_CODE;
	}

//	dest->vector =  (double*) memcpy(dest->vector, source->vector, newSize);
//	if(dest->vector == NULL)
//	{
//		return ERROR_CODE;
//	}
	dest->len = source->len;

	for(int i = 0; i < source->len; i++)
	{
		dest->vector[i] = source->vector[i];
	}
//	printV(dest, NULL);
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
	if(pVector == NULL || pMaxVector == NULL)
	{
		return ERROR_CODE;
	}
	int ok = SUCCESS_CODE;
	Vector* compareVector = (Vector*) pVector;
	Vector* maxVector = (Vector*) pMaxVector;
	if(maxVector->vector == NULL)
	{
		ok = copyVectorToAnother(compareVector, maxVector);
	}
	else
	{
		if(calcNorm(maxVector) < calcNorm(compareVector))
		{
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
	Vector* maxVector = malloc(sizeof(Vector));
	if(maxVector == NULL)
	{
		return NULL;
	}
	maxVector->vector = NULL;
	maxVector->len = 0;

	if(forEachRBTree(tree, copyIfNormIsLarger, maxVector) == SUCCESS_CODE)
	{
		return  maxVector;
	}
	return NULL;
}


#endif // STRUCTS_C
