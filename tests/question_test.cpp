#include <gtest/gtest.h>
#include "question.h"

class QuestionTest : public ::testing::Test {
protected:
    Question question;
    const QString contest{"What is 2 + 2?"};
    const QStringList answers{"2", "3", "4", "5"};
    const uint indexOfCorrectAnswer{2};
    QStringList emptyList;

    void SetUp() override {
        question = Question(contest, answers, indexOfCorrectAnswer);
    }
};

TEST_F(QuestionTest, questionGetterSetterTest) {
    EXPECT_EQ(question.getAnswers(), answers);
    EXPECT_EQ(question.getContents(), contest);
    EXPECT_EQ(question.getIndexOfCorrectAnswer(), indexOfCorrectAnswer);

    EXPECT_TRUE(question.setContents("What is 2 + 4?"));
    EXPECT_TRUE(question.setAnswers(QStringList{"4", "5", "6", "7"}));
    EXPECT_TRUE(question.setIndexOfCorrectAnswer(1));

    EXPECT_NE(question.getAnswers(), answers);
    EXPECT_NE(question.getContents(), contest);
    EXPECT_NE(question.getIndexOfCorrectAnswer(), indexOfCorrectAnswer);
}

TEST_F(QuestionTest, functionShouldThrowExceptionTest) {
    const uint outOfRangeIndex{7};
    const QString newContents;

    EXPECT_THROW(Question q(contest, emptyList, indexOfCorrectAnswer), std::invalid_argument);
    EXPECT_THROW(question.setAnswers(emptyList), std::invalid_argument);
    EXPECT_THROW(Question q(contest, answers, outOfRangeIndex), std::out_of_range);
    EXPECT_THROW(question.setContents(newContents), std::invalid_argument);
    EXPECT_THROW(question.changeAnswer(outOfRangeIndex, "7"), std::out_of_range);
}

TEST_F(QuestionTest, isAnswerCorrectTest) {
    EXPECT_TRUE(question.isAnswerCorrect(indexOfCorrectAnswer));
    uint incorrectIndex{3};
    EXPECT_FALSE(question.isAnswerCorrect(incorrectIndex));
}

TEST_F(QuestionTest, changeAnswerTest) {
    const QStringList newAnswerList{"2", "3", "4", "6"};
    const size_t indexOfAnswer{3};
    EXPECT_TRUE(question.changeAnswer(indexOfAnswer, "6"));
    EXPECT_EQ(question.getAnswers(), newAnswerList);
}

TEST_F(QuestionTest, isEqualAndNotEqualTest) {
    Question anotherQuestion;
    EXPECT_TRUE(anotherQuestion != question);
    anotherQuestion = question;
    EXPECT_TRUE(anotherQuestion == question);
}



