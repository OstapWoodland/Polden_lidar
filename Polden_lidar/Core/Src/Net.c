/*
 * Net.c
 *
 *  Created on: Sep 25, 2025
 *      Author: Ostap
 */


#include <Net.h>



wiz_NetInfo g_WIZNETINFO;
uint8_t         mac[6] =        {0x40, 0x08, 0xDC, 0x00, 0xAB, 0xCD},
                ip[4] =         {192, 168, 0, 155},
                netmask[4] =    {255, 255, 255, 0},
                gateway[4] =    {192, 168, 0, 250},
                dnsserv[4] =    {0, 0, 0, 0};
extern SPI_HandleTypeDef        W5500_USING_SPI;



connect_stat_t connect_state = DISCONNECTED;



void W5500_select(void)
{
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
}



void W5500_deselect(void)
{
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
}



void W5500_reset(void)
{
    HAL_GPIO_WritePin(W5500_RES_GPIO_Port, W5500_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(W5500_RES_GPIO_Port, W5500_RES_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
}



void W5500_ReadBuff(uint8_t* buff, uint16_t len)
{
    HAL_SPI_Receive(&W5500_USING_SPI, buff, len, HAL_MAX_DELAY);
}



void W5500_WriteBuff(uint8_t* buff, uint16_t len)
{
    HAL_SPI_Transmit(&W5500_USING_SPI, buff, len, HAL_MAX_DELAY);
}



void W5500_WriteByte(uint8_t byte)
{
    W5500_WriteBuff( &byte, sizeof(byte) );
}



uint8_t W5500_ReadByte(void)
{
    uint8_t byte;
    W5500_ReadBuff( &byte, sizeof(byte) );
    return byte;
}



uint8_t net_init(void)
{


    W5500_reset();


    reg_wizchip_cs_cbfunc( W5500_select, W5500_deselect );
    reg_wizchip_spi_cbfunc( W5500_ReadByte , W5500_WriteByte );
    reg_wizchip_spiburst_cbfunc( W5500_ReadBuff, W5500_WriteBuff );


    uint8_t W5500_fifo_size[2][8] = { {2, 2, 2, 2, 2, 2, 2, 2},
                                      {2, 2, 2, 2, 2, 2, 2, 2} };
    if ( ctlwizchip( CW_INIT_WIZCHIP , (void*)W5500_fifo_size ) == -1 )
    {
        return W5500_ERR;
    }


    for ( uint8_t i = 0; i <= 5; ++i )
    {
        g_WIZNETINFO.mac[i] = mac[i];
    }
    for ( uint8_t i = 0; i <= 3; ++i)
    {
        g_WIZNETINFO.ip[i] = ip[i];
        g_WIZNETINFO.sn[i] = netmask[i];
        g_WIZNETINFO.gw[i] = gateway[i];
        g_WIZNETINFO.dns[i] = dnsserv[i];
    }
    g_WIZNETINFO.dhcp = NETINFO_STATIC;
    wizchip_setnetinfo(&g_WIZNETINFO);


    wizchip_getnetinfo(&g_WIZNETINFO);
    for ( uint8_t i = 0; i <= 5; ++i )
    {
        if ( g_WIZNETINFO.mac[i] != mac[i] ) return W5500_ERR;
    }
    for ( uint8_t i = 0; i <= 3; ++i)
    {
        if ( g_WIZNETINFO.ip[i] != ip[i] ) return W5500_ERR;
        if ( g_WIZNETINFO.sn[i] != netmask[i] ) return W5500_ERR;
        if ( g_WIZNETINFO.gw[i] != gateway[i] ) return W5500_ERR;
        if ( g_WIZNETINFO.dns[i] != dnsserv[i] ) return W5500_ERR;
    }


    uint8_t phycfgr_stat = 0;
    do
	{
    	if(ctlwizchip(CW_GET_PHYLINK, (void*) &phycfgr_stat ) == -1 );
   	}
    while(phycfgr_stat == PHY_LINK_OFF);

    return W5500_OK;
}



uint8_t net_tcp_serv_open(uint8_t socket_number, uint32_t port)
{


    uint8_t io_mode = SOCK_IO_NONBLOCK;
    if ( ctlsocket(socket_number, CS_SET_IOMODE, &io_mode ) == SOCKERR_ARG )
        return W5500_ERR;



    uint8_t retval = socket( socket_number, Sn_MR_TCP, port, 0);
    if ( retval != socket_number ) return W5500_ERR;

    do
    {
        HAL_Delay(10);
    }
    while ( getSn_SR(socket_number) != SOCK_INIT );

    retval = listen(socket_number);
    if ( retval != SOCK_OK ) return W5500_ERR;

    do
    {
        HAL_Delay(10);
    }
    while ( getSn_SR(socket_number) != SOCK_LISTEN ) ;


    return W5500_OK;
}



uint8_t net_tcp_serv_waiting_connection(uint8_t socket_number)
{

	int sock_state = getSn_SR(socket_number);
	if(sock_state == SOCK_CLOSE_WAIT) disconnect(socket_number);
	if( sock_state != SOCK_ESTABLISHED ) net_tcp_serv_close(socket_number);

    while ( sock_state != SOCK_ESTABLISHED )
    {

    	sock_state = getSn_SR(socket_number);
    	if(sock_state == SOCK_CLOSED)
    	{
    		net_tcp_serv_open(socket_number, 50505);
    	}
    	__NOP();
    }



    uint8_t remoteIP[4];
    uint16_t remotePort;
    getsockopt(socket_number, SO_DESTIP, remoteIP);
    getsockopt(socket_number, SO_DESTPORT, &remotePort);


    connect_state = CONNECTED;


    return W5500_OK;
}



int32_t net_tcp_serv_rx(uint8_t socket_number, uint8_t *buff, uint16_t size_packet)
{

	int32_t ret = 0;
	//if(getSn_RX_RSR(user_socket) > 0) Или лучше так???
    if((getSn_IR(socket_number) & Sn_IR_RECV))
    {
    	setSn_IR(socket_number, Sn_IR_RECV);
        while ((getSn_IR(socket_number) & Sn_IR_RECV));

        ret = recv(socket_number, buff, 1024);

        if ( ret < 0 )
        {
            close(socket_number);
            return -1;
        }
    }
    else
    {
    	return 0;
    }


    return ret;
}



uint8_t net_tcp_serv_tx(uint8_t socket_number, char *buff, uint16_t size_packet)
{
    uint32_t ret = 0;
    ret = send(socket_number, (uint8_t*)buff, size_packet);
    if (ret < 0)
    {

        close(socket_number);
        return W5500_ERR;
    }
    else
    {
        return W5500_OK;
    }
}



uint8_t net_tcp_serv_close(uint8_t socket_number)
{
    if ( close(socket_number) != SOCK_OK ) return W5500_ERR;

    connect_state = DISCONNECTED;

    return W5500_OK;
}








uint8_t net_udp_open(uint8_t socket_number, uint16_t local_port)
{
    uint8_t io_mode = SOCK_IO_NONBLOCK;

    int8_t ret = socket(socket_number, Sn_MR_UDP, local_port, 0);
    if (ret != socket_number)
        return W5500_ERR;

    if (ctlsocket(socket_number, CS_SET_IOMODE, &io_mode) == SOCKERR_ARG)
        return W5500_ERR;


    uint32_t timeout = 100000;
    do
    {
    	timeout--;
    }
    while (getSn_SR(socket_number) != SOCK_UDP && timeout > 0);

    if(getSn_SR(socket_number) != SOCK_UDP) return W5500_ERR;

    return W5500_OK;
}

int32_t net_udp_rx(uint8_t socket_number,
                   uint8_t *buff,
                   uint16_t buff_size,
                   uint8_t *remote_ip,
                   uint16_t *remote_port)
{
	int32_t ret = 0;

    //if((getSn_IR(socket_number) & Sn_IR_RECV))
    //{
    	//setSn_IR(socket_number, Sn_IR_RECV);
        //while ((getSn_IR(socket_number) & Sn_IR_RECV))
        //{
        	//HAL_Delay(1);
        //}

        ret = recvfrom(socket_number, buff, buff_size, remote_ip, remote_port);

        if (ret == SOCK_BUSY)
        {
        // неблокирующий режим, данных нет
        	return 0;
        }

        if (ret < 0)
        {
        // здесь по желанию обработать ошибки, например SOCKERR_SOCKCLOSED
        	return ret;
        }
    	//}
    	return ret;  // количество принятых байт
}

uint8_t net_udp_tx(uint8_t socket_number,
                   uint8_t *buff,
                   uint16_t size,
                   uint8_t *remote_ip,
                   uint16_t remote_port)
{
    int32_t ret = sendto(socket_number, buff, size, remote_ip, remote_port);

    if (ret == SOCK_BUSY)
    {
        // буфер занят, можно повторить позже
        return W5500_ERR;
    }

    if (ret < 0)
    {
        return W5500_ERR;
    }

    if ((uint16_t)ret != size)
    {
        return W5500_ERR;
    }

    return W5500_OK;
}

uint8_t net_udp_close(uint8_t socket_number)
{
    if (close(socket_number) != SOCK_OK)
        return W5500_ERR;

    return W5500_OK;
}

