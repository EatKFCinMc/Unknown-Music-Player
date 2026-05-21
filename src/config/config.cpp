#include "config.h"

#include "log.h"
#include "globalVar.h"

void write_to_config() {
    std::string home_dir = std::getenv("HOME");
    if (home_dir.empty()) {
        logger("Home directory not found");
        return;
    }
    std::string config_dir = home_dir + "/.config";
    logger("Writing config to: " + config_dir);

}


void load_from_config() {

}