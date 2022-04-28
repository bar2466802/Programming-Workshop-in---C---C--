#define  _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <regex.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

#define  OUTPUT_FILE_NAME "railway_planner_output.txt"
#define  READ_AUTH "r"
#define  WRITE_AUTH "w"
#define  E_NO_FILE "File doesn't exists."
#define  E_EMPTY_FILE "File is empty."
#define  E_IN_LINE "Invalid input in line: %ld."

/**
* A structure to represent a road part
*/
typedef struct RoadPart
{
	char _leftType; // the left type connector
	char _rightType; // the right type connector
	long int _partLength; // the part's length - must be positive integer
	long int _partPrice; // the part's price - must be positive integer
}RoadPart;
/**
* A structure to represent the given road's info
*/
typedef struct RoadInfo
{
	long int _length; // the road's length
	long int _numberOfTypes; // the number of connectors' types
	char* _types; // an array of pointers containing the connectors types
	long int _numberOfParts; // total number of possible road's parts
	RoadPart* _parts; // an array of pointers containing the possible road's parts
}RoadInfo;

/**
* Print to the output file
* @param output - the wanted data to print into the file
* @return EXIT_SUCCESS if the operation was successful, EXIT_FAILURE otherwise
*/
void writeToOutputFile(const char* output)
{
	const char* FILE_PATH = OUTPUT_FILE_NAME;
	int isOk = 0;
	if(output == NULL)
	{
		exit(EXIT_FAILURE);
	}
	FILE *file = fopen(FILE_PATH, WRITE_AUTH); // try to open the output file
	if (file != NULL)
	{
		isOk = fputs(output, file); // try to write to the file
		if(isOk == EOF)
		{
			//an error occurred while trying to write to the output file
		}
		isOk = fclose(file);
		if(isOk == EOF)
		{
			//Error while closing the output file
			exit(EXIT_FAILURE);
		}
	}
	else //We couldn't open the output file
	{
		exit(EXIT_FAILURE);
	}
}

void freeRoadsTable(long int*** roadsTable, RoadInfo** roadInfo)
{
	for (long int l = 0; l <= (*roadInfo)->_length; l++)
	{
		free((*roadsTable)[l]);
		(*roadsTable)[l] = NULL;
	}
	free(*roadsTable);
	*roadsTable = NULL;
}

void freeBeforeExit(RoadInfo** roadInfo, long int*** table)
{
	if(table != NULL && *table != NULL)
	{
		freeRoadsTable(&(*table), roadInfo);
	}

	if((*roadInfo) != NULL)
	{
		if((*roadInfo)->_types != NULL)
		{
			free((*roadInfo)->_types);
			(*roadInfo)->_types = NULL;
		}
		if((*roadInfo)->_parts != NULL)
		{
			free((*roadInfo)->_parts);
			(*roadInfo)->_parts = NULL;
		}
		(*roadInfo) = NULL;
	}
}

/**
* Exit with failure
* @param error - the reason for failure
* @param file - the given file to close
* @param roadInfo - structure storing the given road's info
*/
void exitWithError(const char* error, const long int index, char** line, FILE** file,
		RoadInfo** roadInfo)
{
	if(error != NULL)
	{
		if(index != -1)
		{
			int sz = snprintf(NULL, 0, error, index);
			char *msg = (char*)malloc(sz * sizeof(char) + 1);
			if(msg != NULL)
			{
				snprintf(msg, sz + 1, error, index);
				writeToOutputFile(msg);
				free(msg);
			}
		}
		else
		{
			writeToOutputFile(error);
		}

	}
	if(*line != NULL)
	{
		free((*line));
		(*line) = NULL;
	}
	if((*file) != NULL)
	{
		fclose((*file));
		(*file) = NULL;
	}
	freeBeforeExit(&(*roadInfo), NULL);
	//exit is outside the if because we wanna terminate the program no matter what
	exit(EXIT_FAILURE);
}

/**
* Check if the program was called with a non-empty file
* @param file - the given file to check
*/
void checkIfFileEmpty(FILE** file)
{
	long initialLoc = ftell(*file); //save current location in file
	int isOk = 0;
	isOk = fseek(*file, 0, SEEK_END);
	if(isOk != 0) //Error while entering to the file
	{
		exitWithError(E_NO_FILE, -1, NULL, &(*file), NULL);
	}
	if (ftell(*file) == 0) //Check if the file has no lines
	{
		//File has no lines
		exitWithError(E_EMPTY_FILE, -1, NULL, &(*file), NULL);
	}

	isOk =  fseek(*file, initialLoc, SEEK_SET); //return to the initial location in the file
	if(isOk != 0)
	{
		//Error while entering to the file
		exitWithError(E_NO_FILE, -1, NULL, &(*file), NULL);
	}
}

