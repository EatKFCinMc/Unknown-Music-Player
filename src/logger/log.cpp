#include "log.h"

#include <stdio.h>
#include <filesystem>
#include <thread>
#include <ctime>

#include "../event/globalVar.h"

void log_init() {
    if (std::filesystem::exists("ump.log"))
        std::filesystem::remove("ump.log");
}

// Get the current date/time. The format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

void log_thread(const std::string &message) {
    // Simple logging to a file
    if (LOGGER_DISABLED) return;
    std::string buf = currentDateTime();
    buf = "[" + buf + "]: " + message;
    FILE *logFile = fopen("ump.log", "a");
    if (logFile != nullptr)
        fprintf(logFile, "%s\n", buf.c_str());
    fclose(logFile);
}

void logger(const std::string &message) {
    std::thread log_t(log_thread, message);
    log_t.detach();
}