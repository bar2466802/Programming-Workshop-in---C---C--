/**
 * Ex1 - Bar Melinarskiy - 318189982
 *
 * Exercise 1 - Students' management program. This program accepts 3 options as detailed below:
 * If 'best' is entered then printing the highest graded student out of the given list of students
 * else if 'merge' – The students are sorted in ascending order by their grades
 * and last if 'quick' - The students are sorted in ascending order by their names
 *
 * @return success code - returning 0 if the program finished successfully
 */

#include <stdio.h>
#include "string.h"
#include <stdbool.h>
#include <ctype.h>

//Global Variables
#define MAX_NUM_OF_STUDENTS 5500

//Structures definitions
typedef struct Student
{
	unsigned long int id;
	int grade;
	int age;
	char name[42];
	char country[42];
	char city[42];
}Student;

//Functions declaration
bool checkID(unsigned long int id, char param[], int inputIndex);
bool checkGrade(int grade, char param[], int inputIndex);
bool checkAge(int age, char param[], int inputIndex);
bool checkName(char name[], int inputIndex);
bool checkAddress(char country[]);
bool isInit(char parm[]);
void wrongRequest();
int getStudentsInfo(Student *students, Student* bestStudent, char option[]);
void printStudents(Student* students, int numberOfStudents);
void best(Student* bestStudent);
void merge(Student students[], int left, int divide, int right);
void mergeSort(Student students[], int left, int right);
void swap(Student *s1, Student *s2);
int partition (Student students[], int low, int high);
void quickSort(Student students[], int low, int high);

//Main function
int main(int argc, char* argv[])
{
	const int LEGAL_PARM_NUM = 2;
	const char BEST[] = "best", MERGE[] = "merge", QUICK[] = "quick" ;
	struct Student students[MAX_NUM_OF_STUDENTS];
	struct Student bestStudent;

	if (argc != LEGAL_PARM_NUM) //Checking if the input is only 1 parm as required
	{
		wrongRequest();
		return 1;
	}
	size_t length = 0;
	length = strlen(argv[1]);
	char option[5];
	strncpy(option, argv[1], length);//get the desired option
	if (strcmp(option, BEST) == 0)//Check if the desired option is - best
	{
		//First get the students list
		int numberOfStudents = getStudentsInfo(students, &bestStudent, option);
		if(numberOfStudents > 0)//if the user has entered students then print the best one
		{
			best(&bestStudent);
		}
		else if (numberOfStudents < 0)
		{
			return 1;
		}

	}
	else if (strcmp(option, MERGE) == 0) //Check if the desired option is - merge
	{
		//First get the students list
		int numberOfStudents = getStudentsInfo(students, &bestStudent, option);
		if(numberOfStudents > 0)//if the user has entered students then sort them by grade
		{
			mergeSort(students, 0, numberOfStudents - 1);//do the sorting
			printStudents(students, numberOfStudents);//print the sorted list
		}
		else if (numberOfStudents < 0)
		{
			return 1;
		}
	}
	else if (strcmp(option, QUICK) == 0) //Check if the desired option is - quick
	{
		//First get the students list
		int numberOfStudents = getStudentsInfo(students, &bestStudent, option);
		if(numberOfStudents > 0)//if the user has entered students then sort them by name
		{
			quickSort(students, 0 ,numberOfStudents - 1);//do the sorting
			printStudents(students, numberOfStudents);//print the sorted list
		}
		else if (numberOfStudents < 0)
		{
			return 1;
		}
	}
	else //the user has entered a wrong option
	{
		wrongRequest();
		return 1;
	}
	return 0;
}

