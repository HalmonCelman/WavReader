#include "wavreader.h"

uint32_t dataCounter=0;

/*
    returns 0 when equal, 1 when different
    BUT this is NOT just strcmp
    ends when str2 have \0 inside or 'strings' are different
    it means that:
    cmpstr2("a","abc") will return 1,
    but cmpstr2("abc","a") will return 0
    because "abc" isn't inside "a", but "a" is inside "abc"
*/
static uint8_t cmpstr2(char * str1, char* str2){
    uint8_t diff = 0;
    while(*str2 && !diff){
        diff = (uint8_t)(*str1++ != *str2++);
    }
    return diff;
}

/*
    returns DblWord from data provided
    remembering that this data is in LSB first format
*/
static uint32_t makeDblWord(uint8_t* data){
    data+=3;
    uint32_t DblWord = (uint8_t)*data--;
    DblWord = (DblWord<<8) + (uint8_t)*data--;
    DblWord = (DblWord<<8) + (uint8_t)*data--;
    DblWord = (DblWord<<8) + (uint8_t)*data;
    return DblWord;
}

/*
    returns Word from data provided
    remembering that this data is in LSB first format
*/
static uint16_t makeWord(uint8_t* data){
    data++;
    uint16_t Word = (uint8_t)*data--;
    Word = (Word<<8) + (uint8_t)*data;
    return Word;
}


inline uint8_t isEndOfData(WR_info *info){
    return (dataCounter >= info->data_size);
}


/*
    Initializes WavReader
    returns 0 if everything is OK
    else it returns error code
    argument: pointer to structure where info about data should be stored
*/
WR_ERROR WR_Init(WR_info *info){
    WR__Device_Init();

    dataCounter=0;

    // checking start of "file"
    if(cmpstr2(WR__Device_getData(4),"RIFF"))   return WR_NO_RIFF;
    info->file_size_8 = makeDblWord(WR__Device_getData(4));
    if(cmpstr2(WR__Device_getData(4),"WAVE"))   return WR_NO_WAVE;
    if(cmpstr2(WR__Device_getData(4),"fmt"))    return WR_NO_FMT;
    WR__Device_getData(4); // skip format size
    info->format_type = makeWord(WR__Device_getData(2));
    if( info->format_type != 1)    return WR_NO_PCM;
    info->number_of_channels = makeWord(WR__Device_getData(2));
    info->sample_rate = makeDblWord(WR__Device_getData(4));
    WR__Device_getData(4); // skip bytes per second 
    info->bytes_per_sample = makeWord(WR__Device_getData(2));
    info->bits_per_sample = makeWord(WR__Device_getData(2));
    if(cmpstr2(WR__Device_getData(4),"data"))   return WR_NO_DATA;
    info->data_size = makeDblWord(WR__Device_getData(4));

    return WR_OK;
}

void get16bitSample(WR_info* info, uint16_t *channels){   
    for(int i=0; i<info->number_of_channels ;i++){
        *channels++ = makeWord(WR__Device_getData(2)); //not sure if makeWord will work here
        dataCounter+=16;
    }
}

void get8bitSample(WR_info* info, uint8_t *channels){   
    for(int i=0; i<info->number_of_channels ;i++){
        *channels++ = WR__Device_getData(2)+128; //not sure if its correct - hardware will verify
    }
}