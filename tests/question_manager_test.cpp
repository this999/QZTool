#include <QSet>
#include <gtest/gtest.h>
#include <limits>

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
  const Question firstQuestion{contest, answers, indexOfCorrectAnswer};
  const Question secondQuestion{secondContest, secondAnswers,
                                indexOfCorrectAnswer};
  const QMap<uint, Question> questions{{firstIndex, firstQuestion},
                                       {secondIndex, secondQuestion}};
  const QuestionManager questionManager = QuestionManager(questions);
  QuestionManager secondQuestionManager;

  void SetUp() override { secondQuestionManager = QuestionManager(questions); }
};

TEST_F(QuestionManagerTest, addQuestionTest) {
  secondQuestionManager.clearQuestions();
  secondQuestionManager.addQuestion(contest, answers, indexOfCorrectAnswer);
  secondQuestionManager.addQuestion(secondQuestion);

  EXPECT_EQ(secondQuestionManager, questionManager);

  QString emptyQString;
  QStringList emptyQStringList;
  uint indexOfOutOfRangeAnswer{9};
  EXPECT_THROW(secondQuestionManager.addQuestion(emptyQString, answers,
                                                 indexOfCorrectAnswer),
               std::invalid_argument);
  EXPECT_THROW(secondQuestionManager.addQuestion(contest, QStringList{"a"},
                                                 indexOfOutOfRangeAnswer),
               std::out_of_range);
  EXPECT_THROW(secondQuestionManager.addQuestion(contest, emptyQStringList,
                                                 indexOfCorrectAnswer),
               std::invalid_argument);
}

TEST_F(QuestionManagerTest, removeQuestion) {
  secondQuestionManager.clearQuestions();
  secondQuestionManager.addQuestion(firstQuestion);
  secondQuestionManager.addQuestion(secondQuestion);
  secondQuestionManager.addQuestion(secondQuestion);

  ASSERT_NE(secondQuestionManager, questionManager);
  secondQuestionManager.removeQuestion(2);
  EXPECT_EQ(secondQuestionManager, questionManager);
  EXPECT_THROW(secondQuestionManager.removeQuestion(9), std::out_of_range);
}

TEST_F(QuestionManagerTest, changeAnswerTest) {
  const size_t indexOfAnswer{1};
  const QStringList newCorrectAnswers{"2", "8", "4", "5"};

  secondQuestionManager.changeOneAnswer(firstIndex, indexOfAnswer, "8");
  EXPECT_NE(secondQuestionManager, questionManager);
  EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getAnswers(),
            newCorrectAnswers);
  EXPECT_EQ(secondQuestionManager.getQuestion(secondIndex).getAnswers(),
            secondAnswers);
  EXPECT_EQ(secondQuestionManager.getAllQuestions().size(),
            questionManager.getAllQuestions().size());
  EXPECT_THROW(secondQuestionManager.changeOneAnswer(7, 1, "Dummy answer"),
               std::out_of_range);
  EXPECT_THROW(secondQuestionManager.changeOneAnswer(0, 9, "Dummy answer"),
               std::out_of_range);

  secondQuestionManager = QuestionManager(questions);

  secondQuestionManager.changeAllAnswers(firstIndex, newCorrectAnswers);
  EXPECT_NE(secondQuestionManager, questionManager);
  EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getAnswers(),
            newCorrectAnswers);
  EXPECT_EQ(secondQuestionManager.getQuestion(secondIndex).getAnswers(),
            secondAnswers);
  EXPECT_EQ(secondQuestionManager.getAllQuestions().size(),
            questionManager.getAllQuestions().size());
  EXPECT_THROW(secondQuestionManager.changeAllAnswers(7, secondAnswers),
               std::out_of_range);
}

TEST_F(QuestionManagerTest, changeIndexOfCorrectAnswerTest) {
  const uint newIndexOfCorrectAnswer{3};

  secondQuestionManager.changeIndexOfCorrectAnswer(firstIndex,
                                                   newIndexOfCorrectAnswer);
  EXPECT_NE(secondQuestionManager, questionManager);
  EXPECT_EQ(secondQuestionManager.getAllQuestions().size(),
            questionManager.getAllQuestions().size());
  EXPECT_EQ(
      secondQuestionManager.getQuestion(firstIndex).getIndexOfCorrectAnswer(),
      newIndexOfCorrectAnswer);
  EXPECT_THROW(secondQuestionManager.changeIndexOfCorrectAnswer(
                   5, newIndexOfCorrectAnswer),
               std::out_of_range);
}

