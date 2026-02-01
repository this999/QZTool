#include "question_manager.h"
#include <stdexcept>

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
    if (questionContents.isEmpty()) {
        throw std::invalid_argument("Questions must have contents.");
    }
    else if (questionAnswers.isEmpty()) {
        throw std::invalid_argument("Questions must have at least one answer.");
    }
    else if (questionIndexOfCorrectAnswer >= static_cast<uint>(questionAnswers.size())) {
        throw std::out_of_range("Index of correct answer is out of range");
    }
    else {
        questions.insert(questions.isEmpty() ? 0 : questions.lastKey() + 1,
                         Question(questionContents, questionAnswers, questionIndexOfCorrectAnswer));
    }
}

void QuestionManager::removeQuestion(const uint index){
    if (questions.contains(index)) {
        questions.remove(index);
    }
    else {
        throw std::out_of_range("Question not found");
    }
}

void QuestionManager::changeOneAnswer(const uint indexOfQuestion ,const uint indexOfAnswer, const QString &newAnswer) {
    auto &question{findQuestion(indexOfQuestion)};
    question.changeAnswer(indexOfAnswer, newAnswer);
}

void QuestionManager::changeAllAnswers(const uint indexOfQuestion, const QStringList &newAnswers) {
    auto &question{findQuestion(indexOfQuestion)};
    question.setAnswers(newAnswers);

}

void QuestionManager::changeIndexOfCorrectAnswer(const uint indexOfQuestion, const uint newIndexOfCorrectAnswer) {
    auto &question{findQuestion(indexOfQuestion)};
    question.setIndexOfCorrectAnswer(newIndexOfCorrectAnswer);
}

void QuestionManager::changeContents(const uint indexOfQuestion, const QString newContents) {
    auto &question{findQuestion(indexOfQuestion)};
    question.setContents(newContents);
}

const QMap<uint, Question>& QuestionManager::getAllQuestions() const noexcept{
    return questions;
}

QMap<uint, Question> QuestionManager::copyOfAllQuestions() const{
    return questions;
}

Question QuestionManager::getQuestion(const uint indexOfQuestion) const {
    if (questions.contains(indexOfQuestion)) {
        return questions[indexOfQuestion];
    }
    else {
        throw std::out_of_range("Question not found");
    }
}

Question& QuestionManager::findQuestion(const uint indexOfQuestion) {
    auto it {questions.find(indexOfQuestion)};
    if (it == questions.end()) {
        throw std::out_of_range("Question not found");
    }
    return it.value();
}

const Question& QuestionManager::findQuestion(const uint indexOfQuestion) const {
    auto it{questions.find(indexOfQuestion)};
    if (it == questions.end()) {
        throw std::out_of_range("Question not found");
    }
    return it.value();
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
