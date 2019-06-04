// tts_proces.c
//-------DEVRLOPEX-----------
//Created: Gerasimchuk A.
//Date: 3.04/2016
#include "stdint.h"
#include "ctype.h"
#include "stdio.h"
#include "string.h"
//-------------------Add dictionary files----------------
#include "tts_proces.h"
#include "dictionary.h"
//------------------------------------------------------
#include "pwm.h"


SWordBuf WordBuf;  // buffer words to TTS
static uint8_t buffer[MAX_SIZE_BUFFER];
uint8_t buff_word[2][15000]; // Ð±ÑƒÑ„Ñ„ÐµÑ€ Ñ�Ð»Ð¾Ð²
uint32_t pos_word;
S_state sTTSState; //Ð±ÑƒÑ„Ñ„ÐµÑ€ Ð¿Ð¾Ñ‚Ð¾Ñ‡Ð½Ð¾Ð³Ð¾ Ñ�Ð¾Ñ�Ñ‚Ð¾Ñ�Ð½Ð¸Ñ� Ð¿Ñ€Ð¾Ð¸Ð³Ñ€Ñ‹Ð²Ð°Ð¸Ð¼Ð¾Ð³Ð¾


void delay_m(uint32_t period){
	uint32_t cnt=0;
	while(cnt<period){
		cnt++;
	}
}

void tts_delay(uint16_t word_delay){
	while(WordBuf.fNext){} //Wait complete play noise
	WordBuf.fNext=1;
	// NOISE
	WordBuf.Next.length=word_delay;
	WordBuf.Next.position=0;
	WordBuf.Next.sampl=&noise[0];
	while(WordBuf.fNext){} //Wait complete play noise
	WordBuf.fNext=1;
}


TTS_ERROR tts_play(uint8_t* p_sentens){
	uint32_t sentenc_size=0; // size sentence
	uint16_t counter;
	// Calculate size of sentence
	while(p_sentens[sentenc_size] != 0){
		if(sentenc_size>=(MAX_SIZE_SENTENCE-1)){return TTS_INCREAS_SIZE_SENTENCE;}
		sentenc_size++;
	}
	//Make all the letters in capital
	if((*p_sentens))
		for(counter=0; counter<sentenc_size; counter++){
			p_sentens[counter] = toupper((uint32_t)p_sentens[counter]);
	}

}


