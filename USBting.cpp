#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

bool isDeviceConnected(const std::string& portID) {
    std::ostringstream command;
    // if running on RPI use lsusb instead of system_profiler
    command << "lsusb | grep '" << portID << "'";
    // command << "system_profiler SPUSBDataType | grep '" << portID << "'";

    // Run the lsusb command and check if the output contains the specified port ID
    FILE* fp = popen(command.str().c_str(), "r");
    if (!fp) {
        std::cerr << "Error running lsusb command." << std::endl;
        return false;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        if (strstr(buffer, portID.c_str()) != nullptr) {
            pclose(fp);
            return true;
        }
    }

    pclose(fp);
    return false;
}

int main() {
    // Replace "yourPortID" with the specific information for your USB port
    std::string portID = "05ac:12a8";
    while (true) {
        if (isDeviceConnected(portID)) {
            std::cout << "USB device is plugged into port " << portID << "." << std::endl;
        }
        else {
            std::cout << "No USB device detected in port " << portID << "." << std::endl;
        }
    }

    return 0;
}