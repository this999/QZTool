/**
 * @file session.h
 * @brief Declaration of the Session class
 *
 * This class represents a user session in the system.
 * It contains information about the user's authentication state,
 * active connections, and session metadata.
 * The Session class provides methods for managing user sessions
 * and interactions.
 *
 * Example usage:
 */

#pragma once

#include <QDateTime>
#include <QString>

class Session {
public:
  Session() = default;
  Session(const QString &sessionId,
          const QDateTime &createdAt = QDateTime::currentDateTimeUtc());

  QString getSessionId() const noexcept;
  QDateTime getCreatedAt() const noexcept;
  void setSessionId(const QString &sessionId);
  void setCreatedAt(const QDateTime &createdAt);

  bool operator==(const Session &other) const noexcept;
  bool operator!=(const Session &other) const noexcept;

private:
  QString sessionId;
  QDateTime createdAt;
};