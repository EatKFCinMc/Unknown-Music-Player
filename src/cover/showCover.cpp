#include "showCover.h"

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <cstring>

#include "stb_image/stb_image.h"

#include "loadCover.h"

#include "../event/globalVar.h"

#include "../logger/log.h"


void send_kitty_image(const std::string& encoded, size_t width,
    size_t height, int format, size_t display_width, size_t display_height) {

    const size_t chunk_size = 4096;

    size_t pos = 0;
    bool first = true;

    while (pos < encoded.size()) {
        size_t n = std::min(chunk_size, encoded.size() - pos);
        bool more = pos + n < encoded.size();

        if (first) {
            std::cout
                << "\033_G"
                << "f=" << format
                << ",a=T"
                << ",s=" << width
                << ",v=" << height
                << ",c=" << display_width
                << ",r=" << display_height
                << ",m=" << (more ? 1 : 0)
                << ";"
                << encoded.substr(pos, n)
                << "\033\\";
            first = false;
        } else {
            std::cout
                << "\033_G"
                << "m=" << (more ? 1 : 0)
                << ";"
                << encoded.substr(pos, n)
                << "\033\\";
        }
        pos += n;
    }

    std::cout << std::flush;
}


static const char b64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode(const unsigned char *data, size_t len)
{
    std::string out;
    int val = 0, valb = -6;
    for (size_t i = 0; i < len; i++) {
        val = (val << 8) + data[i];
        valb += 8;
        while (valb >= 0) {
            out.push_back(b64_table[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(b64_table[((val << 8) >> (valb + 6)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}


void displayCover(const size_t col, const size_t row, const std::string& dir) {
    logger("Loading cover in dir: " + dir);
    if (term_name.find("kitty") == std::string::npos) {
        logger("Skipping cover display because TERM is not kitty-compatible: " + term_name);
        return;
    }

    int img_w = 0;
    int img_h = 0;
    int channels = 0;
    auto pixels = getCoverRawData(dir, img_w, img_h, channels);
    logger("Get cover data complete");
    if (pixels == nullptr) {
        logger("Unable to get cover from file");
        return;
    }

    size_t byte_count = static_cast<size_t>(img_w) * img_h * channels;
    std::string encoded = base64_encode(pixels, byte_count);
    logger("Encode cover data complete");
    if (encoded.empty()) {
        logger("Unable to encode cover into base64");
        stbi_image_free(pixels);
        return;
    }

    int format = 0;
    if (channels == 3) {
        format = 24;
    } else if (channels == 4) {
        format = 32;
    } else {
        logger("Unsupported cover channel count: " + std::to_string(channels));
        stbi_image_free(pixels);
        return;
    }

    const size_t display_width = lb_width > 1 ? lb_width - 1 : 1;
    const size_t display_height = std::min(display_width / 2, term_height > 2 ? term_height - 2 : 1UL);

    logger("img_w=" + std::to_string(img_w) +
       " img_h=" + std::to_string(img_h) +
       " channels=" + std::to_string(channels) +
       " lb_width=" + std::to_string(lb_width) +
       " raw_byte_count=" + std::to_string(byte_count) +
       " encoded_size=" + std::to_string(encoded.size()));

    printf("\033[%zu;%zuH", row, col);
    send_kitty_image(encoded, img_w, img_h, format, display_width, display_height);
    stbi_image_free(pixels);
}


void get_term_name() {
    const char *term = std::getenv("TERM");
    term_name = term == nullptr ? "" : term;
}
