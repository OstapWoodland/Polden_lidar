
#ifndef INC_MESSAGE_PROCESSING_H_
#define INC_MESSAGE_PROCESSING_H_


#include "main.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Polden_lidar_main.h"

#include "Net.h"
#include "delay_mks.h"

#define MAX_PAYLOAD_SIZE (100)
#define HEADER_SIZE  (3)
#define MESSAGE_SIZE (HEADER_SIZE + 11)

#define MESSAGE_ERR_HEADER   (-1)
#define MESSAGE_ERR_CHECKSUM (-2)
#define MESSAGE_ERR_OK        (0)

#define DELAYED_SENDER (34000)

typedef enum{
	NET_ERR_OK,
	NET_ERR_RX,
	NET_ERR_TX,
	NET_ERR_HEADER,
	NET_ERR_CHECKSUM
}message_processing_error_t;

typedef struct{

	char header[3];
	uint8_t counter;
	uint16_t payload_size;
	uint8_t payload[MAX_PAYLOAD_SIZE];
	uint8_t checksum;

}message_t;


typedef enum{
	SENDING_OK,
	SENDING_STOP
}state_sender_t;

typedef enum{
	NET_W5500_INIT,
	NET_SOCK_INIT,
	NET_WAITING,
	NET_CHECK_RECEIVE,
	NET_PROCESS_RECV,
	NET_PROCESS_DATA_OUT,
	NET_SEND_DATA,
	NET_SOCK_CLOSE
}message_processing_stages_t;


typedef struct{
	message_processing_stages_t stage;

	uint32_t port;
	uint8_t socket;

	message_t last_msg_struct_input;
	message_t last_msg_struct_output;

	uint8_t __buff_rx[1024];
	uint8_t __buff_tx[1024];
	uint8_t __remote_ip[4];
	uint16_t __remote_port;

	int32_t __size_last_receive_packet;

	state_sender_t __flag_sender;
	uint32_t __tmr_sender;

	uint8_t __flag_net_init;
	uint8_t __flag_socket_opened;

}message_processing_t;


message_processing_error_t processing_message(message_processing_t *message_processing);



#endif /* INC_MESSAGE_PROCESSING_H_ */
