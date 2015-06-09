
extern "C" {
	#include <uip.h>
	#include <httpd.h>
	#include <uip_arp.h>
	#include <timer.h>
	#include "clock-arch.h"
}

#include <AppThreads.h>
#include <string.h>
#include <stdio.h>
#include <Ethernet.h>
#include <LPCTimer.h>
#include <Scheduler.h>


extern "C" void Http_RequestPayload_Handler(char * payload) {

}


extern "C" unsigned short Http_Generate_payload(struct request_struct * request) {

	if (strncmp(&request->s->filename[1], "logs.shtml", 5) == 0) {
		uint8_t temp = data.temperatures->Dequeue();

		if (temp == 0) {
			temp = data.temperature;
		}

		return snprintf((char *)request->buffer, UIP_APPDATA_SIZE, "%d&#186;", temp);
	}

	return 0;
}

void tapdev_init() {
	Ethernet::Init();
}

u16_t tapdev_read() {
	return Ethernet::Receive(uip_buf, UIP_BUFSIZE);
}

void tapdev_send() {
	Ethernet::Send(uip_buf, uip_len);
}

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

extern "C" clock_time_t clock_time(void) {
	return (clock_time_t) Timer::GetTickCount(SYSTICK);
}

void WebThread( void * pvParameters ) {
	int i;
	uip_ipaddr_t ipaddr;
	struct timer periodic_timer, arp_timer;

	timer_set(&periodic_timer, CLOCK_SECOND / 2);
	timer_set(&arp_timer, CLOCK_SECOND * 10);

	tapdev_init();
	uip_init();

	uip_ipaddr(ipaddr, 192, 168, 3, 20);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, 192, 168, 3, 1);
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
		} else {

			//
			// If there's no message allow other threads to run.
			Scheduler::Yield();
		}
	}
}

void uip_log(char *m) {}