/**
* Check if the program was called with a valid file-path
* @param filePath - the entered file path
* @param file - file pointer, setting it fot future usage
*/
void checkInputFile(const char* filePath, FILE** file)
{
	if(filePath == NULL || strlen(filePath) == 0)
	{
		//no filepath was received
		exitWithError(E_NO_FILE, -1, NULL, NULL, NULL);
	}
	*file = fopen(filePath, READ_AUTH);
	if(*file == NULL)
	{
		//error while trying to open the input file
		exitWithError(E_NO_FILE, -1, NULL,  NULL, NULL);
	}
	//Check if file is empty
	checkIfFileEmpty(&*file);
}
/**
* Check if the program was called with a valid input
* @param argc - number of input
* @param argv - an array of the given program's input
* @param file - file pointer, setting it fot future usage
* @return EXIT_SUCCESS if the operation was successful and the input is valid, otherwise
 * returning with EXIT_FAILURE
*/
void checkProgramInput(const int argc, char* argv[], FILE** file)
{
	const int LEGAL_PARM_NUM = 2;
	char* USAGE_ERROR = "Usage: RailWayPlanner <InputFile>";
	if (argc != LEGAL_PARM_NUM) //Checking if the input is only 1 parm as required
	{
		exitWithError(USAGE_ERROR, -1, NULL, NULL, NULL);
	}
	const char* filename = argv[1];
	checkInputFile(filename, &*file);
}
/**
* Check that the given param is indeed a number
* @param parm - the given parameter
* @return true if the param is ok, false otherwise
*/
bool checkIsInt(const char * parm)
{
	size_t length;
	length = strlen(parm);

	for (size_t i = 0; i < length; i++)
	{
		//If the name not contains only letters, space and '-' then return with error
		if (!isdigit((char)parm[i]) && parm[i] != '\n')
		{
			printf("%d", (int)parm[i]);
			return false;
		}
	}
	return true; //the name is ok
}

/**
* Check the given road length is a valid input, meaning it is a number >= 0
* @param input - the current line from the input file
* @param index - the current line in the file
* @param file - pointer to the input file
* @param roadInfo - structure storing the given road's info
*/
void checkTrackLen(char** input, const int index, FILE** file, RoadInfo** roadInfo)
{
	char* endPtr;
	int base = 10;
	long int length = strtol(*input, &endPtr, base);
	if(strlen(endPtr) > 1)
	{
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}
	else if(strlen(endPtr) == 1)
	{
		char endChar = (char)(endPtr[0]);
		if(endChar != '\n' && endChar != '\0')
		{
			exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
		}
	}
	if(length > -1)
	{
		(*roadInfo)->_length = length;
	}
	else
	{
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}

//	if(checkIsInt(*input) == false) //Check the 1 line is a number
//	{
////		sprintf(msg, E_IN_LINE,  index);
////		printf("%s", *input);
//		exitWithError(E_IN_LINE, index, &(*input), &(*file), NULL);
//	}
//	const int MIN_LEN = -1;
//	char* endPtr;
//	(*roadInfo)->_length = strtol(*input, &endPtr, 10);
//	int numOfParams = 0;
//	char endChar;
//	numOfParams = sscanf((*input), "%ld%c", &(*roadInfo)->_length, &endChar);
//	printf("%c\n%ld\n%d\n", endChar, (*roadInfo)->_length, numOfParams);
//	if(numOfParams != 2 || endChar != '\n' || endChar != '\0' || MIN_LEN > (*roadInfo)->_length)
		// Check if we got
		// all the parts
//	{
//		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
//	}
//	// Check the line is a number && length is indeed >= 0
//	if(endPtr != NULL && strlen(endPtr) == 1)
//	{
//		char endChar = endPtr[1];
//		if((*roadInfo)->_length < MIN_LEN || endChar == '\n')
//		{
//			exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
//		}
//	}
//	else
//	{
//		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
//	}
}