/**
* Print wrong usage error to thr user=
*/
void wrongRequest()
{
	printf("USAGE: Wrong program parameter. Run it again with best / merge / quick \n");
}
/**
* Get the students list from the user
* @param students - the students list array, here we store all the entered students
* @param bestStudent - stores the best student's info, only if the option is 'best'
* @param option - the selected program optin
* @retun the number of enterd students, if an error accored return -1 to main
*/
int getStudentsInfo(Student *students, Student* bestStudent, char option[])
{
	//Student's info related variants
	unsigned long int id;
	int grade, age;
	char  name[42], country[42], city[42];
	double bestGrade = 0.0;

	//User input related variants
	const int LEGAL_PARM_NUM = 6;
	const char BEST[] = "best";
	const char QUIT = 'q';
	char line[60], checkLine[60];
	size_t length = 0;
	int numberOfStudents = 0, inputIndex = 0;

	//Ask the user to enter a student's info
	printf("Enter student info. To exit press q, then enter\n");
	//get the student's info from the user
	if ( fgets(line, sizeof(line), stdin) == NULL )
	{
		printf("ERROR: You must enter all the 6 required fields\n");
		return 1;
	}

	//Continue asking form more users until 'q' is entered
	while(!(line[0] == QUIT && line[1] == '\n'))
	{
		length = strlen(line); //get the length of the entered info
		strncpy(checkLine, line, length);//copy the info so we don't change the original input
		//Check if all the 6 required parameters were entered in the right order
		int numOfParams;
		numOfParams = sscanf(line, "%lu,%[^','],%d,%d,%[^','],%s", &id, name, &grade,
							 &age, country, city);
		if (numOfParams != LEGAL_PARM_NUM)
		{
			printf("ERROR: You must enter all the 6 required fields in the right order\n");
			printf("in line %d\n",inputIndex);
			//Ask the user for another input
			printf("Enter student info. To exit press q, then enter\n");
			//get the student's info from the user]
			if ( fgets(line, sizeof(line), stdin) == NULL )
			{
				printf("ERROR: You must enter all the 6 required fields\n");
				printf("in line %d\n",inputIndex);
				return -1;
			}

			inputIndex++;
			continue;
		}
		else
		{
			//Check that each parameter is legal
			char *param = strtok(checkLine, ",");//split the given input by ','
			int index = 0;
			bool ok = true;
			while(param != NULL && index < LEGAL_PARM_NUM && ok)
			{
				index++;
				switch(index)
				{
					case 1:
						//Check the student's ID
						if(checkID(id, param, inputIndex) != true){
							ok = false;//break the while parameters while loop;
						}
						break;
					case 2:
						//Check the student's name
						if(checkName(name, inputIndex) != true){
							ok = false;//break the while parameters while loop;
						}
						break;
					case 3:
						//Check the student's grade
						if(checkGrade(grade, param, inputIndex) != true){
							ok = false;//break the while parameters while loop;
						}
						break;
					case 4:
						//Check the student's grade
						if(checkAge(age, param, inputIndex) != true){
							ok = false;//break the while parameters while loop;
						}
						break;
					case 5:
						//Check the student's country
						if(checkAddress(country) != true){
							printf("ERROR: country can only contain alphabetic characters or '-' "
								   "\n");
							printf("in line %d\n",inputIndex);
							ok = false;//break the while parameters while loop;
						}
						break;
					case 6:
						//Check the student's city
						if(checkAddress(city) != true){
							printf("ERROR: city can only contain alphabetic characters or '-' \n");
							printf("in line %d\n",inputIndex);
							ok = false;//break the while parameters while loop;
						}
						break;
					default:
						ok = false;
				}
				param = strtok(NULL, ",");
			}
			if(ok == true)
			{
				if(index < LEGAL_PARM_NUM)//Check if the user has entered all the 6 parameters
				{
					printf("ERROR: You must enter all the 6 required fields\n");
					printf("in line %d\n",inputIndex);
					continue;
				}
					//Check if the user has entered only the 6 parameters
				else if(index > LEGAL_PARM_NUM)
				{
					printf("ERROR: You must enter only the 6 required fields\n");
					printf("in line %d\n",inputIndex);
					continue;
				}
			}
			else
			{
				//Ask the user for another input
				printf("Enter student info. To exit press q, then enter\n");
				//get the student's info from the user]
				if ( fgets(line, sizeof(line), stdin) == NULL )
				{
					printf("ERROR: You must enter all the 6 required fields\n");
					return -1;
				}

				inputIndex++;
				continue;
			}
		}
		/* If we reached this point then we have only the 6 required parameters and they are all
		 * valid so we can save them in the students array
		*/
		students[numberOfStudents].id = id;
		length = strlen(name); //get the length of name
		strncpy(students[numberOfStudents].name, name, length);
		students[numberOfStudents].age = age;
		students[numberOfStudents].grade = grade;
		length = strlen(country); //get the length of country
		strncpy(students[numberOfStudents].country, country, length);
		length = strlen(city); //get the length of city
		strncpy(students[numberOfStudents].city, city, length);
		numberOfStudents++;//increase the number of entered students

		//Check if we need to find out which student is the best
		if (strcmp(option, BEST) == 0)//Check if the desired option is - best
		{
			double currentGrade = (double)grade / age;
			if(bestGrade < currentGrade)//Check if the current student is better
			{
				//The current student is indeed better so update the output struc info
				bestGrade = currentGrade;
				bestStudent->id = id;
				length = strlen(name); //get the length of name
				strncpy(bestStudent->name, name, length);
				bestStudent->age = age;
				bestStudent->grade = grade;
				length = strlen(country); //get the length of country
				strncpy(bestStudent->country, country, length);
				length = strlen(city); //get the length of city
				strncpy(bestStudent->city, city, length);
			}
		}
		//Ask the user for another input
		printf("Enter student info. To exit press q, then enter\n");
		fgets(line, sizeof(line), stdin);//get the student's info from the user
		inputIndex++;
	}
	return numberOfStudents;
}
/**
* Print the best student's info
* @param bestStudent - stores the best student's info
*/
void best(Student* student)
{
	printf("best student info is: %lu,%s,%d,%d,%s,%s\n",student->id, student->name,
		   student->grade, student->age, student->country, student->city);
}