TEST_F(QuestionManagerTest, changeContentsTest) {
  const QString newContents{"What is 2 + 1?"};
  const QString emptyContent;

  secondQuestionManager.changeContents(firstIndex, newContents);
  EXPECT_NE(secondQuestionManager, questionManager);
  EXPECT_EQ(secondQuestionManager.getAllQuestions().size(),
            questionManager.getAllQuestions().size());
  EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getContents(),
            newContents);
  EXPECT_THROW(secondQuestionManager.changeContents(5, newContents),
               std::out_of_range);
  EXPECT_THROW(secondQuestionManager.changeContents(0, emptyContent),
               std::invalid_argument);
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
  EXPECT_THROW(secondQuestionManager.addQuestion(emptyQuestion),
               std::invalid_argument);
  try {
    secondQuestionManager.addQuestion(emptyQuestion);
  } catch (const std::invalid_argument &e) {
    EXPECT_STREQ(e.what(), "Questions must have contents.");
  }

  emptyQuestion.setContents("Dummy contents");
  EXPECT_THROW(secondQuestionManager.addQuestion(emptyQuestion),
               std::invalid_argument);
  try {
    secondQuestionManager.addQuestion(emptyQuestion);
  } catch (const std::invalid_argument &e) {
    EXPECT_STREQ(e.what(), "Questions must have at least one answer.");
  }

  EXPECT_THROW(emptyQuestionManager.getQuestion(1), std::out_of_range);
  EXPECT_THROW(emptyQuestionManager.changeAllAnswers(7, secondAnswers),
               std::out_of_range);
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

TEST_F(QuestionManagerTest, copyOfAllQuestionsReturnsIndependentCopy) {
  QMap<uint, Question> copy = questionManager.copyOfAllQuestions();

  EXPECT_EQ(copy.size(), questionManager.getAllQuestions().size());
  EXPECT_EQ(copy, questionManager.getAllQuestions());

  copy.remove(firstIndex);

  EXPECT_NE(copy.size(), questionManager.getAllQuestions().size());
  EXPECT_EQ(questionManager.getAllQuestions().size(), questions.size());
  EXPECT_EQ(questionManager.getQuestion(firstIndex), firstQuestion);
}

TEST_F(QuestionManagerTest, findQuestionTest) {
  const auto &question = questionManager.findQuestion(firstIndex);
  EXPECT_EQ(question, firstQuestion);
  EXPECT_THROW(questionManager.findQuestion(999), std::out_of_range);
}

TEST_F(QuestionManagerTest, idGenerationMonotonicityAfterRemovals) {
  secondQuestionManager.clearQuestions();
  secondQuestionManager.addQuestion(firstQuestion);
  secondQuestionManager.addQuestion(secondQuestion);
  secondQuestionManager.addQuestion(secondQuestion);

  secondQuestionManager.removeQuestion(1);

  secondQuestionManager.addQuestion(firstQuestion);

  QMap<uint, Question> all = secondQuestionManager.getAllQuestions();
  QList<uint> keysList = all.keys();
  QSet<uint> keys;
  for (const auto &k : keysList) {
    keys.insert(k);
  }
  EXPECT_EQ(static_cast<int>(keys.size()), all.size());
  EXPECT_GE(all.lastKey(), static_cast<uint>(3));
}

TEST_F(QuestionManagerTest, overflowThrowsWhenNextIdMax) {
  const QMap<uint, Question> base{{0, firstQuestion}};
  QuestionManager qm(base);
  qm.setNextIdForTest(std::numeric_limits<uint>::max());
  EXPECT_THROW(qm.addQuestion(firstQuestion), std::overflow_error);
  EXPECT_THROW(qm.addQuestion(QString{"dummy"}, QStringList{"a"}, 0),
               std::overflow_error);
}
