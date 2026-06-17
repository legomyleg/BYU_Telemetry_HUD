#include <chrono>
#include <opencv2/videoio.hpp>
#include <string>
#include <string_view>
#include <telemetry/feed/rtsp_receiver.hpp>
#include <iostream>
#include <thread>

cv::VideoCapture openRtsp(std::string_view url) {
    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;tcp", 1);
    return cv::VideoCapture(std::string(url), cv::CAP_FFMPEG, {
            cv::CAP_PROP_OPEN_TIMEOUT_MSEC, 3'000,
            cv::CAP_PROP_READ_TIMEOUT_MSEC, 1'000,
            cv::CAP_PROP_BUFFERSIZE, 1
    });
}

void captureThreadWorker(std::string_view url, FrameBuffer& buffer, std::atomic<bool>& keep_running) {
    auto cap = openRtsp(url);
    cv::Mat frame;

    int bad_reads = 0;
    auto last_good = std::chrono::steady_clock::now();

    while (keep_running) {
        if (!cap.isOpened()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            cap = openRtsp(url);
            continue;
        }

        bool ok = cap.read(frame);
        if (ok && !frame.empty()) {
            bad_reads = 0;
            last_good = std::chrono::steady_clock::now();
            buffer.update(frame);
            continue;
        }

        bad_reads++;

        auto stale_for = std::chrono::steady_clock::now() - last_good;
        if (bad_reads >= 10 || stale_for > std::chrono::seconds(2)) {
            std::cerr << "RTSP feed stale; reopening decoder\n";
            cap.release();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            cap = openRtsp(url);
            bad_reads = 0;
            last_good = std::chrono::steady_clock::now();
        }    
    }
    cap.release();
}

void videoFileThreadWorker(double start_sec, std::string_view file_path, FrameBuffer &buffer, std::atomic<bool> &keep_running) {
    cv::VideoCapture cap(std::string(file_path), cv::CAP_FFMPEG,
            {
            cv::CAP_PROP_OPEN_TIMEOUT_MSEC, 10000,
            cv::CAP_PROP_READ_TIMEOUT_MSEC, 3000
            });

    if (!cap.isOpened()) {
        std::cerr << "Could not read from video file " + std::string(file_path) + "\n";
        keep_running = false;
        return;
    }

    cap.set(cv::CAP_PROP_POS_MSEC, start_sec * 1000.0);

    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0) {
        fps = 30.0;
    }
    auto frame_delay = std::chrono::milliseconds(
            static_cast<int>(1000.0 / fps)
            );

    cv::Mat frame;
    while (keep_running) {
        if (!cap.read(frame) || frame.empty()) {
            keep_running = false;
            break;
        }

        buffer.update(frame);
        std::this_thread::sleep_for(frame_delay);
    }
}
