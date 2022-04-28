/**
 * Bar Melinarskiy - 4/8/20 - 318189982 - username bar246802
 * Test in C - SpreaderDetectorBackend
 */
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
#define  APPEND_AUTH "a"
#define EMPTY 0
#define  MAX_RISK 1;

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
 * @property id - the id of the person
 * @property age - the age of the person
 * @property name - the name of the person
 * @property meetSomeone - if this person was exposed to someone during a meeting
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
 * @property counter - the number of people in the array
 * @property people - the dynamic array of the received people
 */
typedef struct PeopleArray
{
	unsigned long int counter;
	Person *people;
} PeopleArray;

/**
 *A data structure for storing all Covid-19 risks diagnostic.
 * @property personInfo - the person for whom the diagnostic is for
 * @property risk - the risk for infection
 * @property riskLevel - the level of risk to get infected
 */
typedef struct Diagnosis
{
	Person* personInfo;
	float risk;
	RiskLevel riskLevel;

} Diagnosis;

/**
* A data structure for storing node in the heap storing each person covid-19 risk.
* @property counter - the number of people in the heap
* @property people - the dynamic heap of the received people (as an array)
*/
typedef struct HeapCovidDiagnosis
{
	unsigned long int count;
	Diagnosis *heap;
} HeapCovidDiagnosis;

/**
* A data structure for storing a meeting's info btw 2 people
* @property sourceId - ID for the first person
* @property targetId - ID for the second person
* @property distance - the distance btw the two people
* @property time - the meeting time
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
	Index len = peopleArray->counter;
	for (Index i = 0; i < len; i++)
	{
		free(peopleArray->people[i].name);
	}
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
 * Write the full Covid-19 diagnostic info for each person.
 * @param reportInfo - the diagnostic info to write to the output file
 * @param auth - the auth to open the file with wrtie/append
 */
