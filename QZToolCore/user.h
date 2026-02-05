/**
 * @file user.h
 * @brief Declaration of the User class
 *
 * This class represents a user in the system.
 * It contains information about the user's profile, preferences, and activity.
 * The User class provides methods for managing user data and interactions.
 *
 * Example usage:
 */

#pragma once

#include <QDateTime>
#include <QString>

class User {
public:
  User() = default;
  User(const QString &username,
       const QDateTime &createdAt = QDateTime::currentDateTimeUtc());

  QString getUsername() const noexcept;
  QDateTime getCreatedAt() const noexcept;
  void setUsername(const QString &username);
  void setCreatedAt(const QDateTime &createdAt);

  bool operator==(const User &other) const noexcept;
  bool operator!=(const User &other) const noexcept;

private:
  QString username;
  QDateTime createdAt;
};