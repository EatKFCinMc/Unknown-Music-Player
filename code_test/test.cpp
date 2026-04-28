#include <iostream>
#include <vector>
#include <chafa/chafa.h>
#include <filesystem>
#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>
#include <fstream>
#include <typeinfo>
#include <string>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

using namespace std;

void get_pixels(std::vector<guint8> &pixels, int &width, int &height, int &channels, const string &path) {
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load image\n";
        return;
    }

    for (int i = 0; i < width * height * channels; i++)
        pixels.push_back(data[i]);

    stbi_image_free(data);
}


stbi_uc* getFlacCover(const std::string &path, int &width, int &height, int &channels) {
    TagLib::FLAC::File file(path.c_str());
    if (!file.isValid()) return {};
    auto pictures = file.pictureList();
    if (pictures.isEmpty()) return {};
    const auto *pic = pictures[0];
    if (pic == nullptr) return {};

    TagLib::ByteVector raw = pic->data();
    if (raw.isEmpty()) return {};
    const int d_size = static_cast<int>(raw.size());
    const auto pic_data = reinterpret_cast<stbi_uc*>(raw.data());

    stbi_uc *img = stbi_load_from_memory(pic_data, d_size, &width, &height, &channels, 0);
    if (img == nullptr) return {};
    // std::vector pixels(img, img + width * height * channels);

    return img;
}


stbi_uc* getCoverRawData(const std::string path, int &width, int &height, int &channels) {
    if (path.ends_with("flac"))
        return getFlacCover(path, width, height, channels);
    return nullptr;
}


void send_kitty_image(const std::string& encoded, size_t width,
    size_t height, int format, size_t display_cols, size_t display_rows) {

    const size_t chunk_size = 4096;

    size_t pos = 0;
    bool first = true;

    while (pos < encoded.size()) {
        size_t n = std::min(chunk_size, encoded.size() - pos);
        bool more = pos + n < encoded.size();

        if (first) {
            std::cout
                << "\x1b_G"
                << "f=" << format
                << ",a=T"
                << ",s=" << width
                << ",v=" << height
                << ",c=" << display_cols
                << ",r=" << display_rows
                << ",m=" << (more ? 1 : 0)
                << ";"
                << encoded.substr(pos, n)
                << "\x1b\\";
            first = false;
        } else {
            std::cout
                << "\x1b_G"
                << "m=" << (more ? 1 : 0)
                << ";"
                << encoded.substr(pos, n)
                << "\x1b\\";
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


int main() {
    printf("\033[?1049h");

    string dir = "../test/acta est fabula, plaudite (feat. Irissu, mrcool909090 & Cheryl Stelli) - A-Saph,Irissu,mrcool909090.flac";

    int img_w, img_h, channels;
    auto pixels = getCoverRawData(dir, img_w, img_h, channels);

    size_t byte_count = static_cast<size_t>(img_w) * img_h * channels;
    std::string encoded = base64_encode(pixels, byte_count);

    int format;
    if (channels == 3)
        format = 24;
    else format = 32;

    printf("\033[%lu;%luH", 2, 2);
    send_kitty_image(encoded, img_w, img_h, format, 77, 77/2);
    free(pixels);

    std::this_thread::sleep_for(2s);
    printf("\033[?1049l");
}
