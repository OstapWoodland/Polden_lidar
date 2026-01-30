/*
 * sensor.c
 *
 *  Created on: Jan 27, 2026
 *      Author: Ostap
 */

#include "sensor.h"


static void ultrasonic_trig(distance_sensor_t* sensor);
static void ultrasonic_start_capture_echo(distance_sensor_t* sensor);
static void ultrasonic_stop_capture_echo(distance_sensor_t* sensor);
static uint32_t ultrasonic_calc_distance(distance_sensor_t* sensor);
static int8_t ultrasonic_check_timeout(distance_sensor_t* sensor);


volatile uint32_t* get_ccr_pointer(TIM_TypeDef* tim, uint32_t channel) {
    switch(channel) {
        case TIM_CHANNEL_1: return &tim->CCR1;
        case TIM_CHANNEL_2: return &tim->CCR2;
        case TIM_CHANNEL_3: return &tim->CCR3;
        case TIM_CHANNEL_4: return &tim->CCR4;
        default: return NULL;
    }
}


uint32_t get_triggered_channel(TIM_HandleTypeDef *htim) {

	HAL_TIM_ActiveChannel ch = htim->Channel;

    if (ch == HAL_TIM_ACTIVE_CHANNEL_1) return TIM_CHANNEL_1;
    if (ch == HAL_TIM_ACTIVE_CHANNEL_2) return TIM_CHANNEL_2;
    if (ch == HAL_TIM_ACTIVE_CHANNEL_3) return TIM_CHANNEL_3;
    if (ch == HAL_TIM_ACTIVE_CHANNEL_4) return TIM_CHANNEL_4;

    return 0;
}



stages_distance_sensor_t ultrasonic_state_mashine(distance_sensor_t* sensor)
{
	switch(sensor->stage_measure)
	{
	case READY:
		if((get_ticks_mks() - sensor->tmr_delay_measure) >= DELAY_MEASURE)
		{
			sensor->stage_measure = TRIG_SEND;
		}
		break;

	case TRIG_SEND:

		sensor->echo_rising = 0;
		sensor->echo_falling = 0;

		ultrasonic_trig(sensor);
		ultrasonic_start_capture_echo(sensor);

		sensor->stage_measure = WAITING_ECHO;
		sensor->tmr_delay_measure = get_ticks_mks();
		break;

	case WAITING_ECHO:

		sensor->stage_measure = RISING_ECHO;

		break;

	case RISING_ECHO:

		if(ultrasonic_check_timeout(sensor) != 0)sensor->stage_measure = TIMEOUT_MEASURE;

		if(sensor->echo_rising > 0)
		{
			sensor->stage_measure = FALLING_ECHO;
		}
		break;

	case FALLING_ECHO:

		if(ultrasonic_check_timeout(sensor) != 0)sensor->stage_measure = TIMEOUT_MEASURE;

		if(sensor->echo_falling > 0)
		{
			sensor->stage_measure = DONE_SIGNAL_CAPTURE;
		}
		break;

	case DONE_SIGNAL_CAPTURE:

		ultrasonic_stop_capture_echo(sensor);
		sensor->stage_measure = CALCULATE;

		break;

	case CALCULATE:

		sensor->distance_mm = ultrasonic_calc_distance(sensor);
		sensor->stage_measure = DONE;

		break;

	case DONE:

		sensor->tmr_delay_measure = get_ticks_mks();
		sensor->stage_measure = READY;

		break;

	case TIMEOUT_MEASURE:
		ultrasonic_stop_capture_echo(sensor);
		sensor->tmr_delay_measure = get_ticks_mks();
		sensor->stage_measure = READY;
		break;
	}

	return sensor->stage_measure;
}



void ultrasonic_callback(distance_sensor_t* sensor, uint32_t ch, edge_distance_sensor_t edge)
{
	uint32_t capture_value = 0;

	if(sensor->stage_measure == WAITING_ECHO || sensor->stage_measure == RISING_ECHO || sensor->stage_measure == FALLING_ECHO)
	{
	    volatile uint32_t* ccr = get_ccr_pointer(sensor->tmr->Instance, ch);
	    if (ccr) capture_value = *ccr;


		if(ch == sensor->ch_rising)
		{
			sensor->echo_rising = capture_value;
		}
		else if(ch == sensor->ch_falling)
		{
			sensor->echo_falling = capture_value;
		}
	}
}



static void ultrasonic_trig(distance_sensor_t* sensor)
{
	sensor->trig_port->BSRR = sensor->trig_pin;
	delay_mks(LEN_TRIG_PULSE);
	sensor->trig_port->BSRR = sensor->trig_pin << 16;
}


static void ultrasonic_start_capture_echo(distance_sensor_t* sensor)
{
	HAL_TIM_IC_Start_IT(sensor->tmr, sensor->ch_rising);
	HAL_TIM_IC_Start_IT(sensor->tmr, sensor->ch_falling);
}

static void ultrasonic_stop_capture_echo(distance_sensor_t* sensor)
{
	HAL_TIM_IC_Stop_IT(sensor->tmr, sensor->ch_rising);
	HAL_TIM_IC_Stop_IT(sensor->tmr, sensor->ch_falling);
}

static uint32_t ultrasonic_calc_distance(distance_sensor_t* sensor)
{
	uint32_t length_mm = ((65535 + sensor->echo_falling - sensor->echo_rising) % 65535) / 5.8;
	return length_mm;
}

static int8_t ultrasonic_check_timeout(distance_sensor_t* sensor)
{
	int8_t ret = 0;
	if((get_ticks_mks() - sensor->tmr_delay_measure) > (uint32_t)TIMEOUT_MEASURE_MKS)
	{
		ret = -1;
	}
	return ret;
}
