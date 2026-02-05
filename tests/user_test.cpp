#include "user.h"
#include <gtest/gtest.h>

class UserTest : public ::testing::Test {
protected:
  User user{"test_user", QDateTime::currentDateTimeUtc()};
  User user2{"another_user", QDateTime::currentDateTimeUtc().addSecs(-3600)};
};

TEST_F(UserTest, GettersReturnCorrectValues) {
  EXPECT_EQ(user.getUsername(), "test_user");
  EXPECT_EQ(user.getCreatedAt().date(), QDateTime::currentDateTimeUtc().date());
}

TEST_F(UserTest, SettersUpdateValues) {
  user.setUsername("new_user");
  user.setCreatedAt(QDateTime::currentDateTimeUtc().addDays(1));

  EXPECT_EQ(user.getUsername(), "new_user");
  EXPECT_EQ(user.getCreatedAt().date(),
            QDateTime::currentDateTimeUtc().addDays(1).date());
}

TEST_F(UserTest, EqualityOperatorWorks) {
  EXPECT_TRUE(user == user);
  EXPECT_FALSE(user == user2);
}

TEST_F(UserTest, InequalityOperatorWorks) {
  EXPECT_FALSE(user != user);
  EXPECT_TRUE(user != user2);
}
