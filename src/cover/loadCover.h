#ifndef RAW_H
#define RAW_H
#include <string>

unsigned char * getCoverRawData(std::string path, int &width, int &height, int &channels);
unsigned char * getFlacCover(const std::string &path, int &width, int &height, int &channels);
unsigned char * getMp3Cover(const std::string &path, int &width, int &height, int &channels);
unsigned char * getOggCover(const std::string &path, int &width, int &height, int &channels);

#endif //RAW_H