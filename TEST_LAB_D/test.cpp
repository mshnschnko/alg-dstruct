#include "pch.h"
#include "labdstr.h"

#define MAX_TASKS_PER_STAFF_MEMBER 15
#define GARBAGE 0xAF

/*
IDE: Visual Studio 2019 Community Edition
OS: Windows 10 Pro, Version 20H2, Build 19042.1348
PC configuration:
	CPU: Intel(R) Core(TM) i5-3230M CPU @ 2.60GHz   2.60 GHz
	Motherboard: ASUSTeK COMPUTER INC.  X75VC
	RAM: 12 GB, DDR3
	SSD: SSD M.2 256GB
*/

/*
Lab D, Variant 31 (TaskManagment):
Results:
	Stress Test Time: 44012 ms
	Memory Costs: About 44 MB
*/

class FunctionalTest : public ::testing::Test
{
protected:
	void SetUp() {
		_CrtMemCheckpoint(&m_before);
	}
	void TearDown() {   
		_CrtMemCheckpoint(&m_after);
		if (_CrtMemDifference(&m_diff, &m_before, &m_after)) {
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
			_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
			_CrtMemDumpStatistics(&m_diff);
			FAIL();
		}
	}
private:
	_CrtMemState m_before, m_after, m_diff;
};

void CheckAnswer(const char* inputFileName, const char* outputFileName, const char* correctOutputFileName)
{
	int res = LabSolution(inputFileName, outputFileName);
	EXPECT_EQ(res, 0);
	FILE* outputForCheck = fopen(outputFileName, "r");
	ASSERT_NE(nullptr, outputForCheck);
	unsigned answer[MAX_TASKS_PER_STAFF_MEMBER] = { GARBAGE };
	for (int i = 0; i < MAX_TASKS_PER_STAFF_MEMBER; i++)
		if (!fscanf(outputForCheck, "%c", &answer[i]))
			break;
	fclose(outputForCheck);
	FILE* correctOutputForCheck = fopen(correctOutputFileName, "r");
	unsigned correctAnswer[MAX_TASKS_PER_STAFF_MEMBER] = { GARBAGE };
	for (int i = 0; i < MAX_TASKS_PER_STAFF_MEMBER; i++)
		if (!fscanf(correctOutputForCheck, "%c", &correctAnswer[i]))
			break;
	fclose(correctOutputForCheck);
	EXPECT_TRUE(!memcmp(answer, correctAnswer, MAX_TASKS_PER_STAFF_MEMBER * sizeof(unsigned)));
}

TEST_F(FunctionalTest, ExampleFromTaskFile)
{
	CheckAnswer("test_files/example_from_task_file/Input.txt", "test_files/example_from_task_file/Output.txt", 
		"test_files/example_from_task_file/CorrectOutput.txt");
}

TEST_F(FunctionalTest, NotEnoughEmployees)
{
	CheckAnswer("test_files/not_enough_employees/Input.txt", "test_files/not_enough_employees/Output.txt", 
		"test_files/not_enough_employees/CorrectOutput.txt");
}

TEST_F(FunctionalTest, tasksEndedEarlierThanEmployees)
{
	CheckAnswer("test_files/tasks_ended_earlier_than_employees/Input.txt", "test_files/tasks_ended_earlier_than_employees/Output.txt", 
		"test_files/tasks_ended_earlier_than_employees/CorrectOutput.txt");
}

TEST_F(FunctionalTest, AllEmployeesAreUsedExists)
{
	CheckAnswer("test_files/all_employees_are_used_exists/Input.txt", "test_files/all_employees_are_used_exists/Output.txt", 
		"test_files/all_employees_are_used_exists/CorrectOutput.txt");
}

TEST(Stress_test, GenerateInput)
{
	FILE* input = fopen("test_files/stress_test/Input.txt", "w");
	ASSERT_NE(nullptr, input);
	unsigned tasks = 1000, fullTime = 10000, staffMembers = 700;
	fprintf(input, "%u %u %u\n", tasks, fullTime, staffMembers);
	for (int i = 0; i < (int)tasks; i++)
		fprintf(input, "%d ", (rand() % fullTime + 1));
	fclose(input);
}

TEST(Stress_test, Solution)
{
	int k = LabSolution("test_files/stress_test/Input.txt", "test_files/stress_test/Output.txt");
	ASSERT_TRUE(!k);
}