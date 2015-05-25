/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

extern "C" {
	#include <uip.h>
	#include <uip_arp.h>
	#include <timer.h>
	#include "clock-arch.h"
}

#include <stdio.h>
#include <Ethernet.h>
#include <timers.h>

void tapdev_init() {
	Ethernet::Init();
}

u16_t tapdev_read() {
	return Ethernet::Receive(uip_buf, ETH_FRAG_SIZE);
}

void tapdev_send() {
	Ethernet::Send(uip_buf, UIP_BUFSIZE);
}

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

extern "C" clock_time_t clock_time(void)
{
	return (clock_time_t)Timer::GetTickCount(SYSTICK);
}



int main(void) {

	int i;
		uip_ipaddr_t ipaddr;
		struct timer periodic_timer, arp_timer;

		timer_set(&periodic_timer, CLOCK_SECOND / 2);
		timer_set(&arp_timer, CLOCK_SECOND * 10);

		tapdev_init();
		uip_init();

		uip_ipaddr(ipaddr, 192, 168, 4, 20);
		uip_sethostaddr(ipaddr);
		uip_ipaddr(ipaddr, 192, 168, 4, 1);
		uip_setdraddr(ipaddr);
		uip_ipaddr(ipaddr, 255, 255, 255, 0);
		uip_setnetmask(ipaddr);

		httpd_init();

		while (1) {
			uip_len = tapdev_read();
			if (uip_len > 0) {
				if (BUF->type == htons(UIP_ETHTYPE_IP)) {
					uip_arp_ipin();
					uip_input();
					/* If the above function invocation resulted in data that
					 should be sent out on the network, the global variable
					 uip_len is set to a value > 0. */
					if (uip_len > 0) {
						uip_arp_out();
						tapdev_send();
					}
				} else if (BUF->type == htons(UIP_ETHTYPE_ARP)) {
					uip_arp_arpin();
					/* If the above function invocation resulted in data that
					 should be sent out on the network, the global variable
					 uip_len is set to a value > 0. */
					if (uip_len > 0) {
						tapdev_send();
					}
				}

			} else if (timer_expired(&periodic_timer)) {
				timer_reset(&periodic_timer);
				for (i = 0; i < UIP_CONNS; i++) {
					uip_periodic(i);
					/* If the above function invocation resulted in data that
					 should be sent out on the network, the global variable
					 uip_len is set to a value > 0. */
					if (uip_len > 0) {
						uip_arp_out();
						tapdev_send();
					}
				}

	#if UIP_UDP
				for(i = 0; i < UIP_UDP_CONNS; i++) {
					uip_udp_periodic(i);
					/* If the above function invocation resulted in data that
					 should be sent out on the network, the global variable
					 uip_len is set to a value > 0. */
					if(uip_len > 0) {
						uip_arp_out();
						tapdev_send();
					}
				}
	#endif /* UIP_UDP */

				/* Call the ARP timer function every 10 seconds. */
				if (timer_expired(&arp_timer)) {
					timer_reset(&arp_timer);
					uip_arp_timer();
				}
			}
		}
		return 0;
}



/*---------------------------------------------------------------------------*/
void
uip_log(char *m)
{
  printf("uIP log message: %s\n", m);
}
void
resolv_found(char *name, u16_t *ipaddr)
{
  //u16_t *ipaddr2;

  if(ipaddr == NULL) {
    printf("Host '%s' not found.\n", name);
  } else {
    printf("Found name '%s' = %d.%d.%d.%d\n", name,
	   htons(ipaddr[0]) >> 8,
	   htons(ipaddr[0]) & 0xff,
	   htons(ipaddr[1]) >> 8,
	   htons(ipaddr[1]) & 0xff);
    /*    webclient_get("www.sics.se", 80, "/~adam/uip");*/
  }
}
#ifdef __DHCPC_H__
void
dhcpc_configured(const struct dhcpc_state *s)
{
  uip_sethostaddr(s->ipaddr);
  uip_setnetmask(s->netmask);
  uip_setdraddr(s->default_router);
  resolv_conf(s->dnsaddr);
}
#endif /* __DHCPC_H__ */
void
smtp_done(unsigned char code)
{
  printf("SMTP done with code %d\n", code);
}
void
webclient_closed(void)
{
  printf("Webclient: connection closed\n");
}
void
webclient_aborted(void)
{
  printf("Webclient: connection aborted\n");
}
void
webclient_timedout(void)
{
  printf("Webclient: connection timed out\n");
}
void
webclient_connected(void)
{
  printf("Webclient: connected, waiting for data...\n");
}
void
webclient_datahandler(char *data, u16_t len)
{
  printf("Webclient: got %d bytes of data.\n", len);
}
/*---------------------------------------------------------------------------*/
