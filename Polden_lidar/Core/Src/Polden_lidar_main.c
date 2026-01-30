/*
 * Polden_lidar_main.c
 *
 *  Created on: Jan 19, 2026
 *      Author: Ostap
 */
#include "Polden_lidar_main.h"
#include "sensor.h"
#include "Net.h"
#include "message_processing.h"

#define SENSORS_COUNT (8)

#define SIZE_UBUFF (10)
#define SIZE_ELEMENT_BUFF (4)



extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;


static uint8_t _buff_sensor_0[SIZE_UBUFF * SIZE_ELEMENT_BUFF];
static uint8_t _buff_sensor_1[SIZE_UBUFF * SIZE_ELEMENT_BUFF];
static uint8_t _buff_sensor_2[SIZE_UBUFF * SIZE_ELEMENT_BUFF];
static uint8_t _buff_sensor_3[SIZE_UBUFF * SIZE_ELEMENT_BUFF];
static uint8_t _buff_sensor_4[SIZE_UBUFF * SIZE_ELEMENT_BUFF];
static uint8_t _buff_sensor_5[SIZE_UBUFF * SIZE_ELEMENT_BUFF];
static uint8_t _buff_sensor_6[SIZE_UBUFF * SIZE_ELEMENT_BUFF];
static uint8_t _buff_sensor_7[SIZE_UBUFF * SIZE_ELEMENT_BUFF];


//--------------------------------DEBUG--------------------------//
uint32_t dbg_distance = 0;
//uint32_t *dgb_buff = (uint32_t *)_buff;
//---------------------------------------------------------------//

static universal_ring_buff_t ring_buff[SENSORS_COUNT];
static distance_sensor_t sensors[SENSORS_COUNT];
static message_processing_t talcker;
static uint32_t tmr_check_talker;
void init_sensors();
void talcker_init();

void polden_lidar_main()
{
	start_tmr_mks();
	talcker_init();
	init_sensors();

	tmr_check_talker = get_ticks_mks();

	while(1)
	{
		for(int i = 0; i < SENSORS_COUNT; i++)
		{
			if(ultrasonic_state_mashine(&sensors[i]) == DONE) put_element_to_buff(&ring_buff[i], (uint8_t*)(&(sensors[i].distance_mm)));
			if(ring_buff[i].ring_buff_u8t.current_size_buff)
			{
				uint32_t distance_to_send = 0;
				if(get_element_to_buff(&ring_buff[i], (uint8_t*)(&distance_to_send)) == BUFF_OK)
				{
					uint8_t *dst = talcker.last_msg_struct_output.payload + (i * SIZE_ELEMENT_BUFF);
					memcpy(dst, &distance_to_send, SIZE_ELEMENT_BUFF);
				}

			}

		}


		//if(get_ticks_mks() - tmr_check_talker > 1000)
		//{
			processing_message(&talcker);
		//	tmr_check_talker = get_ticks_mks();
		//}
	}

}



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	for(int i = 0; i < SENSORS_COUNT; i++)
	{
		if(htim == sensors[i].tmr)
		{
			uint32_t active_chan = get_triggered_channel(htim);

			if(active_chan == sensors[i].ch_rising) ultrasonic_callback(&sensors[i], sensors[i].ch_rising, RISING_EDGE);
			if(active_chan == sensors[i].ch_falling) ultrasonic_callback(&sensors[i], sensors[i].ch_falling, FALLING_EDGE);

		}
	}

}



void talcker_init()
{
	talcker.last_msg_struct_output.payload_size = SENSORS_COUNT * SIZE_ELEMENT_BUFF;
	talcker.stage = NET_W5500_INIT;
	talcker.__flag_sender = SENDING_STOP;

	for(int i = 0; i < talcker.last_msg_struct_output.payload_size; i++) talcker.last_msg_struct_output.payload[i] = 0;
}



