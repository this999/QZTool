#include "question.h"


Question::Question(QString contents, QStringList answers, uint indexOfCorrectAnswer) : contents(contents), answers(answers), indexOfCorrectAnswer(indexOfCorrectAnswer) {

    if (answers.isEmpty()) {
        throw std::invalid_argument("Question must have at least one answer.");
    }

    if (indexOfCorrectAnswer >= answers.size()) {
        throw std::out_of_range("Correct answer index is invalid.");
    }
}

void Question::setContents(const QString &contents) noexcept {
    this->contents = contents;
}

void Question::setAnswers(const QStringList &answers) {
    if (answers.isEmpty()) {
        throw std::invalid_argument("Question must have at least one answer.");
    }
    this->answers = answers;
}

void Question::setIndexOfCorrectAnswer(const uint indexOfCorrectAnswer) noexcept {
    this->indexOfCorrectAnswer = indexOfCorrectAnswer;
}

void Question::changeAnswer(const size_t index, const QString &newAnswer) {
    if (index < answers.size()) {
        answers[index] = newAnswer;
    }
    else {
        throw std::out_of_range("Invalid answer index");
    }
}

QString Question::getContents() const noexcept{
    return contents;
}

QStringList Question::getAnswers() const noexcept{
    return answers;
}

uint Question::getIndexOfCorrectAnswer() const noexcept{
    return indexOfCorrectAnswer;
}

bool Question::isAnswerCorrect(const uint indexOfAnswer) const noexcept{
    return indexOfCorrectAnswer == indexOfAnswer;
}

bool Question::operator==(const Question& other) const noexcept{
    return (contents == other.contents) &&
           (answers == other.answers)   &&
           (indexOfCorrectAnswer == other.indexOfCorrectAnswer);
}

bool Question::operator!=(const Question& other) const noexcept{
    return (contents != other.contents) &&
           (answers != other.answers)   &&
           (indexOfCorrectAnswer != other.indexOfCorrectAnswer);
}
