// tts_proces.h
//-------DEVRLOPEX-----------
//Created: Gerasimchuk A.
//Date: 3.04/2016
#ifndef TTS_PROCES_H_
#define TTS_PROCES_H_
#include "stdint.h"
#include "fifo_buff.h"

//-------------IMPORTANT DICTIONARY DEFINITION-----------------------
typedef struct{
	uint8_t leter;
	uint32_t shift;
	uint16_t num_words;
}SLeter;

typedef struct{
	SLeter leter[26];
}SDictionary;
//-------------------------------------------------------------------

#define MAX_SIZE_SENTENCE 254
#define MAX_SIZE_BUFFER   MAX_SIZE_SENTENCE*2
#define IS_LIST_OF_SYMBOL(X)  ()

//------------NOT VAR DEFINITION-------------
#define START_LETER_ASCII    'A'
#define START_NUMBER_ASCII   '0'

//------------PROCESSING WORDS------------------
#define WINDOWS_SIZE     1000
#define Y1                  1
#define Y2                  1
//----------------------------------------------

//----------MUMBERS PROCESSING----------------
#define NUM_DIGITS    3
#define THOUSAND      2
#define MILLION       3

#define HUNDRED_POS   28
#define THOUSAND_POS  29
#define MILLION_POS   30
#define SHIFT_POS_20  18
//-------------------------------------------

// ----------Delay between words---------------
#define WORDS_DELAY 2000
// ----------Delay between numeric---------------
#define NUMERIC_DELAY 1000

typedef enum{
	COMA
}DEVIDE_SYMBOL;

#define IS_DEVIDER(X) ((X==' ')||(X==',')||(X=='.')||(X==0)||(X=='-')||(X=='(')||(X==')'))

typedef enum{
	TTS_OK=0,
	TTS_INCREAS_SIZE_SENTENCE=1,
	TTS_NO_DATA=2,
	TTS_WORD_ERROR=3,
	TTS_WORD_ABSENT=4,
	TTS_DIGITS_INCR=5,
	TTS_IN_PROCES=6
}TTS_ERROR;


typedef struct{
	uint16_t length;
	uint16_t position;
	uint8_t const *sampl;
}SWord;

typedef struct{
	SWord Present;
	SWord Next;
	volatile uint8_t n_buff;
	volatile uint8_t fNext;
}SWordBuf;

typedef struct{
	uint8_t buff_pos;

}S_word_pos;

//---------------STATE STRUCT DESCRIPTION-----------------------------

#define NUM_WORDS    25

// Буффер  семплов слова
typedef struct{
	uint32_t *pos_word;      // указатель на начало семплов слова
	uint16_t word_size;      // размер слова в семплах
}S_samples_position;


typedef struct{
	S_samples_position    BuffWordsSamples[NUM_WORDS];// масив указателей на начало слов
	uint32_t    pos_samples;                    // позиция семпла в слове
	uint16_t    word_pos;                       // позиция поточного слова в буффере слов
	uint8_t     *pos_word;                      // указатель на поточную позицию в тексте с которой нужно продолжать проигрывать
	S_user_fifo  sampless_buffer;               // fifo буффер семплов
}S_state;


TTS_ERROR tts_proces_play(uint8_t* p_sentens);
TTS_ERROR find_word_samples(const uint8_t *pposition, SWordBuf *psFindWord);
TTS_ERROR tts_proces_prepare_sentenc(uint8_t* p_sentens, uint8_t *p_num_words);

uint32_t tts_proces_shift_word(uint8_t* in);
uint32_t tts_proces_shift_digit(uint8_t* in);
TTS_ERROR tts_proces_digit_process(uint8_t *pdigit);

void tts_proces_copy(uint8_t *samples,uint8_t *buffer_wors , uint16_t size);
void tts_proces_normalisation_word(uint8_t *samples,uint8_t *buffer_wors , uint16_t size);
TTS_ERROR tts_proces_digit_play(uint8_t *part_number,uint8_t length_number);

#endif
