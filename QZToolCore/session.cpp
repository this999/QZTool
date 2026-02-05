#include "session.h"

Session::Session(const QString &sessionId, const QDateTime &createdAt)
    : sessionId(sessionId), createdAt(createdAt) {}

QString Session::getSessionId() const noexcept { return sessionId; }

QDateTime Session::getCreatedAt() const noexcept { return createdAt; }

void Session::setSessionId(const QString &sessionId) {
  this->sessionId = sessionId;
}

void Session::setCreatedAt(const QDateTime &createdAt) {
  this->createdAt = createdAt;
}

bool Session::operator==(const Session &other) const noexcept {
  return sessionId == other.sessionId && createdAt == other.createdAt;
}

bool Session::operator!=(const Session &other) const noexcept {
  return !(*this == other);
}