/**
* Check the given road length is a valid input, meaning it is a number >= 0
* @param input - the current line from the input file
* @param index - the current line in the file
* @param file - pointer to the input file
* @param roadInfo - structure storing the given road's info
*/
void checkNumOfTypes(char **input, const int index, FILE** file, RoadInfo** roadInfo)
{
	if(input == NULL || *input == NULL)
	{
		exitWithError(E_IN_LINE, index, NULL, &(*file), NULL);
	}
	char* endPtr;
	int base = 10;
	long int num = strtol(*input, &endPtr, base);
	if(strlen(endPtr) > 1)
	{
		exitWithError(E_IN_LINE, index, &(*input), &(*file), NULL);
	}
	else if(strlen(endPtr) == 1)
	{
		char endChar = (char)(endPtr[0]);
		if(endChar != '\n' && endChar != '\0')
		{
			exitWithError(E_IN_LINE, index, &(*input), &(*file), NULL);
		}
	}
	if(num > -1)
	{
		(*roadInfo)->_numberOfTypes = num;
	}
	else
	{
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}

//	char* msg = NULL;
//	const int MIN_LEN = 1;
//	if(checkIsInt(*input) == false) //Check the 1 line is a number
//	{
//////		sprintf(msg, E_IN_LINE,  index);
////		exitWithError(E_IN_LINE, index, &(*input), &(*file), NULL);
////	}
//	//Check if the number ot types is in the value range
////	int isOk = 0;
////	isOk = sscanf(input, "%ld", &((*roadInfo)->_numberOfTypes));
//	char* eptr;
//	(*roadInfo)->_numberOfTypes = strtol(*input, &eptr, 10);
//	if((*roadInfo)->_numberOfTypes == 0) // double check the line is a number
//	{
////		sprintf(msg, E_IN_LINE,  index);
//		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
//	}
//
//	if((*roadInfo)->_numberOfTypes < MIN_LEN ) // check given number ot types is indeed >= 1
//	{
////		sprintf(msg, E_IN_LINE,  index);
//		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
//	}
}
/**
* Save the road parts' types
* @param input - the current line from the input file
* @param file - pointer to the input file
* @param index - the current line in the file
* @param roadInfo - structure storing the given road's info
*/
void storeThePartsTypes(char** input, FILE** file, RoadInfo** roadInfo)
{
	const char* delimiter = ",";
	char *part = strtok(*input, delimiter); //split the given parts by ','
	int i = 0;
	//Define the types array
	(*roadInfo)->_types = (char *) malloc(sizeof(char) * (*roadInfo)->_numberOfTypes);
	//Check the memory allocation was successful
	if((*roadInfo)->_types == NULL)
	{
		exitWithError(NULL, -1, &(*input), &(*file), &(*roadInfo));
	}
	//split the given types into an array for future usage
	while(part != NULL && i < (*roadInfo)->_numberOfTypes)
	{
		(*roadInfo)->_types[i] = (char)part[0]; //enter the type into the array
		part = strtok(NULL, ","); // get the next type
		i++;
	}
}
void insertPartToArray(char** input, FILE** file, RoadInfo** roadInfo)
{
	//Define the parts
	if((*roadInfo)->_parts == NULL)
	{
		(*roadInfo)->_numberOfParts = 1;
		(*roadInfo)->_parts = (RoadPart *) malloc((*roadInfo)->_numberOfParts *sizeof(RoadPart));
	}
	else
	{
		(*roadInfo)->_numberOfParts++;
		(*roadInfo)->_parts = (RoadPart *)realloc((*roadInfo)->_parts,
												  (*roadInfo)->_numberOfParts * sizeof(RoadPart));
	}
	if((*roadInfo)->_parts == NULL) //Check the memory allocation was successful
	{
		exitWithError(NULL, -1, &(*input), &(*file), &(*roadInfo));
	}
}
/**
* Check that given number is greater than 0
* @param number - the number to check
* @return true if the number is valid, false otherwise
*/
bool isPositiveNum(const long int number)
{
	const int MIN_VAL = 1;
	if(number < MIN_VAL ) // check given num is indeed >= 1
	{
		return false;
	}
	return true;
}
/**
* Save the possible road parts
* @param input - the current line from the input file
* @param file - pointer to the input file
* @param index - the current line in the file
* @param roadInfo - structure storing the given road's info
*/
void checkPartProperties(const int index, char** input, FILE** file, RoadInfo** roadInfo)
{
	long int i = (*roadInfo)->_numberOfParts - 1; //get the index of latest part
	if(isPositiveNum((*roadInfo)->_parts[i]._partLength) == false)
	{
//		char* msg = NULL;
//		sprintf(msg, E_IN_LINE,  index);
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}
	if(isPositiveNum((*roadInfo)->_parts[i]._partPrice) == false)
	{
//		char* msg = NULL;
//		sprintf(msg, E_IN_LINE,  index);
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}
}
/**
* Save the possible road parts
* @param input - the current line from the input file
* @param file - pointer to the input file
* @param index - the current line in the file
* @param roadInfo - structure storing the given road's info
*/
void storeTheParts(char** input, const int index, FILE** file, RoadInfo** roadInfo)
{
	//Consts
	const int NUM_OF_PROPERTIES = 5;
	//Variables
	long int i = 0;
	if((*roadInfo)->_numberOfParts > 0)
	{
		i = (*roadInfo)->_numberOfParts;
	}

	//Create a new line in the parts array
	insertPartToArray(&(*input), &(*file), &(*roadInfo));

	//split the given part into it's 4 properties
	int numOfParams = 0;
	char endChar;
	numOfParams = sscanf(*input, "%c,%c,%ld,%ld%c", &(*roadInfo)->_parts[i]._leftType,
			&(*roadInfo)->_parts[i]._rightType, &(*roadInfo)->_parts[i]._partLength,
						 &(*roadInfo)->_parts[i]._partPrice, &endChar);
	if(numOfParams != NUM_OF_PROPERTIES || endChar != '\n') // Check if we got all the parts
	{
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}
	checkPartProperties(index, &(*input), &(*file), &(*roadInfo));
}

