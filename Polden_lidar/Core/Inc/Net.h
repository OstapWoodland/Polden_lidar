/*
 * net.h
 *
 *  Created on: Sep 25, 2025
 *      Author: Ostap
 */

#ifndef INC_NET_H_
#define INC_NET_H_



#include <main.h>
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "w5500.h"

#define W5500_USING_SPI     hspi1

#define W5500_GPIO_PORT     GPIOA
#define W5500_GPIO_SCS_PIN  GPIO_PIN_4
#define W5500_GPIO_RST_PIN  GPIO_PIN_3


enum STATE
{
    W5500_OK    = 0,
    W5500_ERR   = 1
};

typedef enum
{
	CONNECTED = 0,
	DISCONNECTED
}connect_stat_t;


uint8_t net_init(void);
uint8_t net_tcp_serv_open(uint8_t socket, uint32_t port);
uint8_t net_tcp_serv_waiting_connection(uint8_t socket_number);
int32_t net_tcp_serv_rx(uint8_t socket_number, uint8_t *buff, uint16_t size_packet);
uint8_t net_tcp_serv_tx(uint8_t socket_number, char *buff, uint16_t size_packet);
uint8_t net_tcp_serv_close(uint8_t socket_number);








uint8_t net_udp_open(uint8_t socket_number, uint16_t local_port);
int32_t net_udp_rx(uint8_t socket_number,
                   uint8_t *buff,
                   uint16_t buff_size,
                   uint8_t *remote_ip,
                   uint16_t *remote_port);
uint8_t net_udp_tx(uint8_t socket_number,
                   uint8_t *buff,
                   uint16_t size,
                   uint8_t *remote_ip,
                   uint16_t remote_port);
uint8_t net_udp_close(uint8_t socket_number);























#endif /* INC_NET_H_ */
