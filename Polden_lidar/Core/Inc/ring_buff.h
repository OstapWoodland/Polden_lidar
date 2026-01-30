/*
 * ring_buff.h
 *
 *  Created on: Jan 28, 2026
 *      Author: Ostap
 */

#ifndef INC_RING_BUFF_H_
#define INC_RING_BUFF_H_

#include "main.h"
#include "Polden_lidar_main.h"

#define RING_BUFF_SIZE (512) //размер буффера можно заложить в зависимости от доступных ресурсов


typedef enum{
    BUFF_OK = 0,
    BUFF_OVERFLOW,
    BUFF_INVALID,
	BUFF_IS_FULL,
    BUFF_EMPTY
}ring_buff_result_t;


typedef struct{
    uint8_t *buff;//руками создаем буффер и записываем по указателю
    uint32_t size_buff;//размер тож ручками

    uint32_t start_pos;
    volatile uint32_t end_pos;

    volatile uint32_t current_size_buff;

    volatile uint8_t flag_overflow_buff;

}ring_buff_t;//объявлять как volatile


typedef struct{
	ring_buff_t ring_buff_u8t;
	uint8_t size_element;
}universal_ring_buff_t;//тип под разные размеры данных, при использовании руками приводим к нужному типу


ring_buff_result_t init_ring_buff(ring_buff_t *ring_buff, uint8_t *buff, uint32_t buff_size);
ring_buff_result_t put_byte_to_buff(ring_buff_t *ring_buff, uint8_t byte_to_put);
ring_buff_result_t get_byte_to_buff(ring_buff_t *ring_buff, uint8_t *byte_to_get);

ring_buff_result_t init_universal_ring_buff(universal_ring_buff_t *ring_buff, uint8_t *buff, uint32_t buff_size, uint8_t element_size);//размер буффера делаем кратным размеру элемента
ring_buff_result_t put_element_to_buff(universal_ring_buff_t *ring_buff, uint8_t *element_to_put);
ring_buff_result_t get_element_to_buff(universal_ring_buff_t *ring_buff, uint8_t *element_to_get);

#endif /* INC_RING_BUFF_H_ */
