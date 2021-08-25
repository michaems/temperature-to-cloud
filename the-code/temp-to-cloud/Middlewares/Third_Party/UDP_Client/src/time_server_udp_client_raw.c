#include "time_server_udp_client_raw.h"

const uint8_t time_server_addr[4] = {85, 21, 78, 23};
const uint8_t dest_port = 123;

const uint8_t NTP_PACKET_SIZE = 48;

const char * dest_name = "pool.ntp.org";

struct udp_pcb *upcb;
char   received_data[48];

void UdpClientConnect(void)
{
	err_t err;

	upcb = udp_new();

	ip_addr_t self_ip_addr;
	IP_ADDR4(&self_ip_addr, 192, 168, 0, 12); //todo: put this to globals
	udp_bind(upcb, &self_ip_addr, 8);


	/*configure dest ip and port number*/
	ip_addr_t dest_ip_addr;

	IP_ADDR4(&dest_ip_addr, time_server_addr[0], time_server_addr[1], time_server_addr[2], time_server_addr[3]);


	err = udp_connect(upcb, &dest_ip_addr, dest_port);

	if (err == ERR_OK)
	{
		UdpClientSend();

		udp_recv(upcb, UdpReceiveCallback, NULL);
	}

}

void UdpClientSend(void)
{
	struct pbuf *txBuf;

	char packet_to_send[NTP_PACKET_SIZE];

	uint8_t len = NTP_PACKET_SIZE;

	memset(packet_to_send, 0, NTP_PACKET_SIZE);

	packet_to_send[0]  = 0x00 | (4<<3) | 0x03;
	packet_to_send[1]  = 1;
	packet_to_send[2]  = 6;
	packet_to_send[3]  = 0xEC;
	packet_to_send[12] = 49;
	packet_to_send[13] = 0x4E;
	packet_to_send[14] = 49;
	packet_to_send[15] = 52;


	txBuf = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);

	if (txBuf != NULL)
	{
		pbuf_take(txBuf, packet_to_send, len);

		udp_send(upcb, txBuf);

		pbuf_free(txBuf);
	}
}

void UdpReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	memset(received_data, 0, NTP_PACKET_SIZE);

	strncpy(received_data, (char *)p->payload, p->len);

	time_t received_time = (received_data[40] << 24);
	received_time |= (received_data[41] << 16);
	received_time |= (received_data[42] << 8);
	received_time |= (received_data[43] << 0);

	//todo: fix this later. Currently 19-08-2021 10:15:30 local time (GMT + 3 for Helsinki)
	if (received_time == 0)
	{
		Rtc_Set_DateTime();
		Set_Ntp_Time_Acquired_From_Time_Server();
	}

	pbuf_free(p);
}


