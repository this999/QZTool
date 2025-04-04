/**
 * @file question.h
 * @brief Declaration of the Question class
 *
 * The Question class represents a single multiple-choice question,
 * containing the question text, a list of possible answers,
 * and the index of the correct answer.
 *
 * It is intended to be used in quizzes, exams, or training applications.
 * The class provides functionality to check if a selected answer is correct.
 *
 * Example usage:
 *   Question q("What is the capital of Poland?", {"Krakow", "Warsaw", "Gdansk"}, 1);
 *   bool correct = q.isCorrect(1); // true
 *
 */

#pragma once

#include <QStringList>

class Question {
public:
    Question() = delete;
    Question(QString contents, QStringList answers, uint indexOfCorrectAnswer);

    void setContents(const QString &contents);
    void setAnswers(const QStringList &answers);
    void setIndexOfCorrectAnswer(const uint indexOfCorrectAnswer);

    QString getContents() const;
    QStringList getAnswers() const;
    uint getIndexOfCorrectAnswer() const;

    bool isAnswerCorrect(uint indexOfAnswer);

private:
    QString contents{""};
    QStringList answers;
    uint indexOfCorrectAnswer{0};
};
