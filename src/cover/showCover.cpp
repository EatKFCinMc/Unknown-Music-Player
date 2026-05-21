#include "showCover.h"

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <sys/ioctl.h>

#include "loadCover.h"
#include "globalVar.h"
#include "log.h"
#include "common.h"
#include "chafa/chafa.h"


void send_kitty_image(const std::string& encoded, size_t width,
    size_t height, int format, size_t display_width, size_t display_height) {

    const size_t chunk_size = 4096;

    size_t pos = 0;
    bool first = true;

    printf("\033[%d;%dH", 2, 2);
    while (pos < encoded.size()) {
        size_t n = std::min(chunk_size, encoded.size() - pos);
        bool more = pos + n < encoded.size();

        if (first) {
            std::cout
                << "\033_G"
                << "a=T"
                << ",f=" << format
                << ",t=d"
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


std::string charset =
    "@%#*+=-:. ";
std::vector<std::string> rgbToAsciiArt(
    const unsigned char* raw_data,
    int img_width,
    int img_height,
    int channels,
    int out_width,
    int out_height,
    bool colored = false,
    bool use_background_color = false
) {
    std::vector<std::string> result;

    if (!raw_data || img_width <= 0 || img_height <= 0 ||
        channels < 3 || out_width <= 0 || out_height <= 0) {
        return result;
    }

    result.reserve(out_height);

    double x_ratio = static_cast<double>(img_width) / out_width;
    double y_ratio = static_cast<double>(img_height) / out_height;

    const std::string& chars = charset;

    for (int y = 0; y < out_height; ++y) {
        std::ostringstream line;

        for (int x = 0; x < out_width; ++x) {
            int src_x = std::min(static_cast<int>(x * x_ratio), img_width - 1);
            int src_y = std::min(static_cast<int>(y * y_ratio), img_height - 1);

            int index = (src_y * img_width + src_x) * channels;

            int r = raw_data[index + 0];
            int g = raw_data[index + 1];
            int b = raw_data[index + 2];

            int gray = static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b);

            size_t char_index = gray * (chars.size() - 1) / 255;

            char c = chars[char_index];

            if (colored) {
                if (use_background_color) {
                    line << "\033[48;2;"
                         << r << ';' << g << ';' << b
                         << "m" << c << "\033[0m";
                } else {
                    line << "\033[38;2;"
                         << r << ';' << g << ';' << b
                         << "m" << c << "\033[0m";
                }
            } else {
                line << c;
            }
        }

        result.push_back(line.str());
    }

    return result;
}


void displayCover(const std::string& dir) {
    std::cout<<"\033_Gd=a\033\\";
    logger("Loading cover in dir: " + dir);
    // load rgb data from cover
    int img_w = 0;
    int img_h = 0;
    int channels = 0;
    auto pixels = getCoverRawData(dir, img_w, img_h, channels);
    logger("Get cover data complete");
    if (pixels == nullptr) {
        logger("Unable to get cover from file");
        return;
    }

    if (kitty_support) {
        // if (term_name.find("kitty") == std::string::npos) {
        //     logger("Skipping cover display because TERM is not kitty-compatible: " + term_name);
        //     return;
        // }


        size_t byte_count = static_cast<size_t>(img_w) * img_h * channels;
        std::string encoded = base64_encode(pixels, byte_count);
        if (encoded.empty()) {
            logger("Unable to encode cover into base64");
            free(pixels);
            return;
        }

        int format = 0;
        if (channels == 3) {
            format = 24;
        } else if (channels == 4) {
            format = 32;
        } else {
            logger("Unsupported cover channel count: " + std::to_string(channels));
            free(pixels);
            return;
        }

        logger("img_w=" + std::to_string(img_w) +
           " img_h=" + std::to_string(img_h) +
           " channels=" + std::to_string(channels) +
           " raw_byte_count=" + std::to_string(byte_count) +
           " encoded_size=" + std::to_string(encoded.size()));

        cover_drawing = true;
        printf("\033[%zu;%zuH", cover_x, cover_y);
        send_kitty_image(encoded, img_w, img_h, format, cover_width, cover_height);
        cover_drawing = false;
    } else {
        // use chafa for non-kitty terminal
        auto output = rgbToAsciiArt(pixels, img_w, img_h, channels, cover_width, cover_height, true, true);
        if (output.empty()) {
            logger("Unable to encode cover into rgb");
            free(pixels);
            return;
        }
        for (size_t i = 0; i < output.size(); i++) {
            printAt(2, 2 + i, output[i]);
        }

    }
    free(pixels);
}


// chafa is so shit so I abandon it

// void displayCover(const size_t col, const size_t row, const std::string& dir) {
//     int img_w, img_h, channels;
//     auto pixels = getCoverRawData(dir, img_w, img_h, channels);
//     if (pixels.empty())
//         return;
//     const auto pixel_ptr = pixels.data();
//
//     ChafaTermInfo *term_info;
//     ChafaCanvasMode mode;
//     ChafaPixelMode pixel_mode;
//     ChafaSymbolMap *symbol_map;
//     ChafaCanvasConfig *config;
//     ChafaCanvas *canvas;
//     ChafaFrame *frame;
//     ChafaImage *image;
//     ChafaPlacement *placement;
//
//     gchar **envp;
//     envp = g_get_environ();
//     term_info = chafa_term_db_detect(chafa_term_db_get_default(), envp);
//     mode = chafa_term_info_get_best_canvas_mode(term_info);
//     pixel_mode = chafa_term_info_get_best_pixel_mode(term_info);
//     symbol_map = chafa_symbol_map_new();
//     chafa_symbol_map_add_by_tags(symbol_map,
//                                  chafa_term_info_get_safe_symbol_tags(term_info));
//
//     config = chafa_canvas_config_new();
//     chafa_canvas_config_set_canvas_mode(config, mode);
//     chafa_canvas_config_set_pixel_mode(config, pixel_mode);
//     chafa_canvas_config_set_geometry(config, lb_width-1, lb_width/2-1);
//
//
//     chafa_canvas_config_set_symbol_map(config, symbol_map);
//
//     canvas = chafa_canvas_new(config);
//     if (channels == 3)
//         frame = chafa_frame_new(pixel_ptr, CHAFA_PIXEL_RGB8, img_w, img_h, img_w * channels);
//     else
//         frame = chafa_frame_new(pixel_ptr, CHAFA_PIXEL_RGBA8_PREMULTIPLIED, img_w, img_h, img_w * channels);
//     image = chafa_image_new();
//     chafa_image_set_frame(image, frame);
//     placement = chafa_placement_new(image, 1);
//     chafa_placement_set_tuck(placement, CHAFA_TUCK_STRETCH);
//     chafa_placement_set_halign(placement, CHAFA_ALIGN_START);
//     chafa_placement_set_valign(placement, CHAFA_ALIGN_START);
//     chafa_canvas_set_placement(canvas, placement);
//     chafa_canvas_config_set_cell_geometry(config, 1, 1);
//
//     auto rows = chafa_canvas_print(canvas, term_info);
//     //std::string img = rows[0];
//
//     if (rows->str) {
//         printf("\033[%lu;%luH", row, col);
//         cover_drawing = true;
//         // for (size_t i = 0; rows[i]; i++) {
//         //     printf("%s", rows[i]);
//         // }
//         // printf("%s", img.c_str());
//         printf("%s", rows->str);
//         cover_drawing = false;
//     }
//
//     // Cleanup
//     chafa_placement_unref(placement);
//     chafa_image_unref(image);
//     chafa_frame_unref(frame);
//     chafa_canvas_unref(canvas);
//     chafa_canvas_config_unref(config);
//     chafa_symbol_map_unref(symbol_map);
//     chafa_term_info_unref(term_info);
//     canvas = NULL;
//     config = NULL;
//     symbol_map = NULL;
//     term_info = NULL;
//     g_strfreev(envp);
//     g_string_free(rows, TRUE);
// }
