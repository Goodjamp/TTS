// fifo_buff.c
//-------DEVRLOPEX-----------
//Created: Gerasimchuk A.
//Date: 11.04.2016

#include "stdint.h"
#include "fifo_buff.h"

void fifo_buff_init(S_user_fifo *fifo_struct){
	fifo_struct->pos_read=0;
	fifo_struct->pos_write=0;
}


int16_t fifo_buff_write(S_user_fifo *fifo_struct, int16_t num_write, int8_t *indata){
	uint16_t cnt;
	uint16_t real_write;
	real_write=fifo_buff_num_read(fifo_struct);
	if(num_write<real_write)
	{
		num_write=real_write;
	}
	for(cnt=0;cnt<num_write;cnt++){
		fifo_struct->buffer[fifo_struct->pos_write]=*indata;
		indata++;
		fifo_struct->pos_write++;
		fifo_struct->pos_write&=FIFO_MASK;
	}
	return(real_write);
}

int16_t fifo_buff_read(S_user_fifo *fifo_struct,int16_t num_read, int8_t *outdata){
	uint16_t cnt;
	uint16_t real_read;
	real_read=fifo_buff_num_read(fifo_struct);
	if(num_read<real_read)
	{
		real_read=num_read;
	}
	for(cnt=0;cnt<real_read;cnt++){
		*outdata=fifo_struct->buffer[fifo_struct->pos_read];
		outdata++;
		fifo_struct->pos_read++;
		fifo_struct->pos_read&=FIFO_MASK;
	}
	return(real_read);
}

int16_t fifo_buff_num_read(S_user_fifo *fifo_struct){
	if (fifo_struct->pos_write >= fifo_struct->pos_read){
		return (fifo_struct->pos_write - fifo_struct->pos_read);
	}
	else{
		return ((FIFO_SIZE - fifo_struct->pos_read) + fifo_struct->pos_write);
	}
}

int16_t fifo_buff_num_write(S_user_fifo *fifo_struct){
	if (fifo_struct->pos_write >= fifo_struct->pos_read){
		return ((FIFO_SIZE - fifo_struct->pos_read) + fifo_struct->pos_write);
	}
	else{
		return (fifo_struct->pos_write - fifo_struct->pos_read);
	}
}