void exportDiagnosticInfo(const char reportInfo[], const char* auth)
{
	FILE *outputFile = NULL;
	outputFile = fopen(OUTPUT_FILE, auth);
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
* @return positive number if the first person id is bigger, negative if it's smaller
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
* Inset a new person to the people array from the input file
* @param input - the current line from the input file
* @param index - the current line number in the file
* @param file - the input file
* @param peopleArray - array of people to fill with given info in file
*/
void insertNewPerson(char* input, Index index, FILE** file, PeopleArray* peopleArray)
{
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
	char* delimiter = " ";
	char *param = strtok(input, delimiter);
	int i = 0;
	while (param != NULL && i < 3)
	{
		if(i == 0) //Name
		{
			size_t sz = strlen(param);
			peopleArray->people[index].name = (char*)malloc(sz * sizeof(char) + 1);
			if(peopleArray->people[index].name == NULL)
			{
				//Something went wrong, exit the program and free dynamic memory
				freeThePeople(peopleArray);
				fclose(*file);
				errorInStandardLib();
			}

			strcpy(peopleArray->people[index].name, param);
		}
		else if (i == 1) //Id
		{
			char * pEndId;
			const int BASE = 10;
			peopleArray->people[index].id = strtoull(param, &pEndId, BASE);
		}
		else if (i == 2) //Age
		{
			char * pEndAge;
			peopleArray->people[index].age = strtof(param, &pEndAge);
		}

		param = strtok(NULL, delimiter);
		i++;
	}

	peopleArray->people[index].meetSomeone = TRUE;
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
		insertNewPerson(line, index, &file, peopleArray);
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
		exportDiagnosticInfo(info, WRITE_AUTH);
		exit(EXIT_SUCCESS);
	}
	//Sort the array by the id
	qsort(peopleArray->people, peopleArray->counter, sizeof(Person), compare);
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
* @param d1 - first element
* @param d2 - second element
*/
void swap(Diagnosis *d1, Diagnosis *d2)
{
	Diagnosis temp = *d1;
	*d1 = *d2;
	*d2 = temp;
}
/**
* Get index of left son of current index
* @param index - index of the current node
* @return the index of the left son
*/
Index heapGetLeft(Index index)
{
	return (2 * index) + 1;
}
/**
* Get index of right son of current index
* @param index - index of the current node
* @return the index of the right son
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
	//Check which node has the largest risk
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
	   && diagnosis->heap[rightIndex].risk > diagnosis->heap[largestIndex].risk)
	{
		largestIndex = rightIndex;
	}

	if(largestIndex != index) //check if we need to switch
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
Diagnosis heapExtractMax(HeapCovidDiagnosis* diagnosis)
{
	//Check if the heap os empty
	Diagnosis max = {NULL, 0, Clean};
	if(diagnosis == NULL || diagnosis->count == EMPTY)
	{
		return max;
	}

	max = diagnosis->heap[0];
	//Fix heap after extraction
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
	diagnosis->heap[index].risk = risk;
	Index parentIndex = heapGetParent(index);
	Index i = index;
	while(i > 0 && diagnosis->heap[parentIndex].risk < diagnosis->heap[i].risk)
	{
		swap(&diagnosis->heap[i], &diagnosis->heap[parentIndex]);
		i = parentIndex;
		parentIndex = heapGetParent(i);
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
		personInfo->meetSomeone = TRUE;
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
			free(*input);
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
			free(*input);
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
		free(*input);
		fclose(*file);
		errorInInputFile();
	}
	int isOk = insertToHeap(diagnosis, personInfo, meeting);
	if(isOk == FALSE)
	{
		//Something went wrong, exit the program and free dynamic memory
		freeThePeople(peopleArray);
		freeTheDiagnosticHeap(diagnosis);
		free(*input);
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
		//Something went wrong, exit the program and free dynamic memory
		freeThePeople(peopleArray);
		freeTheDiagnosticHeap(diagnosis);
		errorInInputFile(); //no file path was received
	}

	FILE *file = fopen(path, READ_AUTH);
	if(file == NULL)
	{
		//Something went wrong, exit the program and free dynamic memory
		freeThePeople(peopleArray);
		freeTheDiagnosticHeap(diagnosis);
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
		//Something went wrong, exit the program and free dynamic memory
		freeThePeople(peopleArray);
		freeTheDiagnosticHeap(diagnosis);
		errorInInputFile();
	}
}
/**
 * Create msg to export to output file
 * @param id - id of the person to inform
 * @param riskLevel - risk level of the person to inform
 * @param index - index of line in output file
 * @return 1 if the operation was successful and the input is valid, otherwise  returning with 0
 */
Boolean createReportMsg(char* name, ID id, RiskLevel riskLevel, const Index index)
{
	//Get template of message by risk level
	char* msgTemplate = CLEAN_MSG;
	if(riskLevel == Hospital)
	{
		msgTemplate = MEDICAL_SUPERVISION_THRESHOLD_MSG;
	}
	else if(riskLevel == Quarantine)
	{
		msgTemplate = REGULAR_QUARANTINE_MSG;
	}

	size_t sz = snprintf(NULL, 0, msgTemplate, name, id);
	char* msg = (char*)malloc(sz * sizeof(char) + 1);
	if(msg != NULL)
	{
		snprintf(msg, sz + 1, msgTemplate, name, id);
		if(index == 0) //check if to append to write new file
		{
			exportDiagnosticInfo(msg, WRITE_AUTH);
		}
		else
		{
			exportDiagnosticInfo(msg, APPEND_AUTH);
		}

		free(msg);
		msg = NULL;
		return TRUE;
	}

	return FALSE;
}

/**
 * Inform each person about his/hers diagnostic
 * @param diagnosis - heap with the each person's diagnostic
 * @param peopleArray - array of people  info
 * @return 1 if the operation was successful and the input is valid, otherwise  returning with 0
 */
void alertEachPerson(HeapCovidDiagnosis* diagnosis, PeopleArray* peopleArray)
{
	Boolean isOk = TRUE;
	Index totalCount = 0;
	Index len = diagnosis->count;
	//Loop on all the people who met someone
	for (Index i = 0; i < len; i++)
	{
		Diagnosis max =  heapExtractMax(diagnosis);
		Person* p = max.personInfo;
		isOk = createReportMsg(p->name, p->id, max.riskLevel, totalCount);
		if(isOk == FALSE)
		{
			freeThePeople(&(*peopleArray));
			freeTheDiagnosticHeap(diagnosis);
			errorInStandardLib();
		}
		totalCount++;
	}
	//Loop on all the people who didn't met someone
	for (Index i = 0; i < peopleArray->counter; i++)
	{
		Person p = peopleArray->people[i];
		if(p.meetSomeone == FALSE)
		{
			isOk = createReportMsg(p.name, p.id, Clean, totalCount);
			if(isOk == FALSE)
			{
				freeThePeople(peopleArray);
				freeTheDiagnosticHeap(diagnosis);
				errorInStandardLib();
			}
		}
		totalCount++;
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

	const char* peopleFilePath = argv[1];
	const char* meetingsFilePath = argv[2];

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
