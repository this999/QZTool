#include "question_manager.h"
#include <QDebug>

QuestionManager::QuestionManager(QMap<uint, Question> questions) : questions(questions) {
    if (questions.isEmpty()) {
        throw std::invalid_argument("Questions must have at least one question.");
    }
}

void QuestionManager::addQuestion(const Question &question) {
    if (question.getContents().isEmpty()) {
        throw std::invalid_argument("Questions must have contents.");
    }
    else if (question.getAnswers().isEmpty()) {
        throw std::invalid_argument("Questions must have at least one answer.");
    }
    else {
        questions.insert(questions.isEmpty() ? 0 : questions.lastKey() + 1, question);
    }
}

void QuestionManager::addQuestion(const QString& questionContents, const QStringList& questionAnswers, const uint questionIndexOfCorrectAnswer) {
    try {
        questions.insert(questions.isEmpty() ? 0 : questions.lastKey() + 1, Question(questionContents, questionAnswers, questionIndexOfCorrectAnswer));
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: " << e.what();
    }
    catch (const std::invalid_argument& e) {
        qDebug() << "Error: " << e.what();
    }
}

bool QuestionManager::removeQuestion(const uint index) noexcept{
    if (questions.contains(index)) {
        questions.remove(index);
        return true;
    }
    else {
        return false;
    }
}

void QuestionManager::changeOneAnswer(const uint indexOfQuestion ,const size_t indexOfAnswer, const QString &newAnswer) {
    try {
        auto question{findQuestion(indexOfQuestion)};
        question.value().changeAnswer(indexOfAnswer, newAnswer);
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: " << e.what();
    }
}

void QuestionManager::changeAllAnswers(const uint indexOfQuestion, const QStringList &newAnswers) {
    try {
        auto question{findQuestion(indexOfQuestion)};
        question.value().setAnswers(newAnswers);
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: " << e.what();
    }

    catch (const std::invalid_argument& e) {
        qDebug() << "Error: " << e.what();
    }
}

void QuestionManager::changeIndexOfCorrectAnswer(const uint indexOfQuestion, const uint newIndexOfCorrectAnswer) {
    try {
        auto question{findQuestion(indexOfQuestion)};
        question.value().setIndexOfCorrectAnswer(newIndexOfCorrectAnswer);
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: " << e.what();
    }
    catch (const std::logic_error& e) {
        qDebug() << "Error: " << e.what();
    }
}

void QuestionManager::changeContents(const uint indexOfQuestion, const QString newContents) {
    try {
        auto question{findQuestion(indexOfQuestion)};
        question.value().setContents(newContents);
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: " << e.what();
    }
    catch (const std::invalid_argument& e) {
        qDebug() <<"Error: " << e.what();
    }
}

QMap<uint, Question> QuestionManager::getAllQuestions() const noexcept{
    return questions;
}

Question QuestionManager::getQuestion(const uint indexOfQuestion) const {
    if (questions.contains(indexOfQuestion)) {
        return questions[indexOfQuestion];
    }
    else {
        throw std::out_of_range("Question not found in the map");
    }
}

QMap<uint, Question>::iterator QuestionManager::findQuestion(const uint indexOfQuestion) {
    auto it{questions.find(indexOfQuestion)};

    if (it != questions.end()) {
        return it;
    }
    else {
        throw std::out_of_range("Question not found in the map");
    }
    // Poprawić to niech zwraca wartość z iteratora
}

bool QuestionManager::operator==(const QuestionManager& other) const noexcept{
    return questions == other.questions;
}

bool QuestionManager::operator!=(const QuestionManager& other) const noexcept{
    return questions != other.questions;
}

void QuestionManager::clearQuestions() noexcept{
    questions.clear();
}
