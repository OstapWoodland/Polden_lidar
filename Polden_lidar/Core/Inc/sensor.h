/*
 * sensor.h
 *
 *  Created on: Jan 27, 2026
 *      Author: Ostap
 */

#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

#include "main.h"
#include "Polden_lidar_main.h"
#include "delay_mks.h"

#define LEN_TRIG_PULSE (30)
#define DELAY_MEASURE (10000)
#define TIMEOUT_MEASURE_MKS (40000)

typedef enum{
	FALLING_EDGE = 0,
	RISING_EDGE
}edge_distance_sensor_t;

typedef enum{
	READY = 0,
	TRIG_SEND,
	WAITING_ECHO,
	RISING_ECHO,
	FALLING_ECHO,
	DONE_SIGNAL_CAPTURE,
	CALCULATE,
	TIMEOUT_MEASURE,
	DONE
}stages_distance_sensor_t;

typedef struct{

	GPIO_TypeDef *trig_port;
	uint16_t trig_pin;

	GPIO_TypeDef *echo_port;
	uint16_t echo_pin;

	TIM_HandleTypeDef* tmr;
	uint32_t ch_rising;
	uint32_t ch_falling;

	volatile stages_distance_sensor_t stage_measure;

	volatile uint32_t echo_rising;
	volatile uint32_t echo_falling;

	volatile uint32_t distance_mm;

	volatile uint32_t tmr_delay_measure;

}distance_sensor_t;


stages_distance_sensor_t ultrasonic_state_mashine(distance_sensor_t* sensor);
void ultrasonic_callback(distance_sensor_t* sensor, uint32_t ch, edge_distance_sensor_t edge);
uint32_t get_triggered_channel(TIM_HandleTypeDef *htim);
volatile uint32_t* get_ccr_pointer(TIM_TypeDef* tim, uint32_t channel) ;

#endif /* INC_SENSOR_H_ */
