/*
 * delay_mks.c
 *
 *  Created on: Jan 27, 2026
 *      Author: Ostap
 */

#include "delay_mks.h"

extern TIM_HandleTypeDef htim2;

void start_tmr_mks()
{
	HAL_TIM_Base_Start_IT(&htim2);
}

uint32_t get_ticks_mks()
{
	uint32_t ret = TIM2->CNT;
	return ret;
}

void delay_mks(uint32_t delay_len)
{
	uint32_t tmr = get_ticks_mks();
	while((uint32_t)(get_ticks_mks() - tmr) < (uint32_t)delay_len);
}


