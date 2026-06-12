#include "telemetry/telemetry_config.hpp"
#include <atomic>
#include <functional>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <opencv2/videoio.hpp>
#include <string_view>
#include <thread>

class FrameBuffer {
private:
    std::mutex mtx;
    cv::Mat shared_frame;
    bool has_new_frame = false;
public:
    void update(const cv::Mat& new_frame) {
        std::lock_guard<std::mutex> lock(mtx);
        new_frame.copyTo(shared_frame);
        has_new_frame = true;
    }
    bool getLatest(cv::Mat& output_frame) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!has_new_frame || shared_frame.empty()) {
            return false;
        }

        output_frame = shared_frame;
        has_new_frame = false;
        return true;
    }
};

void captureThreadWorker(std::string_view url, FrameBuffer& buffer, std::atomic<bool>& keep_running); 

struct RtspFeed {
    FrameBuffer buffer;
    std::atomic<bool> keep_running;
    std::thread worker;

    RtspFeed() : buffer(FrameBuffer{}), keep_running(true), worker(captureThreadWorker, RTSP_URL, std::ref(buffer), std::ref(keep_running)) {}
};

