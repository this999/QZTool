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
    EXPECT_TRUE(secondQuestionManager.addQuestion(contest, answers, indexOfCorrectAnswer));
    EXPECT_TRUE(secondQuestionManager.addQuestion(secondQuestion));

    EXPECT_EQ(secondQuestionManager, questionManager);

    QString emptyQString;
    QStringList emptyQStringList;
    EXPECT_NO_THROW(secondQuestionManager.addQuestion(emptyQString, answers, indexOfCorrectAnswer));
    EXPECT_NO_THROW(secondQuestionManager.addQuestion(contest, emptyQStringList, indexOfCorrectAnswer));
}

TEST_F(QuestionManagerTest, removeQuestion) {
    secondQuestionManager.clearQuestions();
    secondQuestionManager.addQuestion(firstQuestion);
    secondQuestionManager.addQuestion(secondQuestion);
    secondQuestionManager.addQuestion(secondQuestion);

    ASSERT_NE(secondQuestionManager, questionManager);
    EXPECT_TRUE(secondQuestionManager.removeQuestion(2));
    EXPECT_EQ(secondQuestionManager, questionManager);
    EXPECT_FALSE(secondQuestionManager.removeQuestion(9));
}

TEST_F(QuestionManagerTest, changeAnswerTest) {
    const size_t indexOfAnswer{1};
    const QStringList newCorrectAnswers{"2", "8", "4", "5"};

    EXPECT_TRUE(secondQuestionManager.changeOneAnswer(firstIndex, indexOfAnswer, "8"));
    EXPECT_NE(secondQuestionManager, questionManager);
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getAnswers(), newCorrectAnswers);
    EXPECT_EQ(secondQuestionManager.getQuestion(secondIndex).getAnswers(), secondAnswers);
    EXPECT_EQ(secondQuestionManager.getAllQuestions().size(), questionManager.getAllQuestions().size());
    EXPECT_NO_THROW(secondQuestionManager.changeOneAnswer(7, 1, "Dummy answer"));
    EXPECT_NO_THROW(secondQuestionManager.changeOneAnswer(0, 9, "Dummy answer"));

    secondQuestionManager = QuestionManager(questions);

    EXPECT_TRUE(secondQuestionManager.changeAllAnswers(firstIndex, newCorrectAnswers));
    EXPECT_NE(secondQuestionManager, questionManager);
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getAnswers(), newCorrectAnswers);
    EXPECT_EQ(secondQuestionManager.getQuestion(secondIndex).getAnswers(), secondAnswers);
    EXPECT_EQ(secondQuestionManager.getAllQuestions().size(), questionManager.getAllQuestions().size());
    EXPECT_NO_THROW(secondQuestionManager.changeAllAnswers(7, secondAnswers));
}

TEST_F(QuestionManagerTest, changeIndexOfCorrectAnswerTest) {
    const uint newIndexOfCorrectAnswer{3};

    EXPECT_TRUE(secondQuestionManager.changeIndexOfCorrectAnswer(firstIndex, newIndexOfCorrectAnswer));
    EXPECT_NE(secondQuestionManager, questionManager);
    EXPECT_EQ(secondQuestionManager.getAllQuestions().size(), questionManager.getAllQuestions().size());
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getIndexOfCorrectAnswer(), newIndexOfCorrectAnswer);
    EXPECT_NO_THROW(secondQuestionManager.changeIndexOfCorrectAnswer(5, newIndexOfCorrectAnswer));
}

TEST_F(QuestionManagerTest, changeContentsTest) {
    const QString newContents{"What is 2 + 1?"};
    const QString emptyContent;

    EXPECT_TRUE(secondQuestionManager.changeContents(firstIndex, newContents));
    EXPECT_NE(secondQuestionManager, questionManager);
    EXPECT_EQ(secondQuestionManager.getAllQuestions().size(), questionManager.getAllQuestions().size());
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getContents(), newContents);
    EXPECT_NO_THROW(secondQuestionManager.changeContents(5, newContents));
    EXPECT_NO_THROW(secondQuestionManager.changeContents(0, emptyContent));
}

TEST_F(QuestionManagerTest, getOneAndAllAndClearQuestionTest) {
    EXPECT_EQ(secondQuestionManager.getAllQuestions(), questions);
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex), firstQuestion);

    secondQuestionManager.clearQuestions();
    EXPECT_TRUE(secondQuestionManager.getAllQuestions().isEmpty());
}

TEST_F(QuestionManagerTest, functionShouldThrowExceptionTest) {
    QuestionManager emptyQuestionManager;
    const QMap<uint, Question> emptyQuestionMap;
    Question emptyQuestion;

    EXPECT_THROW(QuestionManager qm(emptyQuestionMap), std::invalid_argument);
    EXPECT_THROW(secondQuestionManager.addQuestion(emptyQuestion), std::invalid_argument);
    try {
        secondQuestionManager.addQuestion(emptyQuestion);
    }
    catch (const std::invalid_argument& e) {
        EXPECT_STREQ(e.what(), "Questions must have contents.");
    }

    emptyQuestion.setContents("Dummy contents");
    EXPECT_THROW(secondQuestionManager.addQuestion(emptyQuestion), std::invalid_argument);
    try {
        secondQuestionManager.addQuestion(emptyQuestion);
    }
    catch (const std::invalid_argument& e) {
        EXPECT_STREQ(e.what(), "Questions must have at least one answer.");
    }

    EXPECT_THROW(emptyQuestionManager.getQuestion(1), std::out_of_range);
    EXPECT_NO_THROW(emptyQuestionManager.changeAllAnswers(7, secondAnswers));
}

TEST_F(QuestionManagerTest, isClearTest) {
    ASSERT_FALSE(secondQuestionManager.getAllQuestions().isEmpty());
    secondQuestionManager.clearQuestions();
    EXPECT_TRUE(secondQuestionManager.getAllQuestions().isEmpty());
}

TEST_F(QuestionManagerTest, isEqualTest) {
    EXPECT_TRUE(secondQuestionManager == questionManager);
    secondQuestionManager.clearQuestions();
    EXPECT_TRUE(secondQuestionManager != questionManager);
}
