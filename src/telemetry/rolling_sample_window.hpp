#pragma once
#include "sensor_data.hpp"
#include "telemetry_config.hpp"
#ifndef WINDOW_SIZE
#define WINDOW_SIZE 120
#endif

class RollingSampleWindow {
private:
    SensorData window[WINDOW_SIZE];
    int next;
    int count;
public:
  void add_sample(SensorData sample) {
      window[next] = sample;
      next = (next + 1) % WINDOW_SIZE;
      if (count < WINDOW_SIZE) {
          count++;
      }
  }

  bool full() {
      return count == WINDOW_SIZE;
  }

  bool empty() {
      return count == 0;
  }

  int size() const {
      return count;
  }
};