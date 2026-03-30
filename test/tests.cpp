// Copyright 2021 GHA Test Team
#include "TimedDoor.h"
#include <chrono>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <thread>

class MockTimerClient : public TimerClient {
public:
  MOCK_METHOD(void, Timeout, (), (override));
};

class TimedDoorTest : public ::testing::Test {
protected:
  void SetUp() override { door = new TimedDoor(5); }

  void TearDown() override { delete door; }

  TimedDoor *door;
};

TEST_F(TimedDoorTest, InitialStateIsClosed) {
  EXPECT_FALSE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, LockAndUnlockChangeState) {
  door->unlock();
  EXPECT_TRUE(door->isDoorOpened());
  door->lock();
  EXPECT_FALSE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, GetTimeOutReturnsCorrectValue) {
  EXPECT_EQ(door->getTimeOut(), 5);
}

TEST_F(TimedDoorTest, ThrowStateThrowsWhenOpen) {
  door->unlock();
  EXPECT_THROW(door->throwState(), std::runtime_error);
}

TEST_F(TimedDoorTest, ThrowStateNoThrowWhenClosed) {
  door->lock();
  EXPECT_NO_THROW(door->throwState());
}

TEST_F(TimedDoorTest, DoorTimerAdapterTimeoutOnOpenDoorThrows) {
  door->unlock();
  DoorTimerAdapter adapter(*door);
  EXPECT_THROW(adapter.Timeout(), std::runtime_error);
}

TEST_F(TimedDoorTest, DoorTimerAdapterTimeoutOnClosedDoorNoThrow) {
  door->lock();
  DoorTimerAdapter adapter(*door);
  EXPECT_NO_THROW(adapter.Timeout());
}

TEST(TimerTest, TimerCallsTimeoutAfterDelay) {
  Timer timer;
  MockTimerClient mockClient;
  EXPECT_CALL(mockClient, Timeout()).Times(1);
  timer.tregister(0, &mockClient);
  // Allow the detached thread to execute
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

TEST_F(TimedDoorTest, UnlockAlreadyOpenDoesNothing) {
  door->unlock();
  EXPECT_TRUE(door->isDoorOpened());
  door->unlock(); // second unlock should not change state
  EXPECT_TRUE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, LockAfterOpenClosesDoor) {
  door->unlock();
  EXPECT_TRUE(door->isDoorOpened());
  door->lock();
  EXPECT_FALSE(door->isDoorOpened());
}