/**
* Part of the merge-sort implementation - sorting in ascending order the two sub-arrays
* of the student's list and merge them back together
* @param students - the students list array, here we store all the entered students
* @param left - the index of the start of the left sub-array
* @param divide - the index of the end of the left sub-array
* @param right - the index of the end of the right sub-array
*/
void merge(Student students[], int left, int divide, int right)
{
	//indexes
	int i = 0, j = 0, k = 0;
	int n1 = divide - left + 1;
	int n2 = right - divide;

	//create the left & right sub-arrays
	Student leftSub[MAX_NUM_OF_STUDENTS], rightSub[MAX_NUM_OF_STUDENTS];

	//copy the relevant students to the left sub-array
	for (i = 0; i < n1; i++)
	{
		leftSub[i] = students[left + i];
	}
	//copy the relevant students to the right sub-array
	for (j = 0; j < n2; j++)
	{
		rightSub[j] = students[divide + 1 + j];
	}

	//Merge the sub-array while sorting
	i = 0; // Initial index of left subarray
	j = 0; // Initial index of right subarray
	k = left; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
		if (leftSub[i].grade <= rightSub[j].grade)//check which student's has a lower grade
		{
			//left student has the lower grade
			students[k] = leftSub[i];
			i++;
		}
		else
		{
			//right student has the lower/eq grade
			students[k] = rightSub[j];
			j++;
		}
		k++;
	}

	//Making sure all the left-sub-array is copied back to the source
	while (i < n1)
	{
		students[k] = leftSub[i];
		i++;
		k++;
	}

	//Making sure all the right-sub-array is copied back to the source
	while (j < n2)
	{
		students[k] = rightSub[j];
		j++;
		k++;
	}
}
/**
* Part of the merge-sort implementation - sorting the students in ascending order by grade
* @param students - the students list array, here we store all the entered students
* @param left - the index of the start of the left sub-array
* @param right - the index of the end of the right sub-array
*/
void mergeSort(Student students[], int left, int right)
{
	//Continue Sorting as long as the the left index is smaller then the right one
	if (left < right)
	{
		// calc the divide brw the two sub arrays
		int divide = left + (right - left) / 2;

		// Sort the right & left halves
		mergeSort(students, left, divide);
		mergeSort(students, divide+1, right);

		merge(students, left, divide, right);
	}
}
/**
* Print the students' info
* @param students - the students list array, here we store all the entered students
*/
void printStudents(Student* students, int numberOfStudents)
{
	for(int i = 0 ; i < numberOfStudents; i++)
	{
		printf("%lu,%s,%d,%d,%s,%s\n",students[i].id, students[i].name,
			   students[i].grade, students[i].age, students[i].country, students[i].city);
	}
}
/**
* Part of the quick-sort implementation - sorting the students in ascending order by name
* @param students - the students list array, here we store all the entered students
* @param low - the starting index
* @param high - the ending index
*/
void quickSort(Student students[], int low, int high)
{
	//Continue Sorting as long as the the low index is smaller then the high one
	if (low < high)
	{
		//partitioning index
		int pivot = partition(students, low, high);

		/**
		* Sorting the students who are before the partition-student and the once who are after
		* him by name
		*/
		quickSort(students, low, pivot - 1);
		quickSort(students, pivot + 1, high);
	}
}
/**
* Part of the quick-sort implementation - replace the location of 2 students
* @param s1 - student1
* @param s2 - student2
*/
void swap(Student *s1, Student *s2){
	Student temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}
