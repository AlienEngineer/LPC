extern "C" {
#include <uip.h>
#include <httpd.h>
#include <uip_arp.h>
#include <timer.h>
#include "clock-arch.h"
}

#include <stdlib.h>
#include <AppThreads.h>
#include <string.h>
#include <stdio.h>
#include <Ethernet.h>
#include <LPCTimer.h>

#define LIMIT_INF		9
#define LIMIT_SUP		9
#define CURR_TIME		9
#define LIMIT_TAG		4

#define IsPageRequest(page, size) (strncmp(&request->s->filename[1], page, size) == 0)

const char field_seperator[2] = "&";

char * StartToken(char * buffer) {
	return strtok((buffer), field_seperator);
}

char * NextToken(uint8_t tagSize) {
	return (char *) (strtok(NULL, field_seperator) + tagSize);
}

uint32_t GetNextInt(uint8_t tagSize) {
	return atoi(NextToken(tagSize));
}

char substr[20];

char * GetSubString(char * str, uint8_t start, uint8_t len) {
	strncpy(substr, str + start, len);
	substr[len] = 0;
	return substr;
}

uint32_t GetInt(char * str, uint8_t start, uint8_t len) {
	return atoi(GetSubString(str, start, len));
}

extern "C" void Http_RequestPayload_Handler(struct request_struct * request) {
	StartToken((char*) request->buffer);

	if (strncmp(&request->s->filename[1], "configure.shtml", 10) == 0) {

		context.LimitInf = GetNextInt(LIMIT_INF);
		context.LimitSup = GetNextInt(LIMIT_SUP);

		char * token = NextToken(CURR_TIME);

		uint16_t year = GetInt(token, 0, 4);
		uint16_t month = GetInt(token, 7, 2);
		uint16_t day = GetInt(token, 12, 2);

		uint16_t hour = GetInt(token, 15, 2);
		uint16_t minute = GetInt(token, 20, 2);
		uint16_t second = GetInt(token, 25, 2);

		// DateTime date(year, month, day, hour, minute, second);

		context.RealTimeClock.Config(year, month, day, hour, minute, second);
	}

}

extern "C" unsigned short Http_Generate_payload(
		struct request_struct * request) {

	if (IsPageRequest("logs.shtml", 5)) {

		if (strncmp(request->s->scriptptr, "log-list", 8) == 0) {

			LogEntry entry = context.Entries[request->s->recall];

			if (entry.HasData) {

				request->s->recall++;

				return snprintf((char *) request->buffer, UIP_APPDATA_SIZE,
						"<li class=\"list-group-item\"><span class=\"badge\">%d</span>%03d - %04d/%02d/%02d %02d:%02d:%02d</li>",
						entry.Temperature, request->s->recall, entry.Date.Year,
						entry.Date.Month, entry.Date.Day, entry.Date.Hour,
						entry.Date.Minute, entry.Date.Second);
			} else {
				request->s->recall = 0;
			}
			return 1;
		} else {
			return snprintf((char *) request->buffer, UIP_APPDATA_SIZE,
					"%d&#186;", context.CurrentTemperature);
		}

	} else if (IsPageRequest("configure.shtml", 10)) {
		DateTime * date = context.RealTimeClock.GetDate();

		return snprintf((char *) request->buffer, UIP_APPDATA_SIZE,
				"{ limitInf: %d, limitSup: %d, date: '%04d/%02d/%02d %02d:%02d:%02d' }",
				context.LimitInf, context.LimitSup, date->Year, date->Month,
				date->Day, date->Hour, date->Minute, date->Second);
	}

	return 1;
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

void WebThread(void * pvParameters) {
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
			// If there's no message, allow other threads to run.
			Scheduler::Yield();
		}
	}
}

void uip_log(char *m) {
}
