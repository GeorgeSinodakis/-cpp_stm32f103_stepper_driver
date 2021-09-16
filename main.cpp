#include <stdint.h>
#include "nvic.h"
#include "tim2345.h"
#include "delay.h"
#include "rcc.h"
#include "gpio.h"
#include "stepperMotor.h"

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

stepperMotor motor;

extern "C" void TIM2_IRQHandler()
{
	TIM2_SR &= ~(1<<0);
	TIM2_ARR = (u16)(TIM2clock / (u32)motor.sps);
	if(!motor.makeStep()) NVIC_ICER0 |= 1<<28;
}

int main(void)
{
	clock_72Mhz();

	GPIOA_clock_enable();
	GPIOA_mode(INPUT_PULL_UPDOWN, button0Pin);
	GPIOA_set(button0Pin);
	GPIOA_mode(INPUT_PULL_UPDOWN, button1Pin);
	GPIOA_set(button1Pin);

	GPIOB_clock_enable();
	GPIOB_mode(OUTPUT_PUSHPULL, stepPin);
	GPIOB_clear(stepPin);
	GPIOB_mode(OUTPUT_PUSHPULL, dirPin);
	GPIOB_clear(dirPin);

	RCC_APB1ENR |= 1;
	TIM2_ARR = 1000;
	TIM2_CR2 = 0;
	TIM2_DIER = 1<<0;
	TIM2_PSC = 2;
	TIM2_CR1 = 1<<2|1<<0;

	while (1)
	{
		if(!GPIOA_read(button0Pin))
		{
			if(motor.motorStopped())
			{
				motor.moveToPosition(motor.position + motor.Spr * 200);
			}
		}		
		if(!GPIOA_read(button1Pin))
		{
			if(motor.motorStopped())
			{
				motor.moveToPosition(motor.position - motor.Spr * 20);
			}
		}		
	}
}