/**
* Part of the quick-sort implementation - sort the current sub-array and find the next pivot
* @param s1 - student1
* @param s2 - student2
* @return the index of the next pivot student
*/
int partition (Student students[], int low, int high)
{
	const int EQUAL = 0;//Flag if two names are the same
	Student pivot = students[high];//get the info of the pivot student
	int i = (low - 1); //index of first student in the sub-array

	for (int j = low; j <= high- 1; j++)
	{
		//Checking the current student name in compersion to the pivot student
		if (strcmp(students[j].name, pivot.name) < EQUAL)
		{
			//Indeed the current student's name is before the pivot-student one
			i++;
			swap(&students[i], &students[j]);
		}
	}
	swap(&students[i + 1], &students[high]);
	return (i + 1);
}
/**
* Check that the given student's ID is as required
* @param id - the ID as an number
* @param param - the ID as a string
* @param inputIndex - the index of the current input
* @return true if the ID is ok, false otherwise
*/
bool checkID(unsigned long int id, char param[], int inputIndex)
{
	const unsigned long int MAX_ID = 10000000000, MIN_ID = 999999999;

	//First we check if the entered grade is a real number
	bool ok = isInit(param);
	if(ok == false)
	{
		printf("ERROR: ID must only be a 10 digits number\n");
		printf("in line %d\n",inputIndex);
		return false;
	}

	//Check if the id is a 10 digits number only
	if(id < MIN_ID || id > MAX_ID){
		printf("ERROR: ID must be a 10 digits number\n");
		printf("in line %d\n",inputIndex);
		return false;
	}
	return true;
}
/**
* Check that the given student's grade is as required
* @param grade - the grade as an number
* @param param - the grade as a string
* @param inputIndex - the index of the current input
* @return true if the grade is ok, false otherwise
*/
bool checkGrade(int grade, char param[], int inputIndex)
{
	//First we check if the entered grade is a real number
	bool ok = isInit(param);
	if(ok == false)
	{
		printf("ERROR: Grade must only be a number between 0 and 100\n");
		printf("in line %d\n",inputIndex);
		return false;
	}
	const int MAX_GRADE = 100, MIN_GRADE = 0;
	//Check if the grade is btw 0 and 100 only
	if(grade < MIN_GRADE || grade > MAX_GRADE){
		printf("ERROR: Grade must be between 0 and 100\n");
		printf("in line %d\n",inputIndex);
		return false;
	}
	return true;
}
/**
* Check that the given student's age is as required
* @param age - the age as an number
* @param param - the age as a string
* @param inputIndex - the index of the current input
* @return true if the age is ok, false otherwise
*/
bool checkAge(int age, char param[], int inputIndex)
{
	//First we check if the entered age is a real number
	bool ok = isInit(param);
	if(ok == false)
	{
		printf("ERROR: Age must only be a number between 18 and 120\n");
		printf("in line %d\n",inputIndex);
		return false;
	}
	const int MAX_AGE = 120, MIN_AGE = 18;
	//Check if the grade is btw 0 and 100 only
	if(age < MIN_AGE || age > MAX_AGE){
		printf("ERROR: Age must be between 18 and 120\n");
		printf("in line %d\n",inputIndex);
		return false;
	}
	return true;
}
/**
* Check that the given student's name is as required
* @param name - the name of the student
* @param inputIndex - the index of the current input
* @return true if the name is ok, false otherwise
*/
bool checkName(char name[], int inputIndex)
{
	int length;
	length = strlen(name);

	for (int i = 0; i < length; i++)
	{
		//If the name not contains only letters, space and '-' then return with error
		if (!isalpha(name[i]) && !(isspace(name[i])) && !(name[i] == '-'))
		{
			printf("ERROR: name can only contain alphabetic characters, spaces or ’-’\n");
			printf("in line %d\n",inputIndex);
			return false;
		}
	}
	return true;//the name is ok
}
/**
* Check that the given student's address is as required
* @param address - the name of address meaning the name of the city/country
* @param inputIndex - the index of the current input
* @return true if the address is ok, false otherwise
*/
bool checkAddress(char place[])
{
	int length;
	length = strlen(place);
	bool ok = true;
	for (int i = 0; i < length; i++)
	{
		//Check if the place contains only letters, spaces or '-', if not then return with error
		ok = isalpha(place[i]) || isspace(place[i]) || place[i] == '-';
		if (ok == false)
		{
			return false;
		}
	}
	return true;//the place is ok
}
/**
* Check that the given param is indeed a number
* @param parm - the given parameter
* @return true if the param is ok, false otherwise
*/
bool isInit(char parm[])
{
	int length;
	length = strlen(parm);

	for (int i = 0; i < length; i++)
	{
		//If the name not contains only letters, space and '-' then return with error
		if (!isdigit(parm[i]) && parm[i] != '\n')
		{
			return false;
		}
	}
	return true;//the name is ok
}