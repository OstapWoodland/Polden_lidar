/*
 * delay_mks.h
 *
 *  Created on: Jan 27, 2026
 *      Author: Ostap
 */

#ifndef INC_DELAY_MKS_H_
#define INC_DELAY_MKS_H_

#include "main.h"
#include "Polden_lidar_main.h"

void start_tmr_mks();
uint32_t get_ticks_mks();
void delay_mks(uint32_t delay_len);


#endif /* INC_DELAY_MKS_H_ */