/*
void tts_update_samples(){
	uint16_t rest_samples; // Ðº-Ð²Ð¾ Ñ�ÐµÐ¼Ð¿Ð»Ð¾Ð² ÐºÐ¾Ñ‚Ð¾Ñ€Ñ‹Ðµ Ñ�Ð¾Ñ‚Ð°Ð»Ð¸Ñ�ÑŒ Ð² Ñ�Ð»Ð¾Ð²Ðµ
	uint16_t write_samples_fifo; // Ðº-Ð²Ð¾ Ñ�ÐµÐ¼Ð¿Ð»Ð¾Ð² ÐºÐ¾Ñ‚Ð¾Ñ€Ñ‹Ðµ Ñ�Ð¾Ñ‚Ð°Ð»Ð¸Ñ�ÑŒ Ð² Ñ�Ð»Ð¾Ð²Ðµ
	uint16_t p_samples;
	// ÐºÐ¾Ð»Ð¸Ñ‡ÐµÑ�Ñ‚Ð²Ð¾ Ñ�ÐµÐ¼Ð¿Ð»Ð¾Ð² Ð² Ñ�Ñ‚ÐµÐºÐµ Ñ�Ð²Ð¾Ð´Ð¾Ð±Ð½Ñ‹Ñ… Ð´Ð»Ñ� Ð·Ð°Ð¿Ð¸Ñ�Ð¸
	write_samples_fifo=fifo_buff_num_write(&sTTSState.sampless_buffer);
	// ÐºÐ¾Ð»Ð¸Ñ‡ÐµÑ�Ñ‚Ð²Ð¾ Ð¾Ñ�Ñ‚Ð°Ð²ÑˆÐ¸Ñ…Ñ�Ñ� Ñ�ÐµÐ¼Ð¿Ð»Ð¾Ð² Ð¸Ð· Ð¿Ð¾Ñ‚Ð¾Ñ‡Ð½Ð¾Ð³Ð¾ Ñ�Ð»Ð¾Ð²Ð°
	rest_samples=sTTSState.BuffWordsSamples[sTTSState.pos_word].word_size -sTTSState.pos_samples;
	// Ð½Ð°Ñ‡Ð°Ð»Ð¾ Ð¿Ð¾Ð·Ð¸Ñ†Ð¸Ð¹ Ñ�ÐµÐ¼Ð¿Ð»Ð¾Ð² ÐºÐ¾Ñ‚Ð¾Ñ€Ñ‹Ðµ Ð½ÑƒÐ¶Ð½Ð¾ ÐºÐ¾Ð¿Ð¸Ñ€Ð¾Ð²Ð°Ñ‚ÑŒ Ð² FIFO
	p_samples=sTTSState.BuffWordsSamples[sTTSState.pos_word].pos_word+sTTSState.samples_pos.pos_samples;
	// ÐºÐ¾Ð¿Ð¸Ñ€ÑƒÑŽ Ñ�ÐµÐ¼Ð¿Ð»Ñ‹ Ñ�Ð»Ð¾Ð²Ð° Ð² Ð±ÑƒÑ„Ñ„ÐµÑ€

	// ÐµÑ�Ð»Ð¸ Ñ�ÐµÐ¼Ð¿Ð»Ð¾Ð² Ð½ÐµÐ´Ð¾Ñ�Ñ‚Ð°Ñ‚Ð¾Ñ‡Ð½Ð¾ - Ð·Ð°Ð¿Ð¸Ñ�Ð°Ñ‚ÑŒ Ñ‚Ðµ Ñ‡Ñ‚Ð¾ Ð¾Ñ�Ñ‚Ð°Ð»Ð¸Ñ�ÑŒ Ð¸ Ð¿ÐµÑ€ÐµÐ¹Ñ‚Ð¸ Ð½Ð° Ð´Ñ€ÑƒÐ³Ð¾Ðµ Ñ�Ð»Ð¾Ð²Ð¾.
	// Ð•Ð¡Ð›Ð˜ Ð¡Ð›ÐžÐ’Ð� Ð’ ÐŸÐžÐ¢ÐžÐ§Ð�ÐžÐœ Ð‘Ð£Ð¤Ð¤Ð•Ð Ð• Ð¡Ð›ÐžÐ’ Ð—Ð�ÐšÐžÐ�Ð§Ð˜Ð›Ð˜Ð¡Ð¬ - Ð Ð�Ð¡ÐŸÐ�Ð Ð¡Ð˜Ð¢Ð¬ Ð¡Ð›Ð•Ð”Ð£Ð®Ð©Ð£Ð® ÐŸÐžÐ Ð¦Ð˜Ð® Ð¡Ð›ÐžÐ’ Ð˜Ð— Ð¡Ð¢Ð•ÐšÐ�
	if(write_samples_fifo >= rest_samples){ // ÐµÑ�Ð»Ð¸ Ð² Ð±ÑƒÑ„Ñ„ÐµÑ€Ðµ FIFO  ,Ð±Ð¾Ð»ÑŒÑˆÐµ Ð¼ÐµÑ�Ñ‚Ð° Ñ‡ÐµÐ¼ Ñ�ÐµÐ¼Ð¿Ð»Ð¾Ð² Ð² Ð¿Ð¾Ñ‚Ð¾Ñ‡Ð½Ð¾Ð¼ Ñ�Ð»Ð¾Ð²Ðµ
		// Ð´Ð¾Ð¿Ð¸Ñ�Ñ‹Ð²Ð°ÑŽ Ñ�ÐµÐ¼Ð¿Ð»Ñ‹ Ð¸Ð· Ð¿Ð¾Ñ‚Ð¾Ñ‡Ð½Ð¾Ð³Ð¾ Ñ�Ð»Ð¾Ð²Ð°
		fifo_buff_write(&sTTSState.sampless_buffer,rest_samples,(uint8_t)*p_samples);
		&sTTSState.sampless_buffer
		// Ð¾Ð±Ñ€Ð°Ð±Ð°Ñ‚Ñ‹Ð²Ð°ÑŽ Ñ�Ð»ÐµÐ´ÑƒÑŽÑ‰ÐµÐµ Ñ�Ð»Ð¾Ð²Ð¾
		if()
	}
	fifo_buff_write(&sTTSState.sampless_buffer,write_samples_fifo,(uint8_t)*p_samples);
}
*/


TTS_ERROR tts_proces_play(uint8_t* p_sentens){
	TTS_ERROR rez;
	uint8_t cnt;
	uint8_t num_words=0; // numbers words in sentence
	uint16_t pos_next_word=0;
	// Divide sentence on a words
	tts_proces_prepare_sentenc(p_sentens,&num_words);
	//
	WordBuf.n_buff=1;
	find_word_samples(&buffer[pos_next_word],&WordBuf); // Processing first word
	WordBuf.fNext=1;
	pos_next_word+=strlen(( char*)&buffer[pos_next_word])+1;
	pwm_start_play();
	// serial play all words
	for(cnt=1;cnt<=num_words;cnt++){
		// WORD
		if(isalpha((uint32_t)buffer[pos_next_word])){ // word processing
			rez=find_word_samples(&buffer[pos_next_word],&WordBuf);
			switch(rez){
			case TTS_WORD_ABSENT:
				WordBuf.Next.length=2000;
				WordBuf.Next.position=0;
				WordBuf.Next.sampl=&pi[0];
				break;
			}
			tts_delay(WORDS_DELAY);
		}
		else if(isdigit((uint32_t)buffer[pos_next_word])){
			tts_proces_digit_process(&buffer[pos_next_word]);//digit processing
		}
		pos_next_word+=strlen(( char*)&buffer[pos_next_word])+1;

	}
	pwm_stop_play();
	delay_m((uint32_t)6*8000000);

	return TTS_OK;
}


