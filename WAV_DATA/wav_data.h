// wav_data.h
//-------DEVELOPEX-----------
//Created: Gerasimchuk A
//Date: 31.03.2016

#ifndef WAV_DATA_H_
#define WAV_DATA_H_


typedef struct{
	char leter;
	unsigned int shift;
}S_hash_dictionary;

typedef enum{
	WAV_DATA_OK=0,
	WAV_DATA_WORD_NOT_FIND=1
}WAV_DATA_ERROR;



#endif
