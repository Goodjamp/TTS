// pwm.c
//-------DEVRLOPEX-----------
//Created: Gerasimchuk A
//Date: 25.03/2016
#include "pwm.h"
#include "tts_proces.h"

extern SWordBuf WordBuf;


//-----function pwm_config-----------
char pwm_config(void){
	pwm_config_gpio();
	pwm_config_tim();
	return 0;
}


//-----function pwm_config_gpio-----------
char pwm_config_gpio(void){
	GPIO_InitTypeDef gpio_init;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//-----------TIM1 CH1 OUT config---------------
	gpio_init.GPIO_Mode=GPIO_Mode_AF;
	gpio_init.GPIO_OType=GPIO_OType_PP;
	gpio_init.GPIO_Pin=PIN_CH1;
	gpio_init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio_init.GPIO_Speed=GPIO_Speed_25MHz;

	GPIO_PinAFConfig(PORT_CH1,GPIO_PinSource8,GPIO_AF_TIM1);

	GPIO_Init(PORT_CH1,&gpio_init);
//-----------------------------------------------
//-------------DEBUG PIN OUTconfig--------------------
	gpio_init.GPIO_Mode=GPIO_Mode_OUT;
	gpio_init.GPIO_OType=GPIO_OType_PP;
	gpio_init.GPIO_Pin=GPIO_Pin_0;
	gpio_init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio_init.GPIO_Speed=GPIO_Speed_25MHz;

	GPIO_Init(GPIOA,&gpio_init);
	return 0;
}


char pwm_config_tim(void){
	RCC_ClocksTypeDef rcc_clock;
	char pwm_period_per_sampe_rate;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_GetClocksFreq(&rcc_clock);
	TIM1->CR1|=TIM_CR1_ARPE;
	TIM1->PSC = 1;
	pwm_period_per_sampe_rate=(rcc_clock.PCLK2_Frequency * 2)/((ARR_VAL + 1) * (TIM1->PSC + 1) * SAM_SAMPLE_RATE);
	TIM1->RCR=pwm_period_per_sampe_rate;
	TIM1->ARR=ARR_VAL;
	//---------------OUTPUT MODE CONFIG---------------
	TIM1->BDTR&=~(TIM_BDTR_LOCK); // Clear ALL lock bits
	// configuration output mode
	TIM1->CCMR1&=~(TIM_CCMR1_CC1S);
	TIM1->CCMR1|=(TIM_CCMR1_OC1PE);
	TIM1->CCMR1|=(TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	// Enable output channel and configuration polarity
	TIM1->CCER&=(TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC1NE | TIM_CCER_CC1NP);
	TIM1->CCER|=(TIM_CCER_CC1E);
    // Enable output pin
	TIM1->BDTR|=TIM_BDTR_AOE;
	TIM1->BDTR|=TIM_BDTR_MOE;
	// Update interrupt enable
	TIM1->DIER|=TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	// Generation update
	TIM1->EGR|=TIM_EGR_UG;
	TIM1->SR=0;
	//Start counter
	//TIM1->CR1|=TIM_CR1_CEN;
	return 0;
}

void TIM1_UP_TIM10_IRQHandler(void){
	TIM1->SR=0;

	if(WordBuf.Present.position>=WordBuf.Present.length){
		WordBuf.Present=WordBuf.Next; //UPDATE WORD
		WordBuf.fNext=0; // update word
	}
	TIM1->CCR1=(*WordBuf.Present.sampl);
	WordBuf.Present.sampl++;
	WordBuf.Present.position++;
}

void pwm_start_play(void){
	WordBuf.Present=WordBuf.Next; //UPDATE WORD
	TIM1->SR=0;
	TIM1->CR1|=TIM_CR1_CEN;
}

void pwm_stop_play(void){
	TIM1->CR1&=~(TIM_CR1_CEN);
	TIM1->SR=0;
}

