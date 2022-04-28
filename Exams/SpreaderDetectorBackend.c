#define  _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "SpreaderDetectorParams.h"

#define ARGS_COUNT 3
#define  EXIT_FAILURE 1
#define  EXIT_SUCCESS 0
#define WRITE_AUTH "w"
#define READ_AUTH "r"
#define EMPTY 0

#define  MAX_RISK 1;

#define MEETINGS_FILE_NAME  "Meetings.in"
#define PEOPLE_FILE_NAME "People.in"
#define USAGE_MSG "Usage: ./SpreaderDetectorBackend <Path to People.in> <Path to Meetings.in>\n"
#define INPUT_FILE_MSG "Error in input files.\n"
#define OUTPUT_FILE_MSG "Error in output file.\n"
#define NO_INFO ""
#define END_LINE '\n'

/**
 * Id of a person
 */
typedef unsigned long long int ID;

/**
 * Index
 */
typedef unsigned long int Index;

/**
 * A type def of boolean global type
 */
typedef enum
{
	FALSE = 0,
	TRUE
} Boolean;

/**
 * A type def of boolean global type
 */
typedef enum
{
	Hospital = 0,
	Quarantine,
	Clean
} RiskLevel;

/**
 * element in the array for storing persons information.
 */
typedef struct Person
{
	ID id;
	float age;
	char* name;
	Boolean meetSomeone;
} Person;
/**
 *  A data structure for storing persons information in a dynamic array.
 */
typedef struct PeopleArray
{
	unsigned long int counter;
	Person *people;
} PeopleArray;

/**
 *A data structure for storing all Covid-19 risks diagnostic.
 */
typedef struct Diagnosis
{
	Person* personInfo;
	float risk;
	RiskLevel riskLevel;

} Diagnosis;

/**
* A data structure for storing node in the heap storing each person covid-19 risk.
*/
typedef struct HeapCovidDiagnosis
{
	unsigned long int count;
	Diagnosis *heap;
} HeapCovidDiagnosis;

/**
* A data structure for storing a meeting's info btw 2 people
*/
typedef struct MeetingInfo
{
	ID sourceId;
	ID targetId;
	float distance;
	float time;
} MeetingInfo;

/**
 * Prints program usage error message to stderr.
 */
void usage()
{
	fprintf(stderr,  USAGE_MSG);
	 exit(EXIT_FAILURE);
}
/**
 * Prints program input-file error message to stderr.
 */
void errorInInputFile()
{
	fprintf(stderr,  INPUT_FILE_MSG);
	exit(EXIT_FAILURE);
}
/**
 * Prints program output-file error message to stderr.
 */
void errorInOutputFile()
{
	fprintf(stderr,  OUTPUT_FILE_MSG);
	exit(EXIT_FAILURE);
}
/**
 * Prints program output-file error message to stderr.
 */
void errorInStandardLib()
{
	fprintf(stderr,  STANDARD_LIB_ERR_MSG);
	exit(EXIT_FAILURE);
}
/**
* free the dynamic memory of the people's array
* @param peopleArray - array of people to fill with given info in file
*/
void freeThePeople(PeopleArray* peopleArray)
{
	free(peopleArray->people);
	peopleArray->people = NULL;
	peopleArray->counter = 0;
	peopleArray = NULL;
}
/**
* free the dynamic memory of the people's array
* @param peopleArray - array of people to fill with given info in file
*/
void freeTheDiagnosticHeap(HeapCovidDiagnosis* diagnosis)
{
	free(diagnosis->heap);
	diagnosis->heap = NULL;
	diagnosis = NULL;
}
/**
 * Check input file path is valid.
 * @param path - A string with user input for the file path.
 * @param suffix - the file correct name.
 * @return 1 when the path ends with the suffix, 0 otherwise.
 */
int checkFilePath(const char *path, const char *suffix)
{
	size_t stringLen = strlen(path);
	size_t suffixLen = strlen(suffix);

	return suffixLen <= stringLen && !strcmp(path + stringLen - suffixLen, suffix);
}

/**
 * Write the full Covid-19 diagnostic info for each person.
 * @param reportInfo - the diagnostic info to write to the output file
 */
