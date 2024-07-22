#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#include <sysinfoapi.h>
#else
#include <unistd.h>
#include <sys/sysinfo.h>
#endif

// Function to get CPU information
void getCPUInfo() {
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::cout << "Processor Type: " << sysInfo.dwProcessorType << std::endl;
    std::cout << "Number of Cores: " << sysInfo.dwNumberOfProcessors << std::endl;
    // Speed is not directly available; need additional steps
#else
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.substr(0, 10) == "model name") {
            std::cout << "Processor Type: " << line.substr(line.find(":") + 2) << std::endl;
        }
        if (line.substr(0, 7) == "cpu MHz") {
            std::cout << "Processor Speed: " << line.substr(line.find(":") + 2) << " MHz" << std::endl;
            break;
        }
    }
    cpuinfo.close();
#endif
}

// Function to get RAM information
void getRAMInfo() {
#ifdef _WIN32
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    std::cout << "Total RAM: " << statex.ullTotalPhys / (1024 * 1024) << " MB" << std::endl;
#else
    struct sysinfo info;
    sysinfo(&info);
    std::cout << "Total RAM: " << info.totalram / (1024 * 1024) << " MB" << std::endl;
#endif
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_iterations>\n";
        return 1;
    }

    int argument;
    try {
        argument = std::stoi(argv[1]);
        if (argument < 0) {
            throw std::invalid_argument("Negative number not allowed");
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << argv[1] << " is not a valid positive integer.\n";
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Argument out of range: " << argv[1] << " is too large.\n";
        return 1;
    }

    // Get system information
    std::cout << "System Information:" << std::endl;
    getCPUInfo();
    getRAMInfo();

    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // Code block to measure execution time
    for (int i = 0; i < argument; ++i) {
        int result = i * i;
    }

    // Stop measuring time and calculate duration
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Output the duration in seconds
    std::cout << "Value " << argument << " has execution time: " << duration.count() << " seconds\n";

    return 0;
}


// g++ -o benchmark benchmark.cpp
// ./benchmark 1000000
