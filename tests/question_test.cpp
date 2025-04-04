#include <gtest/gtest.h>
#include "question.h"

class QuestionTest : public ::testing::Test {
protected:
    Question question;
    const QString contest{"What is 2 + 2?"};
    const QStringList answers{"2", "3", "4", "5"};
    const uint indexOfCorrectAnswer{1};
    QStringList emptyList;

    void SetUp() override {
        question = Question(contest, answers, indexOfCorrectAnswer);
    }
};

TEST_F(QuestionTest, questionGetterSetterTest) {
    EXPECT_EQ(question.getAnswers(), answers);
    EXPECT_EQ(question.getContents(), contest);
    EXPECT_EQ(question.getIndexOfCorrectAnswer(), indexOfCorrectAnswer);

    question.setContents("What is 2 + 4?");
    question.setAnswers(QStringList{"4", "5", "6", "7"});
    question.setIndexOfCorrectAnswer(2);

    EXPECT_NE(question.getAnswers(), answers);
    EXPECT_NE(question.getContents(), contest);
    EXPECT_NE(question.getIndexOfCorrectAnswer(), indexOfCorrectAnswer);
}

TEST_F(QuestionTest, constructorAndsetAnswersShouldThrowExceptionTest) {
    uint outOfRangeIndex{7};

    EXPECT_THROW(Question q(contest, emptyList, indexOfCorrectAnswer), std::invalid_argument);
    EXPECT_THROW(question.setAnswers(emptyList), std::invalid_argument);
    EXPECT_THROW(Question q(contest, answers, outOfRangeIndex), std::out_of_range);
}

TEST_F(QuestionTest, isAnswerCorrectTest) {
    EXPECT_TRUE(question.isAnswerCorrect(indexOfCorrectAnswer));
    uint incorrectIndex{3};
    EXPECT_FALSE(question.isAnswerCorrect(incorrectIndex));
}




