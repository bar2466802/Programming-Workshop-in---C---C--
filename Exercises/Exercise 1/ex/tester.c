#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "def.h"
#include "testerTools.c"

typedef struct Test
{
	char command[10];
	char file[50];
	char expected[50];
} Test;

Test tests[10] = {{"best", "./tests/test1.txt", "./tests/expected1.txt"},
				  {"best", "./tests/test2.txt", "./tests/expected2.txt"},
				  {"best", "./tests/test3.txt", "./tests/expected3.txt"},
				  {"best", "./tests/test4.txt", "./tests/expected4.txt"},
				  {"best", "./tests/test5.txt", "./tests/expected5.txt"},
				  {"best", "./tests/test6.txt", "./tests/expected6.txt"},
				  {"best", "./tests/test7.txt", "./tests/expected7.txt"}};
int testsLen = 7;


int main(int argc, char *args[])
{
	if (argc == 2 && strcmp(args[1], "school") == 0)
	{
		printf("Testing school solution\n");
	}
	else if (!compile())
	{
		return 0;
	}

	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);

	// general input tests
	printf("General input testing: \n");
	int status;

	if (inputTesting())
	{
		printf("preforming misuse return value test\n");
		if (usageTest())
		{
			printf("\nStarting automatic best test: \n");
			if (bestTest(BATCH_SIZE))
			{
				printf("\nStarting automatic sorting test: \n");
				printf("testing 'merge'-\n");
				status = sortingTest("merge", BATCH_SIZE);

				if (status)
				{
					printf("\ntesting 'quick'-\n");
					if (sortingTest("quick", BATCH_SIZE))
					{
						printf("\n ~~ all tests passed ~~\n");
						clock_gettime(CLOCK_MONOTONIC, &end);
						double time_taken;
						time_taken = (end.tv_sec - start.tv_sec) * 1e9;
						time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
						printf("\nThe test took %f seconds to execute \n", time_taken);
						fprintf(stderr, "\nMake sure your execution time isn't significantly longer than the school solution\n for this test. (~5 seconds *ON THE UNIVERSITY'S COMPUTERS*)\n");
					}
				}
			}
		}
	}

	return 0;
}

int bestTest(int batchSize)
{
	int success = 1;
	for (int i = 0; i < RERUNS; i++)
	{
		createBestTest(batchSize);
		if (runTest(AUTOMATED_TEST, "best", OUTPUT) != 0)
		{
			fprintf(stderr, "Failed running the compiled file, exit code was 1.\n");
			success = 0;
			break;
		}
		if (!checkBest(batchSize))
		{
			fprintf(stderr, "\nFailed best test, check "OUTPUT" and "AUTOMATED_TEST" for more info\n");
			openDebugFiles(AUTOMATED_TEST, "", OUTPUT);
			success = 0;
			break;
		}
		doProgress("Testing", i+1, RERUNS);
	}

	if (success)
	{
		printf("\ntest passed successfully!\n");
		return 1;
	}
	return 0;
}


int sortingTest(char *command, int batchSize)
{
	int success = 1;
	for (int i = 0; i < RERUNS; i++)
	{
		createSortingTest(batchSize);
		if (runTest(AUTOMATED_TEST, command, OUTPUT) != 0)
		{
			fprintf(stderr, "Failed running the compiled file, exit code was 1.\n");
			success = 0;
			break;
		}
		if (strcmp(command, "quick") == 0)
		{
			if (!checkSortingName(batchSize))
			{
				fprintf(stderr, "\nFailed sorting, check output file and test.txt for more info\n");
				openDebugFiles(AUTOMATED_TEST, "", OUTPUT);
				success = 0;
				break;
			}
		}
		else if (strcmp(command, "merge") == 0)
		{
			if (!checkSortingGrade(batchSize))
			{
				fprintf(stderr, "\nFailed sorting, check output file and test.txt for more info\n");
				openDebugFiles(AUTOMATED_TEST, "", OUTPUT);
				success = 0;
				break;
			}
		}
		doProgress("Testing", i+1, RERUNS);
		}

	if (success)
	{
		printf("\ntest passed successfully!\n");
		return 1;
	}
	return 0;
}

int inputTesting()
{
	Test test;
	int success = 1, i;
	for (i = 0; i < testsLen; i++)
	{
		test = tests[i];

		printf("Running test #%d... \n", i+1);
		if (fileExist(test.file) && fileExist(test.expected))
		{
			if (runTest(test.file, test.command, OUTPUT) != 0)
			{
				fprintf(stderr, "Failed running the compiled file, exit code was 1.\n");
				success = 0;
				break;
			}
			if (preformTest(OUTPUT, test.expected) == 1)
			{
				printf("Test #%d passed successfully\n", i+1);
			}
			else
			{
				fprintf(stderr, "Failed test %d, you can view the 'output and '%s' files to compare the two.\n", i+1, test.expected);
				openDebugFiles(tests[i].file, tests[i].expected, OUTPUT);
				success = 0;
				break;
			}
		}
		else
		{
			fprintf(stderr, "missing test/expected file for test #%d \n", i+1);
			success = 0;
		}
		printf("\n");
	}

	return success;
}

int usageTest()
{
	if (runTest("./tests/usageTest.txt", "asfd", OUTPUT) != 1 ||
		runTest("./tests/usageTest.txt", "", OUTPUT) != 1 ||
		runTest("./tests/usageTest.txt", "best quick", OUTPUT) != 1)
	{
		fprintf(stderr, "Failed usage test, expected to return exit code 1 on misuse\n");
		return 0;
	}

	FILE *output = fopen(OUTPUT, "r");
	char line[LINE_SIZE];
	fgets(line, LINE_SIZE, output);
	fclose(output);

	sscanf(line, "%[^\n]", line);
	if (!startsWith(line, "USAGE:"))
	{
		fprintf(stderr, "Failed usage test, expected 'USAGE:'... but got '%s'\n", line);
		return 0;
	}
	return 1;
}

