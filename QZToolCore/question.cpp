#include "question.h"


Question::Question(QString contents, QStringList answers, uint indexOfCorrectAnswer) : contents(contents), answers(answers), indexOfCorrectAnswer(indexOfCorrectAnswer) {

    if (answers.isEmpty()) {
        throw std::invalid_argument("Question must have at least one answer.");
    }

    if (indexOfCorrectAnswer >= answers.size()) {
        throw std::out_of_range("Correct answer index is invalid.");
    }
}

void Question::setContents(const QString &contents) {
    this->contents = contents;
}

void Question::setAnswers(const QStringList &answers) {
    if (answers.isEmpty()) {
        throw std::invalid_argument("Question must have at least one answer.");
    }
    this->answers = answers;
}

void Question::setIndexOfCorrectAnswer(const uint indexOfCorrectAnswer) {
    this->indexOfCorrectAnswer = indexOfCorrectAnswer;
}

QString Question::getContents() const {
    return contents;
}

QStringList Question::getAnswers() const {
    return answers;
}

uint Question::getIndexOfCorrectAnswer() const {
    return indexOfCorrectAnswer;
}

bool Question::isAnswerCorrect(uint indexOfAnswer) {
    return indexOfCorrectAnswer == indexOfAnswer;
}
