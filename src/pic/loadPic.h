#ifndef RAW_H
#define RAW_H
#include <string>
#include <chafa/chafa.h>
#include <vector>

std::vector<guint8> getCoverRawData(std::string &path, int &width, int &height, int &channels);
std::vector<guint8> getFlacCover(const std::string &path, int &width, int &height, int &channels);
std::vector<guint8> getMp3Cover(const std::string &path, int &width, int &height, int &channels);
std::vector<guint8> getOggCover(const std::string &path, int &width, int &height, int &channels);

#endif //RAW_H