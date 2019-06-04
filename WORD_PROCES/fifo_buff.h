// fifo_buff.h
//-------DEVRLOPEX-----------
//Created: Gerasimchuk A.
//Date: 11.04.2016

#ifndef FIFO_BUFF_H_
#define FIFO_BUFF_H_

#include "stdint.h"

#define FIFO_SIZE               8
#define FIFO_MASK    (FIFO_SIZE-1)

typedef struct{
	uint8_t  buffer[FIFO_SIZE];
	uint16_t pos_write;
	uint16_t pos_read;
}S_user_fifo;


typedef enum{
	FIFO_BUFF_OK=0
}FIFO_BUFF_ERROR;


int16_t fifo_buff_write(S_user_fifo *fifo_struct, int16_t num_write, int8_t *indata);
int16_t fifo_buff_read(S_user_fifo *fifo_struct,int16_t num_read, int8_t *outdata);
int16_t fifo_buff_num_read(S_user_fifo *fifo_struct);
int16_t fifo_buff_num_write(S_user_fifo *fifo_struct);

#endif
