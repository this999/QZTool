#include "session.h"
#include <gtest/gtest.h>

class SessionTest : public ::testing::Test {
protected:
    Session session{"test_session", QDateTime::currentDateTimeUtc()};
    Session session2{"another_session", QDateTime::currentDateTimeUtc().addSecs(-3600)};
};

TEST_F(SessionTest, GettersReturnCorrectValues) {
    EXPECT_EQ(session.getSessionId(), "test_session");
    EXPECT_EQ(session.getCreatedAt().date(), QDateTime::currentDateTimeUtc().date());
}

TEST_F(SessionTest, SettersUpdateValues) {
    session.setSessionId("new_session");
    session.setCreatedAt(QDateTime::currentDateTimeUtc().addDays(1));

    EXPECT_EQ(session.getSessionId(), "new_session");
    EXPECT_EQ(session.getCreatedAt().date(), QDateTime::currentDateTimeUtc().addDays(1).date());
}

TEST_F(SessionTest, EqualityOperatorWorks) {
    EXPECT_TRUE(session == session);
    EXPECT_FALSE(session == session2);
}

TEST_F(SessionTest, InequalityOperatorWorks) {
    EXPECT_FALSE(session != session);
    EXPECT_TRUE(session != session2);
}
