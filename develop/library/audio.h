#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#ifndef LIB_AUDIO_H_
#define LIB_AUDIO_H_


class AUDIO{
    public:
        // https://zh.wikipedia.org/wiki/WAV
        struct WAV{
            uint8_t         RIFF[4];        // RIFF Header Magic header
            uint32_t        ChunkSize;      // RIFF Chunk Size
            uint8_t         WAVE[4];        // WAVE Header
            /* "fmt" sub-chunk */
            uint8_t         fmt[4];         // FMT header
            uint32_t        Subchunk1Size;  // Size of the fmt chunk
            uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
            uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
            uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
            uint32_t        bytesPerSec;    // bytes per second
            uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
            uint16_t        bitsPerSample;  // Number of bits per sample
            /* "data" sub-chunk */
            uint8_t         Subchunk2ID[4]; // "data"  string
            uint32_t        Subchunk2Size;  // Sampled data length

            WAV();
        };

        // https://en.wikipedia.org/wiki/MP3#/media/File:Mp3filestructure.svg
        struct MP3{
            bool sync_word[11];
            bool version;
            bool layer[2];
            bool error_protection;
            bool bit_rate[4];
            bool frequency[2];
            bool pad_bit;
            bool priv_bit;
            bool mode[2];
            bool mode_extension[2];
            bool copy;
            bool original;
            bool emphasis[2];

            MP3();
        }

        std::string fileName, subFileName;
        std::vector<uint8_t> data;
        WAV wavH;
        MP3 mp3H;



        AUDIO(const char* fileName);

        AUDIO open(const char* fileName);
}


#endif