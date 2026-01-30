/*
 * ring_buff.c
 *
 *  Created on: Jan 28, 2026
 *      Author: Ostap
 */

#include "ring_buff.h"

ring_buff_result_t init_ring_buff(ring_buff_t *ring_buff, uint8_t *buff, uint32_t buff_size)
{
    if(ring_buff == NULL || buff == NULL || buff_size == 0) return BUFF_INVALID;
    ring_buff_result_t ret = BUFF_OK;

    ring_buff->buff = buff;
    ring_buff->size_buff = buff_size;
    ring_buff->current_size_buff = 0;
    ring_buff->start_pos = 0;
    ring_buff->end_pos = 0;
    ring_buff->flag_overflow_buff = 0;

    return ret;
}



ring_buff_result_t put_byte_to_buff(ring_buff_t *ring_buff, uint8_t byte_to_put)
{
    if(ring_buff == NULL) return BUFF_INVALID;
    ring_buff_result_t ret = BUFF_OK;

    if(ring_buff->current_size_buff == ring_buff->size_buff)return BUFF_OVERFLOW;

    ring_buff->buff[ring_buff->end_pos] = byte_to_put;
    ring_buff->end_pos++;
    if(ring_buff->current_size_buff < ring_buff->size_buff)ring_buff->current_size_buff++;
    if(ring_buff->end_pos >= ring_buff->size_buff) ring_buff->end_pos = 0;

    return ret;
}

ring_buff_result_t get_byte_to_buff(ring_buff_t *ring_buff, uint8_t *byte_to_get)
{
    if(ring_buff == NULL || byte_to_get == NULL) return BUFF_INVALID;
    if(ring_buff->current_size_buff == 0) return BUFF_EMPTY;

    ring_buff_result_t ret = BUFF_OK;

    *byte_to_get = ring_buff->buff[ring_buff->start_pos];

    ring_buff->start_pos++;
    ring_buff->current_size_buff--;
    if(ring_buff->start_pos >= ring_buff->size_buff) ring_buff->start_pos = 0;

    return ret;
}




ring_buff_result_t init_universal_ring_buff(universal_ring_buff_t *ring_buff, uint8_t *buff, uint32_t buff_size, uint8_t element_size)
{
	if(ring_buff == NULL || buff == NULL || buff_size == 0 || element_size == 0) return BUFF_INVALID;
	if((buff_size%element_size)) return BUFF_INVALID; //проверяем что размер буффера кратен размеру элемента, т.е. все влезет
    ring_buff_result_t ret = BUFF_OK;

    ring_buff->size_element = element_size;
    ret = init_ring_buff(&(ring_buff->ring_buff_u8t), buff, buff_size);

    return ret;
}


ring_buff_result_t put_element_to_buff(universal_ring_buff_t *ring_buff, uint8_t *element_to_put)
{
	if(ring_buff == NULL) return BUFF_INVALID;
	if((ring_buff->ring_buff_u8t.size_buff - ring_buff->ring_buff_u8t.current_size_buff) < ring_buff->size_element) return BUFF_IS_FULL;
	ring_buff_result_t ret = BUFF_OK;

	uint8_t byte_for_buff = 0;
	for(int i = 0; i < ring_buff->size_element; i++)
	{
		byte_for_buff = element_to_put[i];
		ret = put_byte_to_buff(&(ring_buff->ring_buff_u8t), byte_for_buff);

		if(ret != BUFF_OK) break;
	}

	return ret;
}


ring_buff_result_t get_element_to_buff(universal_ring_buff_t *ring_buff, uint8_t *element_to_get)
{
	if(ring_buff == NULL) return BUFF_INVALID;
	ring_buff_result_t ret = BUFF_OK;

	uint8_t byte_from_buff = 0;
	for(int i = 0; i < ring_buff->size_element; i++)
	{
		//byte_from_buff = (uint8_t)(element_to_put >> (8*i));
		ret = get_byte_to_buff(&(ring_buff->ring_buff_u8t), &byte_from_buff);

		element_to_get[i] = byte_from_buff;

		if(ret != BUFF_OK) break;
	}

	return ret;
}
















