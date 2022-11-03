#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "audio.h"

AUDIO::WAV::WAV(){};
AUDIO::MP3::MP3(){};


AUDIO::AUDIO(const char* fileName){
    this->fileName = (std::string)fileName;
    std::size_t d = this->fileName.rfind(".");
    if(d != std::string::npos) this->subFileName = this->fileName.substr(d+1);
};

AUDIO AUDIO::open(const char* fileName){
    int BUFSIZE = 256;
    size_t rd;
    std::vector<uint8_t> buffer(BUFSIZE, 0);

    this->fileName = (std::string)fileName;
    std::size_t d = this->fileName.rfind(".");
    if(d != std::string::npos) this->subFileName = this->fileName.substr(d+1);

    FILE* audioFile = std::fopen(this->fileName.c_str(), "rb");
    if(audioFile){
        if(this->subFileName == "mp3") std::fread(this->mp3H, 1, sizeof(this->mp3H), audioFile);
        else if(this->subFileName == "wav") std::fread(this->wavH, 1, sizeof(this->wavH), audioFile);
    }
    while(!feof(audioFile)){
        // rd = fread(buffer.begin(), sizeof(uint8_t), BUFSIZE audioFile);
        // if(rd == BUFSIZE) this->data.insert(this->data.end(), buffer.begin(), buffer.end());
        // else this->data.insert(this->data.end(), buffer.begin(), buffer.begin() + rd);

        std::fread(buffer.begin(), sizeof(uint8_t) * BUFSIZE, 1, audioFile);
        this->data.insert(this->data.end(), buffer.begin(), buffer.end());
    }
    return *this;
}