//--------function SENTENC_TO_WORD_ERROR update_sentenc(uint8_t* p_sentens)-----
//function update_sentenc - update input sentence according next rules^
//   - all letter UP
//	 - copy input buf
//   - Calculate size of sentence
//   - Make all the letters in capital
//   - delete not letters and space symbol
TTS_ERROR tts_proces_prepare_sentenc(uint8_t* p_sentens, uint8_t *p_num_words){
	uint32_t counter=0;   // counter symbol
	uint32_t counter_word=0; // counter words
	uint32_t sentenc_size=0; // size sentence
	// clear pos_word
	pos_word=0;
	// Calculate size of sentence
	while(p_sentens[sentenc_size] != 0){
		if(sentenc_size>=(MAX_SIZE_SENTENCE-1)){return TTS_INCREAS_SIZE_SENTENCE;}
		sentenc_size++;
	}
	//Make all the letters in capital
	if((*p_sentens))
		for(counter=0; counter<sentenc_size; counter++){
			p_sentens[counter] = toupper((uint32_t)p_sentens[counter]);
	}
	// delete not letters, not digits and excess space symbol
	counter=0;
	while(counter<sentenc_size){
		if(isalpha((uint32_t)p_sentens[counter])){
			// copy word in buffer
			counter+=tts_proces_shift_word(&p_sentens[counter]);
			// if after word situated not divider symbol - error
			if(!IS_DEVIDER(p_sentens[counter])){
				return TTS_WORD_ERROR;
			}
			counter_word++;
			continue;
		}
		if(isdigit((uint32_t)p_sentens[counter])){
			// copy numeric in buffer
			counter+=tts_proces_shift_digit(&p_sentens[counter]);
			// if after numeric situated not divider symbol - error
			if(!IS_DEVIDER(p_sentens[counter])){
				return TTS_WORD_ERROR;
			}
			counter_word++;
			continue;
		}
		counter++;
	}

	if(!pos_word){return TTS_NO_DATA;}

	(*p_num_words)=counter_word;
	return TTS_OK;

}


// ------------function uint8_t tts_proces_shift_word(uint8_t* in)------------------
//function tts_proces_shift_word -
uint32_t  tts_proces_shift_word(uint8_t* in){
	uint8_t cnt=0;
	while(isalpha((uint32_t)(*in))){
		buffer[pos_word]=(*in);
		in++;
		pos_word++;
		cnt++;
	}
	buffer[pos_word]=0;
	pos_word++;
	return(cnt);
}

// ------------function uint8_t tts_proces_shift_digit(uint8_t* in)------------------
//
uint32_t  tts_proces_shift_digit(uint8_t* in){
	uint8_t cnt=0;
	while(isdigit((uint32_t)(*in))){
		buffer[pos_word]=(*in);
		in++;
		pos_word++;
		cnt++;
	}
	buffer[pos_word]=0;
	pos_word++;
	return(cnt);
}


//---------------function tts_proces_normalisation_word----------------------------------
//
void tts_proces_normalisation_word(uint8_t *samples,uint8_t *buffer_wors , uint16_t size){
	uint16_t cnt;
	float k1;//=((float)Y2-(float)Y1)/(float)WINDOWS_SIZE;
	float b1;//=(float)Y2-(float)size*(float)k1;
	uint16_t windows_size;
	if(WINDOWS_SIZE<=size){
		windows_size=WINDOWS_SIZE;
		// copy not processing part
		memcpy((char*)buffer_wors,(char*)samples,(size-WINDOWS_SIZE));
	}
	else{
		windows_size=size;
	}
	k1=((float)Y2-(float)Y1)/(float)windows_size;
	b1=(float)Y2-(float)size*(float)k1;

	// processing window part
	for(cnt=(size-windows_size);cnt<size;cnt++){
		if(samples[cnt]>=128){
			buffer_wors[cnt]=128+(samples[cnt]-128)*(cnt*k1+b1);
		}
		else{
			buffer_wors[cnt]=128-(128-samples[cnt])*(cnt*k1+b1);
		}
	}

}


