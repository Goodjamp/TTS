// pwm.h
//-------DEVRLOPEX-----------
//Created: Gerasimchuk A
//Date: 25.03/2016

#ifndef PWM_H_
#define PWM_H_

#include  "stm32f4xx.h"
#include  "stm32f4xx_rcc.h"
#include  "stm32f4xx_gpio.h"
#include  "stm32f4xx_tim.h"




//Port TIM1 ch1 out
#define PORT_CH1 GPIOA
//Pin TIM1_ch1 out
#define PIN_CH1  GPIO_Pin_8;
//Digits of outputs samples (ARR register)
#define ARR_VAL 256


//SAM Sample rate, Hz
#define  SAM_SAMPLE_RATE 16000

char pwm_config(void);
char pwm_config_gpio(void);
char pwm_config_tim(void);
void pwm_start_play(void);
void pwm_stop_play(void);


#endif
