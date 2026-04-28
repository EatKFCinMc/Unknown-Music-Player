#include "log.h"

#include <stdio.h>
#include <filesystem>

#include "../event/events.h"

void log_init() {
    if (std::filesystem::exists("ump.log"))
        std::filesystem::remove("ump.log");
}

void logger(const std::string &message) {
    // Simple logging to a file
    if (LOGGER_DISABLED) return;
    FILE *logFile = fopen("ump.log", "a");
    if (logFile != NULL)
        fprintf(logFile, "%s\n", message.c_str());
    fclose(logFile);
}