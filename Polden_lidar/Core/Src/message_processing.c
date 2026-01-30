/*
 * message_processing.c
 *
 *  Created on: Jan 28, 2026
 *      Author: Ostap
 */

#include "message_processing.h"

static char header[3] = {0x01, 0x02, 0x03};


static uint32_t destport = 50505;
static uint8_t user_socket = 0;

static message_processing_error_t processing_input_message(uint8_t *buff, uint16_t buff_size, message_t *msg_struct);
static void processing_output_message(uint8_t *buff, message_t *msg_struct, uint16_t len);

message_processing_error_t processing_message(message_processing_t *message_processing)
{
	message_processing_error_t ret = NET_ERR_OK;

	switch(message_processing->stage)
	{
		case NET_W5500_INIT:

			message_processing->__flag_net_init = 0;

			if(net_init() == W5500_OK)
			{
				memcpy(message_processing->last_msg_struct_output.header, header, HEADER_SIZE);
				message_processing->last_msg_struct_output.counter = 0;
				message_processing->__flag_net_init = 1;
				message_processing->port = destport;
				message_processing->socket = user_socket;
				message_processing->stage = NET_SOCK_INIT;
			}

			break;

		case NET_SOCK_INIT:

			if(net_udp_open(message_processing->socket, message_processing->port) == W5500_OK)
			{
				message_processing->__flag_socket_opened = 1;
				message_processing->stage = NET_WAITING;
			}
			else
			{
				message_processing->__flag_socket_opened = 0;
				message_processing->__flag_net_init = 0;
				net_udp_close(message_processing->socket);
				message_processing->stage = NET_W5500_INIT;
				message_processing->__flag_sender = SENDING_STOP;
			}

			break;

		case NET_WAITING:

			message_processing->stage = NET_CHECK_RECEIVE;

			uint32_t dt_sender = get_ticks_mks() - message_processing->__tmr_sender;
			if(message_processing->__flag_sender == SENDING_OK && dt_sender > DELAYED_SENDER)
			{
				message_processing->stage = NET_PROCESS_DATA_OUT;
				message_processing->__tmr_sender = get_ticks_mks();
			}

			break;

		case NET_CHECK_RECEIVE:

			int32_t size_packet = net_udp_rx(message_processing->socket, message_processing->__buff_rx, sizeof(message_processing->__buff_rx), message_processing->__remote_ip, &(message_processing->__remote_port));
			if(size_packet < 0)
			{
				message_processing->stage = NET_SOCK_CLOSE;
				ret = NET_ERR_RX;
			}
			else if(size_packet > 0)
			{
				message_processing->__size_last_receive_packet = size_packet;
				message_processing->stage = NET_PROCESS_RECV;
			}
			else
			{
				message_processing->stage = NET_WAITING;
			}

			break;

		case NET_PROCESS_RECV:

			ret = processing_input_message(message_processing->__buff_rx, message_processing->__size_last_receive_packet, &(message_processing->last_msg_struct_input));

			if(ret == NET_ERR_OK)
			{
				if(message_processing->last_msg_struct_input.payload[0] == 1)
				{
					message_processing->__flag_sender = SENDING_OK;
					message_processing->__tmr_sender = get_ticks_mks();
				}
				else
				{
					message_processing->__flag_sender = SENDING_STOP;
				}
			}

			message_processing->stage = NET_WAITING;

			break;

		case NET_PROCESS_DATA_OUT:

			processing_output_message(message_processing->__buff_tx, &(message_processing->last_msg_struct_output), (HEADER_SIZE + 4 + message_processing->last_msg_struct_output.payload_size));
			message_processing->stage = NET_SEND_DATA;

			break;

		case NET_SEND_DATA:

            if (net_udp_tx(message_processing->socket, message_processing->__buff_tx, (HEADER_SIZE + 4 + message_processing->last_msg_struct_output.payload_size), message_processing->__remote_ip, message_processing->__remote_port) != W5500_OK)
            {
            	ret = NET_ERR_TX;
            }
            else
            {
            	message_processing->__tmr_sender = get_ticks_mks();
            	message_processing->last_msg_struct_output.counter++;
            }

            message_processing->stage = NET_WAITING;

			break;

		case NET_SOCK_CLOSE:

			net_udp_close(message_processing->socket);
			message_processing->stage = NET_SOCK_INIT;

			break;
	}

	return ret;
}

static message_processing_error_t processing_input_message(uint8_t *buff, uint16_t buff_size, message_t *msg_struct)
{

	//if(buff_size < MESSAGE_SIZE) return (MESSAGE_SIZE - buff_size);
	if(memcmp(buff, header, HEADER_SIZE) != 0)
	{
		return NET_ERR_HEADER;
	}

	uint8_t checksum = 0;

	for(int i = 0; i < (buff_size - 1); i++) checksum += buff[i];

	uint16_t current_ptr = 0;
	memcpy(msg_struct->header, buff + current_ptr, HEADER_SIZE); current_ptr += HEADER_SIZE;
	memcpy(&msg_struct->counter, buff + current_ptr, 1); current_ptr += 1;
	memcpy(&msg_struct->payload_size, buff + current_ptr, sizeof(msg_struct->payload_size)); current_ptr += sizeof(msg_struct->payload_size);
	memcpy(&msg_struct->payload, buff + current_ptr, msg_struct->payload_size); current_ptr += msg_struct->payload_size;
	memcpy(&msg_struct->checksum, buff + current_ptr, 1); current_ptr += 1;

	if(checksum != msg_struct->checksum)
	{
		return NET_ERR_CHECKSUM;
	}


	return NET_ERR_OK;
}

static void processing_output_message(uint8_t *buff, message_t *msg_struct, uint16_t len)
{
	uint16_t current_ptr = 0;

	memcpy(buff + current_ptr, msg_struct->header, HEADER_SIZE); current_ptr += HEADER_SIZE;
	memcpy(buff + current_ptr, &msg_struct->counter, 1); current_ptr += 1;
	memcpy(buff + current_ptr, &msg_struct->payload_size, sizeof(msg_struct->payload_size)); current_ptr += sizeof(msg_struct->payload_size);
	memcpy(buff + current_ptr, &msg_struct->payload, msg_struct->payload_size); current_ptr += msg_struct->payload_size;

	uint8_t checksum = 0;
	for(int i = 0; i < (len - 1); i++) checksum += buff[i];
	buff[current_ptr] = checksum;

}