void init_sensors()
{
	init_universal_ring_buff(&ring_buff[0], _buff_sensor_0, sizeof(_buff_sensor_0), SIZE_ELEMENT_BUFF);
	init_universal_ring_buff(&ring_buff[1], _buff_sensor_1, sizeof(_buff_sensor_1), SIZE_ELEMENT_BUFF);
	init_universal_ring_buff(&ring_buff[2], _buff_sensor_2, sizeof(_buff_sensor_2), SIZE_ELEMENT_BUFF);
	init_universal_ring_buff(&ring_buff[3], _buff_sensor_3, sizeof(_buff_sensor_3), SIZE_ELEMENT_BUFF);
	init_universal_ring_buff(&ring_buff[4], _buff_sensor_4, sizeof(_buff_sensor_4), SIZE_ELEMENT_BUFF);
	init_universal_ring_buff(&ring_buff[5], _buff_sensor_5, sizeof(_buff_sensor_5), SIZE_ELEMENT_BUFF);
	init_universal_ring_buff(&ring_buff[6], _buff_sensor_6, sizeof(_buff_sensor_6), SIZE_ELEMENT_BUFF);
	init_universal_ring_buff(&ring_buff[7], _buff_sensor_7, sizeof(_buff_sensor_7), SIZE_ELEMENT_BUFF);

	sensors[0].trig_port = ULTRASONIC_1_TRIG_GPIO_Port;
	sensors[0].trig_pin = ULTRASONIC_1_TRIG_Pin;
	sensors[0].echo_port = ULTRASONIC_1_ECHO_GPIO_Port;
	sensors[0].echo_pin = ULTRASONIC_1_ECHO_Pin;
	sensors[0].stage_measure = READY;

	sensors[0].tmr = &htim3;
	sensors[0].ch_rising = TIM_CHANNEL_1;
	sensors[0].ch_falling = TIM_CHANNEL_2;

	sensors[0].trig_port->BSRR = sensors[0].trig_pin << 16;


	sensors[1].trig_port = ULTRASONIC_2_TRIG_GPIO_Port;
	sensors[1].trig_pin = ULTRASONIC_2_TRIG_Pin;
	sensors[1].echo_port = ULTRASONIC_2_ECHO_GPIO_Port;
	sensors[1].echo_pin = ULTRASONIC_2_ECHO_Pin;
	sensors[1].stage_measure = READY;

	sensors[1].tmr = &htim5;
	sensors[1].ch_rising = TIM_CHANNEL_3;
	sensors[1].ch_falling = TIM_CHANNEL_4;

	sensors[1].trig_port->BSRR = sensors[1].trig_pin << 16;


	sensors[2].trig_port = ULTRASONIC_3_TRIG_GPIO_Port;
	sensors[2].trig_pin = ULTRASONIC_3_TRIG_Pin;
	sensors[2].echo_port = ULTRASONIC_3_ECHO_GPIO_Port;
	sensors[2].echo_pin = ULTRASONIC_3_ECHO_Pin;
	sensors[2].stage_measure = READY;

	sensors[2].tmr = &htim3;
	sensors[2].ch_rising = TIM_CHANNEL_3;
	sensors[2].ch_falling = TIM_CHANNEL_4;
	sensors[2].trig_port->BSRR = sensors[2].trig_pin << 16;


	sensors[3].trig_port = ULTRASONIC_4_TRIG_GPIO_Port;
	sensors[3].trig_pin = ULTRASONIC_4_TRIG_Pin;
	sensors[3].echo_port = ULTRASONIC_4_ECHO_GPIO_Port;
	sensors[3].echo_pin = ULTRASONIC_4_ECHO_Pin;
	sensors[3].stage_measure = READY;

	sensors[3].tmr = &htim1;
	sensors[3].ch_rising = TIM_CHANNEL_1;
	sensors[3].ch_falling = TIM_CHANNEL_2;

	sensors[3].trig_port->BSRR = sensors[3].trig_pin << 16;


	sensors[4].trig_port = ULTRASONIC_5_TRIG_GPIO_Port;
	sensors[4].trig_pin = ULTRASONIC_5_TRIG_Pin;
	sensors[4].echo_port = ULTRASONIC_5_ECHO_GPIO_Port;
	sensors[4].echo_pin = ULTRASONIC_5_ECHO_Pin;
	sensors[4].stage_measure = READY;

	sensors[4].tmr = &htim4;
	sensors[4].ch_rising = TIM_CHANNEL_1;
	sensors[4].ch_falling = TIM_CHANNEL_2;

	sensors[4].trig_port->BSRR = sensors[4].trig_pin << 16;


	sensors[5].trig_port = ULTRASONIC_6_TRIG_GPIO_Port;
	sensors[5].trig_pin = ULTRASONIC_6_TRIG_Pin;
	sensors[5].echo_port = ULTRASONIC_6_ECHO_GPIO_Port;
	sensors[5].echo_pin = ULTRASONIC_6_ECHO_Pin;
	sensors[5].stage_measure = READY;

	sensors[5].tmr = &htim1;
	sensors[5].ch_rising = TIM_CHANNEL_3;
	sensors[5].ch_falling = TIM_CHANNEL_4;

	sensors[5].trig_port->BSRR = sensors[5].trig_pin << 16;


	sensors[6].trig_port = ULTRASONIC_7_TRIG_GPIO_Port;
	sensors[6].trig_pin = ULTRASONIC_7_TRIG_Pin;
	sensors[6].echo_port = ULTRASONIC_7_ECHO_GPIO_Port;
	sensors[6].echo_pin = ULTRASONIC_7_ECHO_Pin;
	sensors[6].stage_measure = READY;

	sensors[6].tmr = &htim4;
	sensors[6].ch_rising = TIM_CHANNEL_3;
	sensors[6].ch_falling = TIM_CHANNEL_4;

	sensors[6].trig_port->BSRR = sensors[6].trig_pin << 16;


	sensors[7].trig_port = ULTRASONIC_8_TRIG_GPIO_Port;
	sensors[7].trig_pin = ULTRASONIC_8_TRIG_Pin;
	sensors[7].echo_port = ULTRASONIC_8_ECHO_GPIO_Port;
	sensors[7].echo_pin = ULTRASONIC_8_ECHO_Pin;
	sensors[7].stage_measure = READY;

	sensors[7].tmr = &htim5;
	sensors[7].ch_rising = TIM_CHANNEL_1;
	sensors[7].ch_falling = TIM_CHANNEL_2;

	sensors[7].trig_port->BSRR = sensors[7].trig_pin << 16;

	HAL_Delay(10);
}
















