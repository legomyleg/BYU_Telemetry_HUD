#pragma once
#include "sensor_data.hpp"
#include "raylib.h"
#ifndef WINDOW_SIZE
#define WINDOW_SIZE 60
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

    bool empty() const {
      return count == 0;
    }

    int size() const {
      return count;
    }

    Vector3 avg_accel() const {
      if (empty()) {
          return {0, 0, 0};
      }

      float sum_x = 0.0f;
      float sum_y = 0.0f;
      float sum_z = 0.0f;

      for (int i=0; i < count; i++) {
          sum_x += window[i].ax;
          sum_y += window[i].ay;
          sum_z += window[i].az;
      }

      return {sum_x / count, sum_y / count, sum_z / count};
    }

    Vector3 avg_gyro() const {
      if (empty()) {
          return {0, 0, 0};
      }

      float sum_x = 0.0f;
      float sum_y = 0.0f;
      float sum_z = 0.0f;

      for (int i=0; i < count; i++) {
          sum_x += window[i].gx;
          sum_y += window[i].gy;
          sum_z += window[i].gz;
      }

      return {sum_x / count, sum_y / count, sum_z / count};
    }

    float avg_alt_m() const {
        if (empty()) {
            return 0;
        }

        float sum_alt = 0;
        for (int i=0; i < count; i++) {
            sum_alt += window[i].altM;
        }

        return sum_alt / count;
    }

    int percent_full() const {
      float p = (float)count / (float)WINDOW_SIZE;
      return static_cast<int>(p * 100.0f);
    }
};
