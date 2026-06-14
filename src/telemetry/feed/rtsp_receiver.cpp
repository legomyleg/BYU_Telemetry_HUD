#include <opencv2/videoio.hpp>
#include <string>
#include <telemetry/feed/rtsp_receiver.hpp>
#include <iostream>

void captureThreadWorker(std::string_view url, FrameBuffer& buffer, std::atomic<bool>& keep_running) {
    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;tcp", 1);
    std::cout << "thread opened\n";
    cv::VideoCapture cap(std::string(url), cv::CAP_FFMPEG);
    std::cout << "cap ran\n";
    if (!cap.isOpened()) {
        std::cerr << "Error: could not open RTSP stream from url " << url <<"\n";
        keep_running = false;
        std::cout << "RETURNING\n";
        return;
    }
    std::cout << "cap is OPEN\n";
    cv::Mat temp_frame;
    while (keep_running) {
        bool did_read = cap.read(temp_frame);
        if (did_read && !temp_frame.empty()) {
            buffer.update(temp_frame);
        } else if (!did_read) {
            std::cerr << "Err: Could not read frame. cap.read() failed\n";
        } else {
            std::cerr << "Err: Read empty frame\n";
        }
    }
    cap.release();
}

void videoFileThreadWorker(double start_sec, std::string_view file_path, FrameBuffer &buffer, std::atomic<bool> &keep_running) {
    cv::VideoCapture cap(std::string(file_path), cv::CAP_FFMPEG);

    if (!cap.isOpened()) {
        std::cerr << "Could not read from video file " + std::string(file_path) + "\n";
        keep_running = false;
        return;
    }

    cap.set(cv::CAP_PROP_POS_MSEC, start_sec * 1000.0);

    cv::Mat frame;
    while (keep_running) {
        if (!cap.read(frame) || frame.empty()) {
            keep_running = false;
            break;
        }

        buffer.update(frame);
    }
}
