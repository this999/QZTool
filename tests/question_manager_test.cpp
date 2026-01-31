#include <gtest/gtest.h>

#include "QZToolCore/question_manager.h"

static QVector<QString> *capturedMessages = nullptr;

static void testMessageHandler(QtMsgType, const QMessageLogContext&, const QString &msg) {
    if (capturedMessages) capturedMessages->append(msg);
}

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
    QVector<QString> messages;

    void SetUp() override {
        secondQuestionManager = QuestionManager(questions);
        messages.clear();
        capturedMessages = &messages;
        qInstallMessageHandler(testMessageHandler);
    }

    void TearDown() override {
        qInstallMessageHandler(nullptr);
        capturedMessages = nullptr;
    }
};

TEST_F(QuestionManagerTest, changeContentsLogsOnInvalid) {
    const QString emptyContent;
    secondQuestionManager.changeContents(0, emptyContent);


    EXPECT_FALSE(messages.isEmpty());
    bool found = false;
    for (const auto &msg : messages) {
        if (msg.contains("Question contents cannot be empty") || msg.contains("Error:")) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

// TEST_F(QuestionManagerTest, addQuestionLogsOnInvalid) {
//     Question emptyQuestion;
//     EXPECT_NO_THROW(secondQuestionManager.addQuestion(emptyQuestion));


//     EXPECT_FALSE(messages.isEmpty());
//     bool found = false;
//     for (const auto &msg : messages) {
//         if (msg.contains("Questions must") || msg.contains("cannot") || msg.contains("Error:")) {
//             found = true;
//             break;
//         }
//     }
//     EXPECT_TRUE(found);
// }

TEST_F(QuestionManagerTest, changeOneAnswerLogsOnInvalid) {
    secondQuestionManager.clearQuestions();
    secondQuestionManager.addQuestion(firstQuestion);

    secondQuestionManager.changeOneAnswer(7, 0, "x");
    secondQuestionManager.changeOneAnswer(0, 9, "y");

    EXPECT_FALSE(messages.isEmpty());
    bool found = false;
    for (const auto &msg : messages) {
        if (msg.contains("out of range") || msg.contains("invalid") || msg.contains("Error:")) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

TEST_F(QuestionManagerTest, changeAllAnswersLogsOnInvalid) {
    secondQuestionManager.clearQuestions();
    secondQuestionManager.addQuestion(firstQuestion);

    secondQuestionManager.changeAllAnswers(9, {"a", "b"});
    secondQuestionManager.changeAllAnswers(0, QStringList{});

    EXPECT_FALSE(messages.isEmpty());
    bool found = false;
    for (const auto &msg : messages) {
        if (msg.contains("must have at least one answer") || msg.contains("invalid") || msg.contains("Error:")) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

TEST_F(QuestionManagerTest, changeIndexOfCorrectAnswerLogsOnInvalid) {
    secondQuestionManager.clearQuestions();
    secondQuestionManager.addQuestion(firstQuestion);

    secondQuestionManager.changeIndexOfCorrectAnswer(0, 99);
    secondQuestionManager.changeIndexOfCorrectAnswer(9, 0);

    EXPECT_FALSE(messages.isEmpty());
    bool found = false;
    for (const auto &msg : messages) {
        if (msg.contains("out of range") || msg.contains("invalid") || msg.contains("Error:")) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

// TEST_F(QuestionManagerTest, removeQuestionLogsOnInvalid) {
//     secondQuestionManager.clearQuestions();
//     secondQuestionManager.addQuestion(firstQuestion);

//     EXPECT_FALSE(secondQuestionManager.removeQuestion(9));

//     EXPECT_FALSE(messages.isEmpty());
//     bool found = false;
//     for (const auto &msg : messages) {
//         if (msg.contains("Error:") || msg.contains("not found") || msg.contains("invalid")) {
//             found = true;
//             break;
//         }
//     }
//     EXPECT_TRUE(found);
// }

TEST_F(QuestionManagerTest, addQuestionTest) {
    secondQuestionManager.clearQuestions();
    secondQuestionManager.addQuestion(contest, answers, indexOfCorrectAnswer);
    secondQuestionManager.addQuestion(secondQuestion);

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

    secondQuestionManager.changeOneAnswer(firstIndex, indexOfAnswer, "8");
    EXPECT_NE(secondQuestionManager, questionManager);
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getAnswers(), newCorrectAnswers);
    EXPECT_EQ(secondQuestionManager.getQuestion(secondIndex).getAnswers(), secondAnswers);
    EXPECT_EQ(secondQuestionManager.getAllQuestions().size(), questionManager.getAllQuestions().size());
    EXPECT_NO_THROW(secondQuestionManager.changeOneAnswer(7, 1, "Dummy answer"));
    EXPECT_NO_THROW(secondQuestionManager.changeOneAnswer(0, 9, "Dummy answer"));

    secondQuestionManager = QuestionManager(questions);

    secondQuestionManager.changeAllAnswers(firstIndex, newCorrectAnswers);
    EXPECT_NE(secondQuestionManager, questionManager);
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getAnswers(), newCorrectAnswers);
    EXPECT_EQ(secondQuestionManager.getQuestion(secondIndex).getAnswers(), secondAnswers);
    EXPECT_EQ(secondQuestionManager.getAllQuestions().size(), questionManager.getAllQuestions().size());
    EXPECT_NO_THROW(secondQuestionManager.changeAllAnswers(7, secondAnswers));
}

TEST_F(QuestionManagerTest, changeIndexOfCorrectAnswerTest) {
    const uint newIndexOfCorrectAnswer{3};

    secondQuestionManager.changeIndexOfCorrectAnswer(firstIndex, newIndexOfCorrectAnswer);
    EXPECT_NE(secondQuestionManager, questionManager);
    EXPECT_EQ(secondQuestionManager.getAllQuestions().size(), questionManager.getAllQuestions().size());
    EXPECT_EQ(secondQuestionManager.getQuestion(firstIndex).getIndexOfCorrectAnswer(), newIndexOfCorrectAnswer);
    EXPECT_NO_THROW(secondQuestionManager.changeIndexOfCorrectAnswer(5, newIndexOfCorrectAnswer));
}

TEST_F(QuestionManagerTest, changeContentsTest) {
    const QString newContents{"What is 2 + 1?"};
    const QString emptyContent;

    secondQuestionManager.changeContents(firstIndex, newContents);
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
