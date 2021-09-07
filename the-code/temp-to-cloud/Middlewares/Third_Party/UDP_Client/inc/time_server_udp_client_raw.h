#ifndef THIRD_PARTY_UDP_CLIENT_INC_TIME_SERVER_UDP_CLIENT_RAW_H_
#define THIRD_PARTY_UDP_CLIENT_INC_TIME_SERVER_UDP_CLIENT_RAW_H_

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "lwip/api.h"

#include "rtc_datetime_set_get.h"
#include "global_functions.h"

#include "stdio.h"
#include "string.h"

void UdpClientConnect(void);

void UdpClientSend(void);

void UdpReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

uint8_t UdpVirtualServerConnect(void);

void UdpSendToVirtualServer(uint32_t *data);

void UdpVirtualServerCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

uint8_t UdpVirtualServerConnected(void);

void UdpVirtSrvrConnectNetconn(void);
void UdpVirtSrvrDisconnectNetconn(void);

void UdpSendToVrtlSrvrNetconn(void);



#endif /* THIRD_PARTY_UDP_CLIENT_INC_TIME_SERVER_UDP_CLIENT_RAW_H_ */