void exportDiagnosticInfo(const char reportInfo[])
{
	FILE *outputFile = NULL;
	outputFile = fopen(OUTPUT_FILE, WRITE_AUTH);
	if (outputFile == NULL) //error while trying to create the output file
	{
		errorInOutputFile();
	}
	fprintf(outputFile, "%s", reportInfo);
	fclose(outputFile);
}
/**
* Compare function for qsort of people array
* @param person1 - first person info
* @param person2 - second person info
* @param positive number if the first person id is bigger, negative if it's smaller
*/
int compare (const void * person1, const void * person2)
{

	Person *p1 = (Person *) person1;
	Person *p2 = (Person *) person2;

	return ( p1->id - p2->id );
}
/**
* Get the person info by given id
* @param peopleArray - array of people to fill with given info in file
* @param id - the id of the person we want to find
* @return the person info, NULL if doesn't exist
*/
Person* getPerson(PeopleArray* peopleArray, const ID id)
{
	for (Index i = 0; i < peopleArray->counter; i++)
	{
		if(id == peopleArray->people[i].id)
		{
			return &peopleArray->people[i];
		}
	}
	return NULL;
}
/**
* Get the person info by given id
* @param diagnosis - heap fill with the each person's diagnostic
* @param id - the id of the person we want to find
* @return the person's diagnosis info, NULL if doesn't exist
*/
Diagnosis* getDiagnosis(HeapCovidDiagnosis* diagnosis, const ID id)
{
	for (Index i = 0; i < diagnosis->count; i++)
	{
		if(id == diagnosis->heap[i].personInfo->id)
		{
			return &diagnosis->heap[i];
		}
	}
	return NULL;
}
/**
* Check the given road length is a valid input, meaning it is a number >= 0
* @param input - the current line from the input file
* @param index - the current line number in the file
* @param file - the input file
* @param peopleArray - array of people to fill with given info in file
*/
void insertNewPerson(char** input, Index index, FILE** file, PeopleArray* peopleArray)
{
	const int NUM_OF_ARGS = 4;
	char endChar = END_LINE;
	peopleArray->counter++; //update counter of people

	if(peopleArray->people == NULL) //check if this is the first time
	{
		//ask for memory for the array
		peopleArray->people = (Person *) malloc(peopleArray->counter * sizeof(Person));
		if(peopleArray->people == NULL)
		{
			//Something went wrong, exit the program and free dynamic memory
			freeThePeople(peopleArray);
			fclose(*file);
			errorInStandardLib();
		}
	}
	else
	{
		//ask for more memory for the array
		peopleArray->people = (Person *) realloc(peopleArray->people,
				                                 peopleArray->counter * sizeof(Person));
		if(peopleArray->people == NULL)
		{
			//Something went wrong, exit the program and free dynamic memory
			freeThePeople(peopleArray);
			fclose(*file);
			errorInStandardLib();
		}
	}
	//Split the line info into the parameters
	Person p;
	int result = sscanf
			(
					*input, "%s %llu %f%c",
					p.name,
					&p.id,
					&p.age,
					&endChar
			);
	if (result != NUM_OF_ARGS || endChar != END_LINE) //check the parsing was successful
	{
		//Something went wrong, exit the program and free dynamic memory
		freeThePeople(peopleArray);
		fclose(*file);
		errorInInputFile();
	}

	peopleArray->people[index] = p;
}

/**
 * retrieve the people's info from the given file
 * @param path - A string with user input for the file path.
 * @param peopleArray - array of people to fill with given info in file
 */
