#include <gtest/gtest.h>

#include "QZToolCore/question_manager.h"

class QuestionManagerTest : public ::testing::Test {
protected:
    const QString contest{"What is 2 + 2?"};
    const QStringList answers{"2", "3", "4", "5"};
    const QString secondContest{"What is 2 + 4?"};
    const QStringList secondAnswers{"4", "5", "6", "7"};
    const uint indexOfCorrectAnswer{2};
    const uint firstIndex{0};
    const uint secondIndex{1};
    const Question firstQuestion{contest,answers, indexOfCorrectAnswer};
    const Question secondQuestion{secondContest, secondAnswers, indexOfCorrectAnswer};
    const QMap<uint, Question> questions{{firstIndex, firstQuestion}, {secondIndex, secondQuestion}};
    const QuestionManager questionManager = QuestionManager(questions);
    QuestionManager secondQuestionManager;

    void SetUp() override {
        secondQuestionManager = QuestionManager(questions);
    }
};

TEST_F(QuestionManagerTest, addQuestionTest) {
    secondQuestionManager.clearQuestions();
    secondQuestionManager.addQuestion(contest, answers, indexOfCorrectAnswer);
    secondQuestionManager.addQuestion(secondQuestion);

    EXPECT_EQ(secondQuestionManager, questionManager);
}

TEST_F(QuestionManagerTest, removeQuestion) {
    secondQuestionManager.clearQuestions();
    secondQuestionManager.addQuestion(firstQuestion);
    secondQuestionManager.addQuestion(secondQuestion);
    secondQuestionManager.addQuestion(secondQuestion);

    ASSERT_NE(secondQuestionManager, questionManager);

    secondQuestionManager.removeQuestion(2);

    EXPECT_EQ(secondQuestionManager, questionManager);
}

TEST_F(QuestionManagerTest, changeAnswerTest) {
    const size_t indexOfAnswer{1};
    const QStringList newCorrectAnswers{"2", "8", "4", "5"};

    secondQuestionManager.changeOneAnswer(firstIndex, indexOfAnswer, "8");

    EXPECT_NE(secondQuestionManager, questionManager);
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getAnswers(), newCorrectAnswers);
    EXPECT_EQ(secondQuestionManager.getQuestion(secondIndex).getAnswers(), secondAnswers);
    EXPECT_EQ(secondQuestionManager.getAllQuestions().size(), questionManager.getAllQuestions().size());

    secondQuestionManager = QuestionManager(questions);
    secondQuestionManager.changeAllAnswers(firstIndex, newCorrectAnswers);

    EXPECT_NE(secondQuestionManager, questionManager);
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getAnswers(), newCorrectAnswers);
    EXPECT_EQ(secondQuestionManager.getQuestion(secondIndex).getAnswers(), secondAnswers);
    EXPECT_EQ(secondQuestionManager.getAllQuestions().size(), questionManager.getAllQuestions().size());
}

TEST_F(QuestionManagerTest, changeIndexOfCorrectAnswerTest) {
    const uint newIndexOfCorrectAnswer{3};
    secondQuestionManager.changeIndexOfCorrectAnswer(firstIndex, newIndexOfCorrectAnswer);

    EXPECT_NE(secondQuestionManager, questionManager);
    EXPECT_EQ(secondQuestionManager.getAllQuestions().size(), questionManager.getAllQuestions().size());
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getIndexOfCorrectAnswer(), newIndexOfCorrectAnswer);
}

TEST_F(QuestionManagerTest, changeContentsTest) {
    const QString newContents{"What is 2 + 1?"};
    secondQuestionManager.changeContents(firstIndex, newContents);

    EXPECT_NE(secondQuestionManager, questionManager);
    EXPECT_EQ(secondQuestionManager.getAllQuestions().size(), questionManager.getAllQuestions().size());
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getContents(), newContents);
}

TEST_F(QuestionManagerTest, getOneAndAllAndClearQuestionTest) {
    EXPECT_EQ(secondQuestionManager.getAllQuestions(), questions);
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex), firstQuestion);

    secondQuestionManager.clearQuestions();
    EXPECT_TRUE(secondQuestionManager.getAllQuestions().isEmpty());
}

//Dodac testy operatorów == !=


