#include <gtest/gtest.h>
#include <telemetry/sample_ring_buffer.hpp>

class CoreTests : public ::testing::Test {
protected:
    SampleRingBuffer buffer;

    CoreTests() : buffer(2'000'000) {}

    void SetUp() {}

    void TearDown() {}
};

TEST_F(CoreTests, NewBufferIsEmpty) {
    ASSERT_TRUE(buffer.empty() == true);
    ASSERT_TRUE(buffer.size() == 0);
    buffer.add_sample(SensorData{});
}

TEST_F(CoreTests, OneSample) {
    ASSERT_EQ(buffer.size(), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
