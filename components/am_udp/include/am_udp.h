#ifndef _AM_UDP_H_
#define _AM_UDP__H_

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

#define configECHO_SERVER_ADDR0              192
#define configECHO_SERVER_ADDR1              168
#define configECHO_SERVER_ADDR2              2
#define configECHO_SERVER_ADDR3              7

void udp_req_init();
void vStandardSendExample();
Socket_t am_udp_create_socket(uint16_t src_port);
void am_udp_close_socket(Socket_t socket);
int am_udp_send_msg(Socket_t socket, const char * host, uint16_t host_port, char * msg, uint16_t len);
int am_udp_recv_msg(Socket_t socket, const char * host, uint16_t host_port, char * buffer, uint16_t buflen);

#endif