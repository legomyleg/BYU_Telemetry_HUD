#include "serial.hpp"

SerialPort::SerialPort(const string& port_name, speed_t baud_rate) {
    fd = open(port_name.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);

    while (fd < 0) {
        cout << "Waiting for connection to serial port " << port_name;
        cout.flush();
        sleep_for(milliseconds(250));

        cout << "\r\033[K" << "Waiting for connection to serial port " << port_name << " .";
        cout.flush();
        sleep_for(milliseconds(250));
        cout << "\r\033[K" << "Waiting for connection to serial port " << port_name << " . .";
        cout.flush();
        sleep_for(milliseconds(250));
        cout << "\r\033[K" << "Waiting for connection to serial port " + port_name + " . . .";
        cout.flush();
        sleep_for(milliseconds(250));
        cout << "\r\033[K";
        cout.flush();
    }

    termios tty{};
    if (tcgetattr(fd, &tty) != 0) {
        close(fd);
        throw runtime_error("Could not read serial settings.");
    }

    cfsetispeed(&tty, baud_rate);
    cfsetospeed(&tty, baud_rate);

    tty.c_cflag |= CLOCAL | CREAD;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;

    tcsetattr(fd, TCSANOW, &tty);
}

SerialPort::~SerialPort() {
    if(fd >= 0) {
        close(fd);
    }
}

string SerialPort::read_available() {
    char buffer[256];
    string result;

    while (true) {
        int n = read(fd, buffer, sizeof(buffer));

        if (n > 0) {
            result.append(buffer, n);
        } else {
            break;
        }
    }

    return result;
}
