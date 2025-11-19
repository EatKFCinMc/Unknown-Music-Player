#include "loadPic.h"

#include <taglib/flacfile.h>
#include <taglib/flacpicture.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>
#include <filesystem>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"



std::vector<guint8> getCoverRawData(std::string &path, int &width, int &height, int &channels) {
    if (path.ends_with("flac"))
        return getFlacCover(path, width, height, channels);
    if (path.ends_with("mp3"))
        return getMp3Cover(path, width, height, channels);
    if (path.ends_with("ogg"))
        return getOggCover(path, width, height, channels);
    return {};
}


std::vector<guint8> getFlacCover(const std::string &path, int &width, int &height, int &channels) {
    TagLib::FLAC::File file(path.c_str());
    if (!file.isValid()) return {};
    auto pictures = file.pictureList();
    if (pictures.isEmpty()) return {};
    const auto *pic = pictures[0];
    if (pic == nullptr) return {};

    TagLib::ByteVector raw = pic->data();
    if (raw.isEmpty()) return {};
    const int d_size = static_cast<int>(raw.size());
    const auto pic_data = reinterpret_cast<unsigned char *>(raw.data());

    unsigned char* img = stbi_load_from_memory(pic_data, d_size, &width, &height, &channels, 0);
    if (img == nullptr) return {};
    std::vector pixels(img, img + width * height * channels);

    return pixels;
}


std::vector<guint8> getMp3Cover(const std::string &path, int &width, int &height, int &channels) {
    TagLib::MPEG::File file(path.c_str());
    const TagLib::ID3v2::Tag *tag = file.ID3v2Tag();
    if (tag == nullptr) return {};
    const TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
    if (frames.isEmpty()) return {};
    const auto *PicFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames[0]);
    if (PicFrame == nullptr) return {};

    auto raw = PicFrame->picture();
    if (raw.isEmpty()) return {};
    const auto *pic_data = reinterpret_cast<unsigned char *>(raw.data());
    const int d_size = static_cast<int>(raw.size());

    unsigned char* img = stbi_load_from_memory(pic_data, d_size, &width, &height, &channels, 0);
    if (img == nullptr) return {};
    std::vector pixels(img, img + width * height * channels);

    return pixels;
}


std::vector<guint8> getOggCover(const std::string &path, int &width, int &height, int &channels) {
    return {};
}
