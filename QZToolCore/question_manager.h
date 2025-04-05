/**
 * @file question_manager.h
 * @brief Declaration of the question management class
 *
 * The QuestionManager class is responsible for managing a collection of questions.
 * It provides functionality to add, remove, edit, and retrieve questions,
 * as well as managing question data in a database or from external files.
 *
 * Example usage:
 *   QuestionManager qm;
 *   qm.addQuestion("What is the capital of Poland?", {"Krakow", "Warsaw", "Gdansk"}, 1);
 *
 */

#pragma once

#include "question.h"

#include <QMap>

class QuestionManager{
public:
    QuestionManager() = default;
    QuestionManager(QMap<uint, Question> questions);
    bool operator==(const QuestionManager& other) const;
    bool operator!=(const QuestionManager& other) const;

    void addQuestion(const Question &question);
    void addQuestion(const QString& questionContents, const QStringList& questionAnswers, const uint questionIndexOfCorrectAnswer);
    void removeQuestion(const uint index);
    void changeOneAnswer(const uint indexOfQuestion ,const size_t indexOfAnswer, const QString &newAnswer);
    void changeAllAnswers(const uint indexOfQuestion, const QStringList &newAnswers);
    void changeIndexOfCorrectAnswer(const uint indexOfQuestion, const uint newIndexOfCorrectAnswer);
    void changeContents(const uint indexOfQuestion, const QString newContents);
    void clearQuestions();

    QMap<uint, Question> getAllQuestions() const;
    Question getQuestion(const uint indexOfQuestion) const;

private:
    QMap<uint, Question> questions;
    QMap<uint, Question>::iterator findQuestion(const uint indexOfQuestion);
};
