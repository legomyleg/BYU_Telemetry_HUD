#pragma once
#include <queue>
#include <telemetry/sensor_data.hpp>

using std::queue;

class SampleQueue {
private:
    queue<SensorData> rows;

public:

    SensorData consume_latest() {
        if (rows.empty()) return {};

        SensorData latest;
        while(!rows.empty()) {
            latest = rows.front();
            rows.pop();
        }

        return latest;
    }

    SensorData consume_oldest() {
        auto data = rows.front();
        rows.pop();
        return data;
    }

    SensorData latest() const {
        if (rows.empty()) return {};
        return rows.back();
    }

    void reduce_to_latest() {
        if (rows.empty() || rows.size() == 1) return;

        while (rows.size() > 1) {
            rows.pop();
        }
    }

    void push(SensorData data) {
        rows.push(data);
    }

    bool empty() const {
        return rows.empty();
    }
};
