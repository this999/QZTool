#include "question_manager.h"

QuestionManager::QuestionManager(QMap<uint, Question> questions) : questions(questions) {
    if (questions.isEmpty()) {
        throw std::invalid_argument("Questions must have at least one question.");
    }
}

void QuestionManager::addQuestion(const Question &question) {
    questions.insert(questions.isEmpty() ? 0 : questions.lastKey() + 1, question);
}

void QuestionManager::addQuestion(const QString& questionContents, const QStringList& questionAnswers, const uint questionIndexOfCorrectAnswer) {
    questions.insert(questions.isEmpty() ? 0 : questions.lastKey() + 1, Question(questionContents, questionAnswers, questionIndexOfCorrectAnswer));
    //TODO
    //Handling of Exceptions
}

void QuestionManager::removeQuestion(const uint index) {
    questions.remove(index);
}

void QuestionManager::changeOneAnswer(const uint indexOfQuestion ,const size_t indexOfAnswer, const QString &newAnswer) {
    auto question{findQuestion(indexOfQuestion)};
    question.value().changeAnswer(indexOfAnswer, newAnswer);
}

void QuestionManager::changeAllAnswers(const uint indexOfQuestion, const QStringList &newAnswers) {
    auto question{findQuestion(indexOfQuestion)};
    question.value().setAnswers(newAnswers);

    //TODO
    //Handling of Exceptions
}

void QuestionManager::changeIndexOfCorrectAnswer(const uint indexOfQuestion, const uint newIndexOfCorrectAnswer) {
    auto question{findQuestion(indexOfQuestion)};
    question.value().setIndexOfCorrectAnswer(newIndexOfCorrectAnswer);
}

void QuestionManager::changeContents(const uint indexOfQuestion, const QString newContents) {
    auto question{findQuestion(indexOfQuestion)};
    question.value().setContents(newContents);
}

QMap<uint, Question> QuestionManager::getAllQuestions() const {
    return questions;
}

Question QuestionManager::getQuestion(const uint indexOfQuestion) const {

    if (questions.contains(indexOfQuestion)) {
        return questions[indexOfQuestion];
    }
    else {
        throw std::out_of_range("Question not found in the map");
    }
    //TODO
    //Handling of Exceptions
}

QMap<uint, Question>::iterator QuestionManager::findQuestion(const uint indexOfQuestion) {
    auto it{questions.find(indexOfQuestion)};

    if (it != questions.end()) {
        return it;
    }
    else {
        throw std::out_of_range("Question not found in the map");
    }
    //TODO
    //Handling of Exceptions
}

bool QuestionManager::operator==(const QuestionManager& other) const {
    return questions == other.questions;
}

bool QuestionManager::operator!=(const QuestionManager& other) const {
    return questions != other.questions;
}

void QuestionManager::clearQuestions() {
    questions.clear();
}

