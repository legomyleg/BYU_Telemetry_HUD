#include "state/rocket_state.hpp"
#include "telemetry/sensor_data.hpp"
#include <telemetry/telem_source.hpp>
#include <tests/test_tsrc.hpp>
#include <hud/hud_app.hpp>
#include <gtest/gtest.h>
#include <state/state_update.hpp>

class CoreTests : public ::testing::Test {
protected:

    HudApp app;
    TestingTelemSrc tsrc;

    void SetUp() {}
    void TearDown() {}

    CoreTests() : app(SetupHudApp(2'000'000)), tsrc(TestingTelemSrc{}) {
        app.state.stage = FlightStage::Pad;
    }
};

TEST_F(CoreTests, TotalVelocityMatchesExpected) {
    // Add init sample
    uint64_t t = 1'000'000;
    SensorData d;
    d.t_us = t;
    tsrc.insert_data(d);

    //Get up to speed
    uint64_t increment_t = 1'000'000;
    int total_secs = 2;
    int total_increments = (total_secs * 1'000'000) / increment_t;

    float ax = 50.0f;

    for (int i = 0; i < total_increments; i++) {
        t += increment_t;
        SensorData d{};
        d.t_us = t;
        d.ax = ax;
        tsrc.insert_data(d);
    }

    float expected_tv = ax * total_secs;
    UpdateState(app, app.sample_queue, tsrc);

    ASSERT_EQ(app.state.total_velocity, expected_tv);
}