void retrievePeopleInfo(const char *path, PeopleArray* peopleArray)
{
	if(path == NULL || strlen(path) == 0)
	{
		errorInInputFile(); //no file path was received
	}

	FILE *file = fopen(path, READ_AUTH);
	if(file == NULL)
	{
		errorInInputFile(); //error while trying to open the input file
	}

	//Loop through the input file lines
	char* line = NULL;
	size_t len = 0;
	Index index = 0;

	while ((getline(&line, &len, file)) != EOF)
	{
		insertNewPerson(&line, index, &file, peopleArray);
		index++;
	}
	//Close the input file
	int isOk = 0;
	free(line);
	isOk = fclose(file);
	if(isOk == EOF)
	{
		//Error while closing the input file
		file = NULL;
		errorInInputFile();
	}

	Boolean isEmpty = peopleArray->counter == EMPTY;
	if(isEmpty == TRUE) //If the file is empty then create empty output file and finish program
	{
		char* info = NO_INFO;
		exportDiagnosticInfo(info);
		exit(EXIT_SUCCESS);
	}
	//Sort the array by the id
//	qsort(peopleArray->people, peopleArray->counter, sizeof(Person), compare);
}
/**
* Calc the risk by the given formula
* @param meeting - the meeting's info
* @return the risk
*/
float calcCrnaRisk(const MeetingInfo meeting)
{
	return (meeting.time * MIN_DISTANCE) / (meeting.distance * MAX_TIME);
}
/**
* Switch btw two elements inside the heap
* @param index - index of the current node
* @return the index of the parent node
*/
void swap(Diagnosis *d1, Diagnosis *d2)
{
	Diagnosis temp = *d1;
	*d1 = *d2;
	*d2 = temp;
}
/**
* Get index of parent node of current index
* @param index - index of the current node
* @return the index of the parent node
*/
Index heapGetLeft(Index index)
{
	return (2 * index) + 1;
}
/**
* Get index of parent node of current index
* @param index - index of the current node
* @return the index of the parent node
*/
Index heapGetRight(Index index)
{
	return (2 * index) + 2;
}
/**
* Get index of parent node of current index
* @param index - index of the current node
* @return the index of the parent node
*/
Index heapGetParent(Index index)
{
	return (index - 1) / 2;
}
/**
* Fix heap order to keep the correct order
* @param diagnosis - heap with each person's diagnostic
* @param index - index of the person to start from
*/
void maxHeapify(HeapCovidDiagnosis* diagnosis, const Index index)
{
	//Check if the heap os empty
	if(diagnosis == NULL || diagnosis->count == EMPTY)
	{
		return;
	}

	Index largestIndex;

	Index leftIndex = heapGetLeft(index);
	Index rightIndex = heapGetRight(index);
	if(leftIndex <= (diagnosis->count - 1)
	   && diagnosis->heap[leftIndex].risk > diagnosis->heap[index].risk)
	{
		largestIndex = leftIndex;
	}
	else
	{
		largestIndex = index;
	}

	if(rightIndex <= (diagnosis->count - 1)
	   && diagnosis->heap[leftIndex].risk > diagnosis->heap[largestIndex].risk)
	{
		largestIndex = rightIndex;
	}

	if(largestIndex != index)
	{
		swap(&diagnosis->heap[index], &diagnosis->heap[largestIndex]);
		maxHeapify(&(*diagnosis), largestIndex);
	}
}
/**
* Change risk for given person and then fix heap order
* @param diagnosis - heap with each person's diagnostic
* @return the person's diagnosis with the highest risk
*/
Diagnosis* heapExtractMax(HeapCovidDiagnosis* diagnosis)
{
	//Check if the heap os empty
	if(diagnosis == NULL || diagnosis->count == EMPTY)
	{
		return NULL;
	}

	Diagnosis* max = &diagnosis->heap[0];
	Index lastIndex = diagnosis->count - 1;
	diagnosis->heap[0] = diagnosis->heap[lastIndex];
	diagnosis->count--;
	maxHeapify(diagnosis, 0);
	return max;
}
/**
* Change risk for given person and then fix heap order
* @param diagnosis - heap with each person's diagnostic
* @param index - index of the person to change
* @param risk - risk to change to
*/
void heapIncreaseKey(HeapCovidDiagnosis* diagnosis, const Index index, const float risk)
{
	Diagnosis* currentNode = &diagnosis->heap[index];
	(*currentNode).risk = risk;
	Index parentIndex = heapGetParent(index);
	Diagnosis* parent = &diagnosis->heap[parentIndex];
	Index i = index;
	while(i > 0 && (*parent).risk < (*currentNode).risk)
	{
		swap(currentNode, parent);
		i = parentIndex;
		parentIndex = heapGetParent(index);
		parent = &diagnosis->heap[parentIndex];
	}
}
/**
* Insert new element to the heap
* @param diagnosis - heap to fill with the each person's diagnostic
* @param personInfo - the person exposed to the someone
* @param meeting - the meeting's info
* @return TRUE on success, FALSE otherwise
*/
int insertToHeap(HeapCovidDiagnosis* diagnosis, Person* personInfo, const MeetingInfo meeting)
{
	diagnosis->count++;
	Index index = diagnosis->count - 1;
	if(diagnosis->heap == NULL) //check if this is the first time
	{
		//ask for memory for the heap
		diagnosis->heap = (Diagnosis *) malloc(diagnosis->count * sizeof(Diagnosis));
		if(diagnosis->heap == NULL)
		{
			return FALSE;
		}
		diagnosis->heap[index].risk = MAX_RISK;
		diagnosis->heap[index].riskLevel = Hospital;
		diagnosis->heap[index].personInfo = personInfo;
	}
	else
	{
		//ask for more memory for the heap
		diagnosis->heap = (Diagnosis *) realloc(diagnosis->heap,
												diagnosis->count * sizeof(Diagnosis));
		if(diagnosis->heap == NULL)
		{
			return FALSE;
		}
		Diagnosis* sourceDiagnosis = getDiagnosis(diagnosis, meeting.sourceId);
		if(sourceDiagnosis == NULL)
		{
			return FALSE;
		}
		float risk = sourceDiagnosis->risk * calcCrnaRisk(meeting);
		diagnosis->heap[index].risk = (-1) * INFINITY;
		personInfo->meetSomeone = TRUE;
		diagnosis->heap[index].personInfo = personInfo;
		diagnosis->heap[index].riskLevel = Clean;
		if(risk >= MEDICAL_SUPERVISION_THRESHOLD)
		{
			diagnosis->heap[index].riskLevel = Hospital;
		}
		else if(risk >= REGULAR_QUARANTINE_THRESHOLD)
		{
			diagnosis->heap[index].riskLevel = Quarantine;
		}
		heapIncreaseKey(&(*diagnosis), index, risk);
	}

	return TRUE;
}

