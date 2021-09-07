#include "time_server_udp_client_raw.h"

struct udp_pcb *upcb;
char   received_data[48];
const uint8_t time_server_addr[4] = {85, 21, 78, 23};
const uint8_t dest_port = 123;
const uint8_t NTP_PACKET_SIZE = 48;
const char * dest_name = "pool.ntp.org";


struct udp_pcb *upcb_virtserver;
char   received_data_virtserver[2];
const uint8_t virtual_machine_server[4] = {20, 54, 80, 172};
const uint16_t virtual_machine_port = 20001;
const uint16_t local_port = 20300;
const uint8_t VIRTSERVER_PACKET_SIZE = 8;

uint8_t virtual_server_connected = 0;

/*NETCONN code*/
static struct netconn *conn;
static struct netbuf *buf_send;

static char *data;
unsigned int payload_len;
static ip_addr_t self_ip_addr;
static ip_addr_t server_ip_addr;

/*End of NETCONN code*/

void UdpClientConnect(void)
{
	err_t err;

	upcb = udp_new();

	//ip_addr_t self_ip_addr;
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

uint8_t UdpVirtualServerConnect(void)
{
	err_t err;

	upcb_virtserver = udp_new();

	ip_addr_t self_ip_addr;
	IP_ADDR4(&self_ip_addr, 192, 168, 0, 12); //todo: put this to globals
	udp_bind(upcb_virtserver, &self_ip_addr, local_port);


	/*configure dest ip and port number*/
	ip_addr_t dest_ip_addr;

	IP_ADDR4(&dest_ip_addr, virtual_machine_server[0], virtual_machine_server[1],
			virtual_machine_server[2], virtual_machine_server[3]);

	err = udp_connect(upcb_virtserver, &dest_ip_addr, virtual_machine_port);

	if (err == ERR_OK)
	{
		//UdpSendToVirtualServer(&data);

		udp_recv(upcb_virtserver, UdpVirtualServerCallback, NULL);

		virtual_server_connected = 1;
	}

	return err;
}

void UdpSendToVirtualServer(uint32_t *data)
{
	static struct pbuf *txBuf;

	uint8_t len = VIRTSERVER_PACKET_SIZE;

	char packet_to_send[len];

	memset(packet_to_send, 0, len);

	memcpy(packet_to_send, data, len);

	txBuf = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);

	if (txBuf != NULL)
	{
		pbuf_take(txBuf, packet_to_send, len);

		udp_send(upcb_virtserver, txBuf);

		pbuf_free(txBuf);
	}
}

void UdpVirtualServerCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	memset(received_data, 0, NTP_PACKET_SIZE);

	strncpy(received_data, (char *)p->payload, p->len);

	pbuf_free(p);

}

uint8_t UdpVirtualServerConnected()
{
	return virtual_server_connected;
}

void UdpVirtSrvrConnectNetconn(void)
{
	err_t err;

	IP_ADDR4(&self_ip_addr, 192, 168, 0, 12);

	IP_ADDR4(&server_ip_addr, virtual_machine_server[0], virtual_machine_server[1],
			virtual_machine_server[2], virtual_machine_server[3]);

	conn = netconn_new(NETCONN_UDP);

	if (conn != NULL)
	{
		err = netconn_bind(conn, &self_ip_addr, local_port);

		if (err == ERR_OK)
		{
			err = netconn_connect(conn, &server_ip_addr, virtual_machine_port);

			if (err == ERR_OK)
			{
				virtual_server_connected = 1;
			}
			else
			{
				virtual_server_connected = 0;
			}
		}
	}
}

void UdpVirtSrvrDisconnectNetconn(void)
{
	if (conn != NULL)
	{
		netconn_delete(conn);
		conn = NULL;
	}
}

void UdpSendToVrtlSrvrNetconn(void)
{
	if (UdpVirtualServerConnected() == 1)
	{
		buf_send = netbuf_new();

		payload_len = strlen("Hello SERVER!");
		data = netbuf_alloc(buf_send, payload_len);
		memcpy(data, "Hello SERVER!", payload_len);

		netconn_sendto(conn, buf_send, &server_ip_addr, virtual_machine_port);

		netbuf_delete(buf_send);
		data = NULL;
	}
}

