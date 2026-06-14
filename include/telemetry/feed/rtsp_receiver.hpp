#pragma once
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

        output_frame.copyTo(shared_frame);
        has_new_frame = false;
        return true;
    }
};

void captureThreadWorker(std::string_view url, FrameBuffer& buffer, std::atomic<bool>& keep_running); 
void videoFileThreadWorker(double start_sec, std::string_view file_path, FrameBuffer& buffer, std::atomic<bool>& keep_running); 

enum class FeedType {
    Rtsp,
    VideoFile
};

struct CameraFeedConfig {
    FeedType type;
    const std::string_view source;
    double start_sec = 0;
};

struct CameraFeed {
    FrameBuffer buffer{};
    std::atomic<bool> keep_running = true;
    std::thread worker;

    CameraFeed(CameraFeedConfig config) {
        if (config.type == FeedType::Rtsp) {
            worker = std::thread(captureThreadWorker, config.source, std::ref(buffer), std::ref(keep_running));
        } else {
            worker = std::thread(videoFileThreadWorker, config.start_sec, config.source, std::ref(buffer), std::ref(keep_running));
        }
    }

    CameraFeed(const CameraFeed&) = delete;
    CameraFeed& operator=(const CameraFeed&) = delete;

    ~CameraFeed() {
        keep_running = false;
        if (worker.joinable()) {
            worker.join();
        }
    }
};
