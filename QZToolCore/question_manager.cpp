#include "question_manager.h"
#include <QDebug>

QuestionManager::QuestionManager(QMap<uint, Question> questions) : questions(questions) {
    if (questions.isEmpty()) {
        throw std::invalid_argument("Questions must have at least one question.");
    }
}

bool QuestionManager::addQuestion(const Question &question) {
    bool success{false};
    if (question.getContents().isEmpty()) {
        throw std::invalid_argument("Questions must have contents.");
    }
    else if (question.getAnswers().isEmpty()) {
        throw std::invalid_argument("Questions must have at least one answer.");
    }
    else {
        questions.insert(questions.isEmpty() ? 0 : questions.lastKey() + 1, question);
        success = true;
    }
    return success;
}

bool QuestionManager::addQuestion(const QString& questionContents, const QStringList& questionAnswers, const uint questionIndexOfCorrectAnswer) {
    bool success{false};
    try {
        questions.insert(questions.isEmpty() ? 0 : questions.lastKey() + 1, Question(questionContents, questionAnswers, questionIndexOfCorrectAnswer));
        success = true;
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: " << e.what();
    }
    catch (const std::invalid_argument& e) {
        qDebug() << "Error: " << e.what();
    }
    return success;
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

bool QuestionManager::changeOneAnswer(const uint indexOfQuestion ,const size_t indexOfAnswer, const QString &newAnswer) {
    bool success{false};
    try {
        auto question{findQuestion(indexOfQuestion)};
        question.value().changeAnswer(indexOfAnswer, newAnswer);
        success = true;
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: " << e.what();
    }
    return success;
}

bool QuestionManager::changeAllAnswers(const uint indexOfQuestion, const QStringList &newAnswers) {
    bool success{false};
    try {
        auto question{findQuestion(indexOfQuestion)};
        question.value().setAnswers(newAnswers);
        success = true;
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: " << e.what();
    }

    catch (const std::invalid_argument& e) {
        qDebug() << "Error: " << e.what();
    }
    return success;
}

bool QuestionManager::changeIndexOfCorrectAnswer(const uint indexOfQuestion, const uint newIndexOfCorrectAnswer) {
    bool success{false};
    try {
        auto question{findQuestion(indexOfQuestion)};
        question.value().setIndexOfCorrectAnswer(newIndexOfCorrectAnswer);
        success = true;
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: " << e.what();
    }
    return success;
}

bool QuestionManager::changeContents(const uint indexOfQuestion, const QString newContents) {
    bool success{false};
    try {
        auto question{findQuestion(indexOfQuestion)};
        question.value().setContents(newContents);
        success = true;
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: " << e.what();
    }
    catch (const std::invalid_argument& e) {
        qDebug() <<"Error: " << e.what();
    }
    return success;
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