//--------------function tts_proces_digit_process---------------
TTS_ERROR tts_proces_digit_process(uint8_t *pdigit){
	uint8_t cnt;
	uint8_t num_part;    // number of (NUM_DIGITS) part
	uint8_t extended;
	uint16_t size_number;
	size_number=strlen((char*)pdigit);
	num_part=(size_number-1)/NUM_DIGITS;
	extended=size_number-num_part*NUM_DIGITS;
	num_part++;

	for(cnt=0;cnt<num_part;cnt++){
		if((extended != 0)&&(cnt==0)){
			tts_proces_digit_play(pdigit,extended);
		}
		else{
			tts_proces_digit_play(pdigit+extended+(cnt-1)*3,NUM_DIGITS); //play 3 digits number
		}
		switch(num_part-cnt){   // play  THOUSAND, MILLION and more words if necessary
			case THOUSAND:
				find_word_samples(digit[THOUSAND_POS],&WordBuf);  // play  THOUSAND
				tts_delay(NUMERIC_DELAY);
				break;
			case MILLION:
				find_word_samples(digit[MILLION_POS],&WordBuf);  // play  MILLION
				tts_delay(NUMERIC_DELAY);
				break;
			default: break;
		}
	}
	return TTS_OK;
}

TTS_ERROR tts_proces_digit_play(uint8_t *part_number,uint8_t length_number){
	uint8_t cnt;
	if(length_number>NUM_DIGITS){return TTS_DIGITS_INCR;};
	for(cnt=(NUM_DIGITS-length_number);cnt<NUM_DIGITS;cnt++,part_number++){
		if(*part_number =='0'){
			continue;
		};
		switch(cnt){
		case 0:
			   find_word_samples(digit[*part_number-START_NUMBER_ASCII],&WordBuf); // firest HUNDRED digits
			   tts_delay(NUMERIC_DELAY);
			   find_word_samples(digit[HUNDRED_POS],&WordBuf);  // play  HUNDRED
			   tts_delay(NUMERIC_DELAY);
			   break;
		case 1:
			if(*part_number =='1'){
				find_word_samples(digit[10+(*(part_number+1)-START_NUMBER_ASCII)],&WordBuf); // firest HUNDRED digits
				tts_delay(NUMERIC_DELAY);
				return TTS_OK;
			}
			else{
				find_word_samples(digit[SHIFT_POS_20+(*part_number-START_NUMBER_ASCII)],&WordBuf); // firest HUNDRED digits
				tts_delay(NUMERIC_DELAY);
				break;
			}
		case 2:
			 	find_word_samples(digit[*part_number-START_NUMBER_ASCII],&WordBuf); // firest HUNDRED digits
			 	tts_delay(NUMERIC_DELAY);
			break;
		}

	}
	return TTS_OK;
}


uint32_t data;
//----------function find_word_samples--------------
//function find_word_samples - find position word samples and it parameters
//on dictionary
// pposition  - pointer on first words symbol
TTS_ERROR find_word_samples(const uint8_t *pposition, SWordBuf *psFindWord){
	uint16_t cnt;
	const uint8_t* position_word_area; // pointer on word area in samples file
	uint32_t length_sempl;

	// If no words in selected letter - return
	if(dictionary[(*pposition)-START_LETER_ASCII].num_words == 0){return TTS_WORD_ABSENT;}

	position_word_area=(const uint8_t* )(dictionary[(*pposition)-START_LETER_ASCII].shift+&samples[0]);//find begin position letter
	for(cnt=0;cnt<dictionary[(*pposition)-START_LETER_ASCII].num_words;cnt++){
		if(!strcmp(( char*)pposition,( char*)position_word_area)){ // word find successful

			if(psFindWord->n_buff!=0){psFindWord->n_buff=0;}
			else{psFindWord->n_buff=1;}

			psFindWord->Next.length=*((typeof(psFindWord->Next.length)*)(position_word_area+strlen(( char*)position_word_area)+1)); // length samples
			psFindWord->Next.sampl=&buff_word[psFindWord->n_buff][0];  //(uint8_t const*)(position_word_area+strlen(( char*)position_word_area)+1+sizeof(psFindWord->Next.length)); 	    // address begin samples
			psFindWord->Next.position=0;                                                                                          // update temp counter
			// copy and processingd samples in buffer
			tts_proces_normalisation_word((uint8_t*)(position_word_area+strlen(( char*)position_word_area)+1+sizeof(psFindWord->Next.length)),\
							              (uint8_t*)psFindWord->Next.sampl,
							              psFindWord->Next.length);
			return TTS_OK;
		}
		data = strlen(( char*)position_word_area);
		length_sempl=*((typeof(psFindWord->Next.length)*)(position_word_area+strlen(( char*)position_word_area)+1));
		position_word_area+=strlen(( char*)position_word_area)+1+sizeof(psFindWord->Next.length)+length_sempl;      // position next word
	}

	return TTS_WORD_ABSENT;
}

