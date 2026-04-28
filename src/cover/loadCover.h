#ifndef RAW_H
#define RAW_H
#include <string>
// #include <chafa/chafa.h>
// #include <vector>

#include "stb_image/stb_image.h"

// std::vector<guint8> getCoverRawData(std::string path, int &width, int &height, int &channels);
// std::vector<guint8> getFlacCover(const std::string &path, int &width, int &height, int &channels);
// std::vector<guint8> getMp3Cover(const std::string &path, int &width, int &height, int &channels);
// std::vector<guint8> getOggCover(const std::string &path, int &width, int &height, int &channels);

stbi_uc* getCoverRawData(std::string path, int &width, int &height, int &channels);
stbi_uc* getFlacCover(const std::string &path, int &width, int &height, int &channels);
stbi_uc* getMp3Cover(const std::string &path, int &width, int &height, int &channels);
stbi_uc* getOggCover(const std::string &path, int &width, int &height, int &channels);
#endif //RAW_H