/**
* Check the given road length is a valid input, meaning it is a number >= 0
* @param input - the current line from the input file
* @param index - the current line number in the file
* @param file - the input file
* @param peopleArray - array of people to fill with given info in file
* @param diagnosis - heap to fill with the each person's diagnostic
*/
void calcRisk(char** input, const Index index, FILE** file, PeopleArray* peopleArray,
			  HeapCovidDiagnosis* diagnosis)
{
	const Index FIRST = 1;
	const int ONLY_ID_ARGS_COUNTER = 2;
	const int MEETING_ARGS_COUNTER = 5;
	MeetingInfo meeting = {0, 0, 0, 0};
	char endChar = END_LINE;
	if(index == FIRST)
	{
		int result = sscanf(*input, "%llu%c",
				        &meeting.targetId,
						&endChar
				);

		if (result != ONLY_ID_ARGS_COUNTER || endChar != END_LINE) //check if parsing was successful
		{
			//Something went wrong, exit the program and free dynamic memory
			freeThePeople(peopleArray);
			freeTheDiagnosticHeap(diagnosis);
			fclose(*file);
			errorInInputFile();
		}
	}
	else
	{
		int result = sscanf(*input, "%llu %llu %f %f%c",
							&meeting.sourceId,
							&meeting.targetId,
							&meeting.distance,
							&meeting.time,
							&endChar
						   );

		if (result != MEETING_ARGS_COUNTER || endChar != END_LINE) //check if parsing was successful
		{
			//Something went wrong, exit the program and free dynamic memory
			freeThePeople(peopleArray);
			freeTheDiagnosticHeap(diagnosis);
			fclose(*file);
			errorInInputFile();
		}
	}
	Person* personInfo = getPerson(peopleArray, meeting.targetId);
	if(personInfo == NULL)
	{
		//Something went wrong, exit the program and free dynamic memory
		freeThePeople(&(*peopleArray));
		freeTheDiagnosticHeap(diagnosis);
		fclose(*file);
		errorInInputFile();
	}
	int isOk = insertToHeap(diagnosis, personInfo, meeting);
	if(isOk == FALSE)
	{
		//Something went wrong, exit the program and free dynamic memory
		freeThePeople(peopleArray);
		freeTheDiagnosticHeap(diagnosis);
		fclose(*file);
		errorInStandardLib();
	}
}

/**
 * retrieve the meeting's info from the given file & calc risk level for each person
 * @param path - A string with user input for the file path.
 * @param peopleArray - array of people  info
 * @param diagnosis - heap to fill with the each person's diagnostic
 */
