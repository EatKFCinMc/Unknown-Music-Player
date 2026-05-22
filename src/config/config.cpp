#include "config.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "log.h"
#include "globalVar.h"

void write_to_config() {
    std::string home_dir = std::getenv("HOME");
    if (home_dir.empty()) {
        logger("Home directory not found");
        return;
    }
    std::string config_dir = home_dir + "/.config/ump";
    if (!std::filesystem::exists(config_dir))
        std::filesystem::create_directory(config_dir);
    logger("Writing config to: " + config_dir);

    config_dir += "ump.conf";
    if (std::filesystem::exists(config_dir))
        std::filesystem::remove(config_dir);

    std::ofstream conf_file(config_dir);
    if (!conf_file) {
        logger("Config file open failed");
        return;
    }

    conf_file<<root_dir<<"\n";
    conf_file<<list_cursor<<"\n";
    conf_file<<list_start_cursor<<"\n";
}


void load_from_config() {
    std::string home_dir = std::getenv("HOME");
    if (home_dir.empty()) {
        logger("Home directory not found");
        return;
    }
    std::string config_dir = home_dir + "/.config/ump";
    if (!std::filesystem::exists(config_dir))
        std::filesystem::create_directory(config_dir);
    logger("Writing config to: " + config_dir);

    config_dir += "ump.conf";
    if (!std::filesystem::exists(config_dir)) {
        logger("Config file not found");
        return;
    }

    std::fstream conf_file(config_dir);
    if (!conf_file) {
        logger("Config file open failed");
        return;
    }

    std::string buffer;
    std::getline(conf_file, root_dir);
    std::getline(conf_file, buffer);
    list_cursor = stoi(buffer);
    std::getline(conf_file, root_dir);
    list_start_cursor = stoi(buffer);
}