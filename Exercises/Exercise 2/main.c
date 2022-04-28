/**
 * Ex2 - Bar Melinarskiy - 318189982
 * Exercise 2 - calc the min price of the given road
 * @return success code - returning 0 if the program finished successfully
 */
#define  _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

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
*/
void writeToOutputFile(const char* output)
{
	const char* FILE_PATH = OUTPUT_FILE_NAME; //ref to the output file path
	int isOk = 0;
	if(output == NULL) //check that the output msg is not null
	{
		return;
	}
	FILE *file = fopen(FILE_PATH, WRITE_AUTH); // try to open the output file
	if (file != NULL)
	{
		isOk = fputs(output, file); // try to write to the file
		if(isOk == EOF)
		{
			return; //an error occurred while trying to write to the output file
		}
		isOk = fclose(file);
		if(isOk == EOF)
		{
			//Error while closing the output file
			return;
		}
	}
	else //We couldn't open the output file
	{
		return;
	}
}
/**
* Free dynamic memory of the road's table
* @param roadsTable - 2D table of the possible roads and their min prices
* @param roadInfo - structure storing the given road's info
*/
void freeRoadsTable(long int*** roadsTable, RoadInfo** roadInfo)
{
	//looping through all the rows and freeing each requested memory
	for (long int l = 0; l <= (*roadInfo)->_length; l++)
	{
		free((*roadsTable)[l]);
		(*roadsTable)[l] = NULL;
	}
	free(*roadsTable); //free the cols
	*roadsTable = NULL;
}
/**
* Free dynamic memory of the road's table & road's info
* @param roadsTable - 2D table of the possible roads and their min prices
* @param roadInfo - structure storing the given road's info
*/
void freeBeforeExit(RoadInfo** roadInfo, long int*** table)
{
	if(table != NULL && *table != NULL) //check if we need to free the table
	{
		freeRoadsTable(&(*table), roadInfo);
	}

	if((*roadInfo) != NULL) ///check if we need to free the struc
	{
		if((*roadInfo)->_types != NULL) //free the types array
		{
			free((*roadInfo)->_types);
			(*roadInfo)->_types = NULL;
		}
		if((*roadInfo)->_parts != NULL) //free the parts array
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
* @param index - the index of the line in the input-file, value is -1 if not related to the file
* @param line - the line in the input-file
* @param file - the given file to close
* @param roadInfo - structure storing the given road's info
*/
void exitWithError(const char* error, const long int index, char** line, FILE** file,
		RoadInfo** roadInfo)
{
	if(error != NULL) //check if there is an error to write out
	{
		if(index != -1) //Check if we need to concatenate the error msg with the index
		{
			int sz = snprintf(NULL, 0, error, index); //get the msg size
			char *msg = (char*)malloc(sz * sizeof(char) + 1); //ask for space for the msg
			if(msg != NULL) // check we got the requested space
			{
				snprintf(msg, sz + 1, error, index); //concatenate the error with the index
				writeToOutputFile(msg);
				free(msg); // free the requested space after the msg was writen
			}
		}
		else
		{
			writeToOutputFile(error); //write the msg has is
		}

	}
	if(*line != NULL) //check if we need to free the line ref from the getline function
	{
		free((*line));
		(*line) = NULL;
	}
	if((*file) != NULL) // check if we need to close the file
	{
		fclose((*file));
		(*file) = NULL;
	}
	freeBeforeExit(&(*roadInfo), NULL); //free the road's info struc
	//terminate the program no matter what
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
* Check the given road length is a valid input, meaning it is a number >= 0
* @param input - the current line from the input file
* @param index - the current line in the file
* @param file - pointer to the input file
* @param roadInfo - structure storing the given road's info
*/
void checkTrackLen(char** input, const int index, FILE** file, RoadInfo** roadInfo)
{
	//Check that the line is indeed a long int
	char* endPtr;
	int base = 10;
	long int length = strtol(*input, &endPtr, base);
	if(strlen(endPtr) > 1) //check what is the length of the rest of the line
	{
		//we had more than just a end of line char so this is not valid
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}
	else if(strlen(endPtr) == 1)
	{
		//check if we only have an end of a line char
		char endChar = (char)(endPtr[0]);
		if(endChar != '\n' && endChar != '\0')
		{
			exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
		}
	}
	//Check the range of the number
	const int MIN_VALUE = -1;
	if(length > MIN_VALUE)
	{
		(*roadInfo)->_length = length;
	}
	else
	{
		//value is out of range
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}
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
	//Check that the line is indeed a long int
	char* endPtr;
	int base = 10;
	long int num = strtol(*input, &endPtr, base);
	if(strlen(endPtr) > 1) //check what is the length of the rest of the line
	{
		//we had more than just a end of line char so this is not valid
		exitWithError(E_IN_LINE, index, &(*input), &(*file), NULL);
	}
	else if(strlen(endPtr) == 1)
	{
		//check if we only have an end of a line char
		char endChar = (char)(endPtr[0]);
		if(endChar != '\n' && endChar != '\0')
		{
			exitWithError(E_IN_LINE, index, &(*input), &(*file), NULL);
		}
	}
	//Check the range of the number
	const int MIN_VALUE = 0;
	if(num > MIN_VALUE)
	{
		(*roadInfo)->_numberOfTypes = num;
	}
	else
	{
		//value is out of rang
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}
}
/**
* Allocate dynamic memory for the array of  the road parts' types & Save the road parts' types
* @param input - the current line from the input file
* @param file - pointer to the input file
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
/**
* Allocate dynamic memory for the array of  the road parts
* @param input - the current line from the input file
* @param file - pointer to the input file
* @param roadInfo - structure storing the given road's info
*/
void insertPartToArray(char** input, FILE** file, RoadInfo** roadInfo)
{
	//Define the parts array
	if((*roadInfo)->_parts == NULL) // check if the array already exist
	{
		(*roadInfo)->_numberOfParts = 1;
		(*roadInfo)->_parts = (RoadPart *) malloc((*roadInfo)->_numberOfParts *sizeof(RoadPart));
	}
	else
	{
		// array already exist so ask for more memory
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
* Get the index in the types array of connection type
* @param roadInfo - structure storing the given road's info
* @param type - the connection type
* @return the index in the type's array, if not found return -1
*/
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
	//Check that the part's length is > 0
	if(isPositiveNum((*roadInfo)->_parts[i]._partLength) == false)
	{
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}
	//Check that the part's price is > 0
	if(isPositiveNum((*roadInfo)->_parts[i]._partPrice) == false)
	{
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}
	//Make sure both connection are valid
	const int NOT_FOUND = -1;
	if(getPartTypeIndex(&(*roadInfo), (*roadInfo)->_parts[i]._rightType) == NOT_FOUND)
	{
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}

	if(getPartTypeIndex(&(*roadInfo), (*roadInfo)->_parts[i]._leftType) == NOT_FOUND)
	{
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
	const int NUM_OF_PROPERTIES = 5;
	long int i = 0;
	//set the part's counter
	if((*roadInfo)->_numberOfParts > 0)
	{
		i = (*roadInfo)->_numberOfParts;
	}

	//Create a new line in the parts array
	insertPartToArray(&(*input), &(*file), &(*roadInfo));

	//split the given part into it's 4 properties, check ending of the line
	int numOfParams = 0;
	char endChar;
	numOfParams = sscanf(*input, "%c,%c,%ld,%ld%c", &(*roadInfo)->_parts[i]._leftType,
			&(*roadInfo)->_parts[i]._rightType, &(*roadInfo)->_parts[i]._partLength,
						 &(*roadInfo)->_parts[i]._partPrice, &endChar);
	if(numOfParams != NUM_OF_PROPERTIES || endChar != '\n') // Check if we got all the parts
	{
		//the line's format was not valid
		exitWithError(E_IN_LINE, index, &(*input), &(*file), &(*roadInfo));
	}
	//check all the given properties of the current part
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
/**
* Initialize the table's value
* @param roadsTable - 2D table of the possible roads and their min prices
* @param roadInfo - structure storing the given road's info
*/
void initRoadsTable(long int** roadsTable, RoadInfo** roadInfo)
{
	for (long int l = 0; l <= (*roadInfo)->_length; l++)
	{
		for (long int t = 0; t < (*roadInfo)->_numberOfTypes; t++)
		{
			// if the road's length is 0 then the price is 0
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
/**
* Update a cell in the table
* @param roadsTable - 2D table of the possible roads and their min prices
* @param l - current road length
* @param part - current part to check if it can fit in the road
* @param roadInfo - structure storing the given road's info
*/
void updateRoadsTable(long int** roadsTable, const long int l, const RoadPart* part,
		RoadInfo** roadInfo)
{
	long int currentPrice = (*part)._partPrice;
	//Check the road is even possible
	long int dis = l - (*part)._partLength;
	if(dis > 0)
	{
		long int leftIndex = getPartTypeIndex(&(*roadInfo), part->_leftType);
		if(leftIndex == -1) // make sure we got a valid index
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
	if(rightIndex == -1) // make sure we got a valid index
	{
		freeRoadsTable(&roadsTable, &(*roadInfo));
		exitWithError(NULL, -1, NULL, NULL, &(*roadInfo));
	}
	long int curPrice = roadsTable[l][rightIndex];
	if(currentPrice < curPrice) // check if the current price is lower than the current value
	{
		roadsTable[l][rightIndex] = currentPrice;
	}
}
/**
* Allocate dynamic memory for the 2D table of the possible roads and their min prices
* @param roadInfo - structure storing the given road's info
*/
long int** createRoadsTable(RoadInfo** roadInfo)
{
	//get memory for each of the possible lengths
	long int** roadsTable = (long**) malloc(sizeof(long int*) * (*roadInfo)->_length + 1);
	if(roadsTable == NULL) //check if the request was successful
	{
		freeRoadsTable(&roadsTable, &(*roadInfo));
		exitWithError(NULL, -1, NULL, NULL, &(*roadInfo));
	}

	for (long int l = 0; l <= (*roadInfo)->_length; l++)
	{
		//get memory for each of the possible type
		roadsTable[l] = (long int*)	malloc(sizeof(long int) * (*roadInfo)->_numberOfTypes);
		if(roadsTable[l] == NULL) //check if the request was successful
		{
			freeRoadsTable(&roadsTable, &(*roadInfo));
			exitWithError(NULL, -1, NULL, NULL, &(*roadInfo));
		}
	}
	//init the cells values
	initRoadsTable(roadsTable, &(*roadInfo));
	return roadsTable;
}
/**
* Print the success msg to the output file
* @param price - the min price to print
*/
void createSuccessMsg(const long int price)
{
	const char* success = "The minimal price is: %ld";
	//concatenate the msg with the price
	int sz = snprintf(NULL, 0, success, price);
	char* msg = (char*)malloc(sz * sizeof(char) + 1);
	if(msg != NULL)
	{
		snprintf(msg, sz + 1, success, price);
		writeToOutputFile(msg);
		free(msg);
	}
}
/**
* Find the road's min cost, after the all table was calculated
* @param roadsTable - 2D table of the possible roads and their min prices
* @param roadInfo - structure storing the given road's info
*/
void getTheMinCost(long int** roadsTable, RoadInfo** roadInfo)
{
	long int l = (*roadInfo)->_length;
	long int minCost = INT_MAX;
	//Loop at the line with the road's length and get the min price
	for (long int t = 0; t < (*roadInfo)->_numberOfTypes; t++)
	{
		if(roadsTable[l][t] < minCost)
		{
			minCost = roadsTable[l][t];
		}
	}

	if(minCost == INT_MAX) //Check if no road was found
	{
		minCost = -1;
	}
	createSuccessMsg(minCost); //print to the output file
	freeBeforeExit(&(*roadInfo), &roadsTable); //free the dynamic memory
}

/**
* Calc the road's table
* @param roadInfo - structure storing the given road's info
*/
void calcMinCostRoad(RoadInfo** roadInfo)
{
	if(*roadInfo == NULL)
	{
		exitWithError(NULL, -1, NULL, NULL, &(*roadInfo));
	}
	//If the road's length is 0 -> price is also 0 because no parts are needed
	if((*roadInfo)->_length == 0)
	{
		createSuccessMsg(0);
	}

	long int** roadsTable = createRoadsTable(&(*roadInfo)); //create the table

	for (long int l = 1; l <= (*roadInfo)->_length; l++) // loop at each length
	{
		for (long int p = 0; p < (*roadInfo)->_numberOfParts; p++) // loop at each part
		{
			//If the part's length is <= then the current length then check if it helps the road
			if((*roadInfo)->_parts[p]._partLength <= l)
			{
				updateRoadsTable(roadsTable, l,
						&(*roadInfo)->_parts[p], &(*roadInfo));
			}
		}
	}

	getTheMinCost(roadsTable, &(*roadInfo)); //get the min value after the table is finished
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
	RoadInfo info = {0, 0, NULL, 0, NULL};
	RoadInfo* roadInfo = &info;
	processGivenRoadInfo(&file, &roadInfo);
	calcMinCostRoad(&roadInfo);
	return EXIT_SUCCESS;
}
