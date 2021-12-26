#include "pch.h"
#include "Lab_E6.h"
#define BUFFER_SIZE 1024

class FunctionalTest : public ::testing::Test {
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

class CheckAnswer : public FunctionalTest {};

TEST_F(FunctionalTest, EmptyTree)
{
    tree_t* tree = ReadBinaryTree("test_files/empty_tree/Input.txt");

    EXPECT_EQ(tree, nullptr);
}

TEST_F(FunctionalTest, OneEmptyBranch)
{
    tree_t* tree = ReadBinaryTree("test_files/one_empty_branch/Input.txt");

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->left, nullptr);
    EXPECT_EQ(tree->right, nullptr);

    EXPECT_STREQ(tree->name, "once");

    FreeTree(tree);
}

TEST_F(FunctionalTest, UnidirectionalTree)
{
    tree_t* tree = ReadBinaryTree("test_files/unidirectional_tree/Input.txt");

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->left, nullptr);
    EXPECT_NE(tree->right, nullptr);
    EXPECT_EQ(tree->right->left, nullptr);
    EXPECT_NE(tree->right->right, nullptr);
    EXPECT_EQ(tree->right->right->left, nullptr);
    EXPECT_NE(tree->right->right->right, nullptr);
    EXPECT_EQ(tree->right->right->right->right, nullptr);
    EXPECT_EQ(tree->right->right->right->right, nullptr);

    EXPECT_STREQ(tree->name, "one");
    EXPECT_STREQ(tree->right->name, "two");
    EXPECT_STREQ(tree->right->right->name, "three");
    EXPECT_STREQ(tree->right->right->right->name, "four");

    FreeTree(tree);
}

TEST_F(FunctionalTest, NormalTree)
{
    tree_t* tree = ReadBinaryTree("test_files/normal_tree/Input.txt");

    ASSERT_NE(tree, nullptr);
    EXPECT_NE(tree->left, nullptr);
    EXPECT_NE(tree->right, nullptr);
    EXPECT_NE(tree->left->left, nullptr);
    EXPECT_EQ(tree->left->right, nullptr);
    EXPECT_NE(tree->left->left->right, nullptr);
    EXPECT_EQ(tree->left->left->left, nullptr);
    EXPECT_EQ(tree->left->left->right->left, nullptr);
    EXPECT_EQ(tree->left->left->right->right, nullptr);
    EXPECT_NE(tree->right->left, nullptr);
    EXPECT_NE(tree->right->right, nullptr);
    EXPECT_EQ(tree->right->right->left, nullptr);
    EXPECT_EQ(tree->right->right->right, nullptr);
    EXPECT_EQ(tree->right->left->left, nullptr);
    EXPECT_NE(tree->right->left->right, nullptr);
    EXPECT_EQ(tree->right->left->right->left, nullptr);
    EXPECT_EQ(tree->right->left->right->right, nullptr);

    EXPECT_STREQ(tree->name, "one");
    EXPECT_STREQ(tree->left->name, "two");
    EXPECT_STREQ(tree->left->left->name, "three");
    EXPECT_STREQ(tree->left->left->right->name, "four");
    EXPECT_STREQ(tree->right->name, "five");
    EXPECT_STREQ(tree->right->left->name, "six");
    EXPECT_STREQ(tree->right->left->right->name, "seven");
    EXPECT_STREQ(tree->right->right->name, "eight");

    FreeTree(tree);
}

int FileCompare(const char* outputFileName, const char* correctOutputFileName)
{
    FILE* output = fopen(outputFileName, "r");
    if (!output)
        return false;
    FILE* correctOutput = fopen(correctOutputFileName, "r");
    if (!correctOutput) {
        fclose(output);
        return false;
    }
    char buf1[BUFFER_SIZE] = { 0 }, buf2[BUFFER_SIZE] = { 0 };
    fread(buf1, sizeof(char), BUFFER_SIZE, output);
    fread(buf2, sizeof(char), BUFFER_SIZE, correctOutput);
    fclose(output);
    fclose(correctOutput);
    return !strcmp(buf1, buf2);
}

TEST_F(CheckAnswer, EmptyTree)
{
    int res = Solution("test_files/empty_tree/Input.txt", "test_files/empty_tree/Output.txt");
    EXPECT_EQ(res, -1);
}

TEST_F(CheckAnswer, OneEmptyBranch)
{
    int res = Solution("test_files/one_empty_branch/Input.txt", "test_files/one_empty_branch/Output.txt");
    EXPECT_EQ(res, 0);
    EXPECT_TRUE(FileCompare("test_files/one_empty_branch/Output.txt", "test_files/one_empty_branch/CorrectOutput.txt"));
}

TEST_F(CheckAnswer, UnidirectionalTree)
{
    int res = Solution("test_files/unidirectional_tree/Input.txt", "test_files/unidirectional_tree/Output.txt");
    EXPECT_EQ(res, 0);
    EXPECT_TRUE(FileCompare("test_files/unidirectional_tree/Output.txt", "test_files/unidirectional_tree/CorrectOutput.txt"));
}

TEST_F(CheckAnswer, NormalTree)
{
    int res = Solution("test_files/normal_tree/Input.txt", "test_files/normal_tree/Output.txt");
    EXPECT_EQ(res, 0);
    EXPECT_TRUE(FileCompare("test_files/normal_tree/Output.txt", "test_files/normal_tree/CorrectOutput.txt"));
}