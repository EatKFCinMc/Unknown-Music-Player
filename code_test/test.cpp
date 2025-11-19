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

int main() {
    // get cover picture
    string path = "./code_test/cover.jpg";
    // string m_path = "./test/acta est fabula, plaudite (feat. Irissu, mrcool909090 & Cheryl Stelli) - A-Saph,Irissu,mrcool909090.flac";
    string m_path = "./test/Alea jacta est! (xi Remix) - BlackY.mp3";

    // TagLib::FLAC::File file(m_path.c_str());
    // auto pictures = file.pictureList();
    // if (pictures.isEmpty())
    //     return 0;
    // auto *pic = pictures[0];
    //
    // // std::ofstream out("./code_test/cover.jpg", std::ios::binary);
    // // out.write(pic->data().data(), pic->data().size());
    // TagLib::ByteVector raw = pic->data();
    // int d_size = raw.size();
    //
    // unsigned char *pic_data = (unsigned char*)raw.data();

    TagLib::MPEG::File file(m_path.c_str());
    const TagLib::ID3v2::Tag *tag = file.ID3v2Tag();
    const TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
    const auto *PicFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames[0]);

    auto raw = PicFrame->picture();
    const auto *pic_data = reinterpret_cast<unsigned char *>(raw.data());
    const int d_size = static_cast<int>(raw.size());
    cout<<d_size<<endl;


    // load cover picture
    int img_w, img_h, channels;

    unsigned char* img = stbi_load_from_memory(pic_data, d_size, &img_w, &img_h, &channels, 0);

    cout<<img_w<<" "<<img_h<<" "<<channels<<endl;
    std::vector pixels(img, img + img_w * img_h * channels);
    guint8 *pixel_ptr = pixels.data();

    float ratio = float(img_w) / img_h;

    // Config
    gchar **envp = g_get_environ();
    ChafaTermInfo *term_info = chafa_term_db_detect(chafa_term_db_get_default(), envp);
    ChafaCanvasMode mode;
    mode = chafa_term_info_get_best_canvas_mode(term_info);

    ChafaCanvasConfig *config = chafa_canvas_config_new();
    chafa_canvas_config_set_geometry(config, int(50 * ratio) * 2, 50);
    chafa_canvas_config_set_canvas_mode(config, mode);
    // chafa_canvas_config_set_cell_geometry(config, 1, 1);

    const gchar *term_name = chafa_term_info_get_name(term_info);
    if (!strcmp(term_name, "kitty"))
        chafa_canvas_config_set_pixel_mode(config, CHAFA_PIXEL_MODE_KITTY);

    ChafaSymbolMap *map = chafa_symbol_map_new();
    chafa_symbol_map_add_by_tags(map, chafa_term_info_get_safe_symbol_tags(term_info));
    chafa_canvas_config_set_symbol_map(config, map);

    // Canvas
    ChafaCanvas *canvas = chafa_canvas_new(config);

    // Draw
    chafa_canvas_draw_all_pixels(
        canvas,
        CHAFA_PIXEL_RGB8,
        pixel_ptr,
        img_w,
        img_h,
        img_w * channels
    );

    // Print output
    auto *text = chafa_canvas_print(canvas, nullptr);
    cout << text->str<<endl;
    g_free(text);

    // Cleanup
    chafa_canvas_unref(canvas);
    // chafa_symbol_map_unref(map);
    chafa_canvas_config_unref(config);
    g_strfreev(envp);

    return 0;
}