void calcRiskForEachPerson(const char *path, PeopleArray* peopleArray,
						   HeapCovidDiagnosis* diagnosis)
{
	if(path == NULL || strlen(path) == 0)
	{
		errorInInputFile(); //no file path was received
	}

	FILE *file = fopen(path, READ_AUTH);
	if(file == NULL)
	{
		errorInInputFile(); //error while trying to open the input file
	}

	//Loop through the input file lines
	char* line = NULL;
	size_t len = 0;
	unsigned long index = 1;

	while ((getline(&line, &len, file)) != EOF)
	{
		calcRisk(&line, index, &file, peopleArray, diagnosis);
		index++;
	}
	//Close the input file
	int isOk = 0;
	free(line);
	isOk = fclose(file);
	if(isOk == EOF)
	{
		//Error while closing the input file
		file = NULL;
		errorInInputFile();
	}
}
/**
 * Create msg to export to output file
 * @param diagnosis - heap with the each person's diagnostic
 * @return 1 if the operation was successful and the input is valid, otherwise  returning with 0
 */
Boolean createReportMsg(char* name, ID id, RiskLevel riskLevel)
{
	char* msgTemplate = CLEAN_MSG;
	if(riskLevel == Hospital)
	{
		msgTemplate = MEDICAL_SUPERVISION_THRESHOLD_MSG;
	}
	else if(riskLevel == Quarantine)
	{
		msgTemplate = REGULAR_QUARANTINE_MSG;
	}

	int sz = snprintf(NULL, 0, msgTemplate, name, id);
	char* msg = (char*)malloc(sz * sizeof(char) + 1);
	if(msg != NULL)
	{
		snprintf(msg, sz + 1, msgTemplate, name, id);
		exportDiagnosticInfo(msg);
		free(msg);
		msg = NULL;
		return TRUE;
	}

	return FALSE;
}

/**
 * retrieve the meeting's info from the given file & calc risk level for each person
 * @param diagnosis - heap with the each person's diagnostic
 * @return 1 if the operation was successful and the input is valid, otherwise  returning with 0
 */
void alertEachPerson(HeapCovidDiagnosis* diagnosis, PeopleArray* peopleArray)
{
	Boolean isOk = TRUE;
	for (Index i = 0; i < diagnosis->count; i++)
	{
		Diagnosis* max = heapExtractMax(diagnosis);
		Person* p = max->personInfo;
		isOk = createReportMsg(p->name, p->id, max->riskLevel);
		if(isOk == FALSE)
		{
			freeThePeople(&(*peopleArray));
			freeTheDiagnosticHeap(diagnosis);
			errorInStandardLib();
		}
	}

	for (Index i = 0; i < peopleArray->counter; i++)
	{
		Person p = peopleArray->people[i];
		if(p.meetSomeone == FALSE)
		{
			isOk = createReportMsg(p.name, p.id, Clean);
			if(isOk == FALSE)
			{
				freeThePeople(peopleArray);
				freeTheDiagnosticHeap(diagnosis);
				errorInStandardLib();
			}
		}
	}
}

/**
 * Program's main
 * @param argc count of args
 * @param argv args values
 * @return program exit status code
 */
int main(int argc, char **argv)
{
	if(argc != ARGS_COUNT) //check the number of parameters is valid
	{
		usage();
	}

	//Check the given files' paths are valid
	const char* peopleFilePath = argv[1];
//	if(checkFilePath(peopleFilePath, PEOPLE_FILE_NAME) == FALSE)
//	{
//		errorInInputFile();
//	}
	const char* meetingsFilePath = argv[2];
//	if(checkFilePath(meetingsFilePath, MEETINGS_FILE_NAME) == FALSE)
//	{
//		errorInInputFile();
//	}


	PeopleArray peopleArray = {EMPTY, NULL};
	HeapCovidDiagnosis diagnosis = {EMPTY, NULL};
	//retrieve the people's info from the given file
	retrievePeopleInfo(peopleFilePath, &peopleArray);
	//retrieve the meeting's info from the given file & Calc risk for each person
	calcRiskForEachPerson(meetingsFilePath, &peopleArray, &diagnosis);
	//Alert each people about is risk
	alertEachPerson(&diagnosis, &peopleArray);

	freeThePeople(&peopleArray);
	freeTheDiagnosticHeap(&diagnosis);

	return EXIT_SUCCESS;
}
