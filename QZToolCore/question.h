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
    Question() = default;
    Question(QString contents, QStringList answers, uint indexOfCorrectAnswer);

    bool operator==(const Question& other) const noexcept;
    bool operator!=(const Question& other) const noexcept;

    void setContents(const QString &contents);
    void setAnswers(const QStringList &answers);
    void setIndexOfCorrectAnswer(const uint indexOfCorrectAnswer);
    void changeAnswer(const size_t index, const QString &newAnswer);

    QString getContents() const noexcept;
    QStringList getAnswers() const noexcept;
    uint getIndexOfCorrectAnswer() const noexcept;

    bool isAnswerCorrect(const uint indexOfAnswer) const noexcept;

private:
    QString contents{""};
    QStringList answers;
    uint indexOfCorrectAnswer{0};
};