/**
* Process the input file and store the given info for future usage
* @param file - pointer to the input file
* @param roadInfo - structure storing the given road's info
*/
void processGivenRoadInfo(FILE** file, RoadInfo** roadInfo)
{
	char* line = NULL;
	size_t len = 0;
	int index = 1;

	//Loop through the input file lines
	while ((getline(&line, &len, *file)) != EOF)
	{
		switch(index)
		{
			case 1:
				//Road length
				checkTrackLen(&line, index, &(*file), &(*roadInfo));
				break;
			case 2:
				//Number of parts types
				checkNumOfTypes(&line, index, &(*file), &(*roadInfo));
				break;
			case 3:
				//Parts types
				storeThePartsTypes(&line, &(*file), &(*roadInfo));
				break;
			default:
				//Part
				storeTheParts(&line, index, &(*file), &(*roadInfo));
				break;
		}
		index++;
	}
	//Close the input file
	int isOk = 0;
	free(line);
	isOk = fclose(*file);
	if(isOk == EOF)
	{
		//Error while closing the input file
		exitWithError(NULL, -1, &line, &(*file), &(*roadInfo));
	}
	(*file) = NULL;
}

void initRoadsTable(long int** roadsTable, RoadInfo** roadInfo)
{
	for (long int l = 0; l <= (*roadInfo)->_length; l++)
	{
		for (long int t = 0; t < (*roadInfo)->_numberOfTypes; t++)
		{
			if(l == 0)
			{
				roadsTable[l][t] = 0;
			}
			else
			{
				roadsTable[l][t] = INT_MAX;
			}

		}
	}
}

void printArray(long int** roadsTable, RoadInfo** roadInfo)
{
	for (long int l = 0; l <= (*roadInfo)->_length; l++)
	{
		for (long int t = 0; t < (*roadInfo)->_numberOfTypes; t++)
		{
			long int price = roadsTable[l][t];
			printf("\t%ld", price);
		}
		printf("\n");
	}
	printf("\n");
}

long int getPartTypeIndex(RoadInfo** roadInfo, const char type)
{
	for (long int t = 0; t < (*roadInfo)->_numberOfTypes; t++)
	{
		if(type == (*roadInfo)->_types[t])
		{
			return t;
		}
	}
	return -1;
}

