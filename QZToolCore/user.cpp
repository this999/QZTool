#include "user.h"

User::User(const QString &username, const QDateTime &createdAt)
    : username(username), createdAt(createdAt) {}

QString User::getUsername() const noexcept {
    return username;
}

QDateTime User::getCreatedAt() const noexcept {
    return createdAt;
}

void User::setUsername(const QString &username) {
    this->username = username;
}

void User::setCreatedAt(const QDateTime &createdAt) {
    this->createdAt = createdAt;
}

bool User::operator==(const User &other) const noexcept {
    return username == other.username && createdAt == other.createdAt;
}

bool User::operator!=(const User &other) const noexcept {
    return !(*this == other);
}
