#ifndef WAVREADER_H
#define WAVREADER_H

#include <stdint.h>

typedef enum{
    WR_OK = 0,
    WR_NO_RIFF,
    WR_NO_WAVE,
    WR_NO_FMT,
    WR_NO_PCM,
    WR_NO_DATA
} WR_ERROR;

typedef struct{
    uint32_t file_size_8;       // size of file - 8 bytes
    uint16_t format_type;       // 1 means PCM
    uint16_t number_of_channels;
    uint32_t sample_rate;
    uint16_t bytes_per_sample;  // of all channels tohether
    uint16_t bits_per_sample;   // how many bits are per sample and channel
    uint32_t data_size;         // size of data
} WR_info;

WR_ERROR WR_Init(WR_info* info);
uint8_t isEndOfData(void);

extern void WR__Device_Init(void);
extern char* WR__Device_getData(uint8_t numOfBytes);
extern void WR__Device_End(void);

#endif