void updateRoadsTable(long int** roadsTable, const long int l, const RoadPart* part,
		RoadInfo** roadInfo)
{
	long int currentPrice = (*part)._partPrice;
	//Check the road is even possible
	long int dis = l - (*part)._partLength;
	if(dis > 0)
	{
		long int leftIndex = getPartTypeIndex(&(*roadInfo), part->_leftType);
		if(leftIndex == -1)
		{
			freeRoadsTable(&roadsTable, &(*roadInfo));
			exitWithError(NULL, -1, NULL, NULL, &(*roadInfo));
		}

		if(roadsTable[dis][leftIndex] == INT_MAX)
		{
			return; //This road isn't possible so we don't need to update
		}

		currentPrice += roadsTable[dis][leftIndex]; //add price until now
	}

	long int rightIndex = getPartTypeIndex(&(*roadInfo), part->_rightType);
	if(rightIndex == -1)
	{
		freeRoadsTable(&roadsTable, &(*roadInfo));
		exitWithError(NULL, -1, NULL, NULL, &(*roadInfo));
	}
	long int curPrice = roadsTable[l][rightIndex];
	if(currentPrice < curPrice)
	{
		roadsTable[l][rightIndex] = currentPrice;
	}
}

long int** createRoadsTable(RoadInfo** roadInfo)
{
	long int** roadsTable = (long**) malloc(sizeof(long int*) * (*roadInfo)->_length + 1);
	if(roadsTable == NULL)
	{
		freeRoadsTable(&roadsTable, &(*roadInfo));
		exitWithError(NULL, -1, NULL, NULL, &(*roadInfo));
	}

	for (long int l = 0; l <= (*roadInfo)->_length; l++)
	{
		roadsTable[l] = (long int*)	malloc(sizeof(long int) * (*roadInfo)->_numberOfTypes);
		if(roadsTable[l] == NULL)
		{
			freeRoadsTable(&roadsTable, &(*roadInfo));
			exitWithError(NULL, -1, NULL, NULL, &(*roadInfo));
		}
	}

	initRoadsTable(roadsTable, &(*roadInfo));
	return roadsTable;
}

void createSuccessMsg(const long int price)
{
	const char* success = "The minimal price is: %ld";
	int sz = snprintf(NULL, 0, success, price);
	char* msg = (char*)malloc(sz * sizeof(char) + 1);
	if(msg != NULL)
	{
		snprintf(msg, sz + 1, success, price);
		writeToOutputFile(msg);
		free(msg);
		msg = NULL;
	}
}

void getTheMinCost(long int** roadsTable, RoadInfo** roadInfo)
{
	long int l = (*roadInfo)->_length;
	long int minCost = INT_MAX;
	for (long int t = 0; t < (*roadInfo)->_numberOfTypes; t++)
	{
		if(roadsTable[l][t] < minCost)
		{
			minCost = roadsTable[l][t];
		}
	}
	printArray(roadsTable, roadInfo);
	if(minCost == INT_MAX) //Check if no road was found
	{
		minCost = -1;
	}
	createSuccessMsg(minCost);
	freeBeforeExit(&(*roadInfo), &roadsTable);
}

/**
* Process the input file and store the given info for future usage
* @param roadInfo - structure storing the given road's info
*/
void calcMinCostRoad(RoadInfo** roadInfo)
{
	if(*roadInfo == NULL)
	{
		exitWithError(NULL, -1, NULL, NULL, &(*roadInfo));
	}

	if((*roadInfo)->_length == 0)
	{
		createSuccessMsg(0);
	}

	long int** roadsTable = createRoadsTable(&(*roadInfo));

	for (long int l = 1; l <= (*roadInfo)->_length; l++)
	{
		for (long int p = 0; p < (*roadInfo)->_numberOfParts; p++)
		{
			if((*roadInfo)->_parts[p]._partLength == l)
			{
				updateRoadsTable(roadsTable, l,
						&(*roadInfo)->_parts[p], &(*roadInfo));
			}
			else if ((*roadInfo)->_parts[p]._partLength < l)
			{
				updateRoadsTable(roadsTable, l, &(*roadInfo)->_parts[p], &(*roadInfo));
			}
		}
	}

	getTheMinCost(roadsTable, &(*roadInfo));
}

int main(int argc, char* argv[])
{
	//File
	FILE* file = NULL;
	checkProgramInput(argc, argv, &file);
	if(file == NULL) //Make sure we have a valid pointer to the input file
	{
		exitWithError(E_NO_FILE, -1, NULL, NULL, NULL);
	}
	RoadInfo info = {0,0,NULL,0,NULL};
	RoadInfo* roadInfo = &info;
	processGivenRoadInfo(&file, &roadInfo);
	calcMinCostRoad(&roadInfo);
	return EXIT_SUCCESS;
}
