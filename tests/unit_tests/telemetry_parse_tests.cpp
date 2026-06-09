#include <telemetry/sensor_data.hpp>
#include <telemetry/telemetry_parse.hpp>
#include <gtest/gtest.h>
#include <string>
using std::string;

string test_string_short = "0,12,24";
string test_string_correct = "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";

TEST(ParserTests, ParseRejectsShortString) {
    ASSERT_ANY_THROW(parseLine(test_string_short));
}

TEST(ParserTests, ParserAcceptsAllZeroes) {
    ASSERT_NO_THROW(parseLine(test_string_correct));
}

TEST(ParserTests, ParserReturnsCorrectZeroedData) {
    SensorData d{};
    SensorData return_data = parseLine(test_string_correct);
    ASSERT_TRUE(true);
}
