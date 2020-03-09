/*
 * Copyright (c) 2011, Swedish Institute of Computer Science.
 * Copyright (c) 2020, Federal University of Para.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/**
 * \file
 *      broadcast-unicast
 * \author
 *      Marlon W. Santos <marlon.santos.santos@icen.ufpa.br>
 */

#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ip/uiplib.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "sys/node-id.h"
#include "simple-udp.h"
#include "servreg-hack.h"
#include "net/rpl/rpl.h"
#include <stdio.h>
#include <string.h>
#include "ip64-addr.h"
#include <stdlib.h>

#define UDP_PORT 1234
#define SERVICE_ID 190
#define SEND_INTERVAL		(30 * CLOCK_SECOND)
#define SEND_TIME		(random_rand() % (SEND_INTERVAL))

static struct simple_udp_connection broadcast_connection;
static struct simple_udp_connection sender_connection;
static struct simple_udp_connection receiver_connection;
static uip_ipaddr_t *address;
static unsigned int is_an_event=0;
/*---------------------------------------------------------------------------*/
PROCESS(sender_process,"Sender process");
PROCESS(receiver_process,"Receiver process");
PROCESS(broadcast_unicast_process, "Broadcast-unicast process");
AUTOSTART_PROCESSES(&broadcast_unicast_process);
/*---------------------------------------------------------------------------*/
	static void
receiver(struct simple_udp_connection *c,
		const uip_ipaddr_t *sender_addr,
		uint16_t sender_port,
		const uip_ipaddr_t *receiver_addr,
		uint16_t receiver_port,
		const uint8_t *data,
		uint16_t datalen)
{
	int default_size_message = 11;
	char message[default_size_message];

	memcpy(message,data,datalen);

	printf("Data received from ");
	uip_debug_ipaddr_print(sender_addr);
	printf(" with length %d: '%s'\n",datalen, message);

	address=sender_addr;

	const char* typeMessage = "Some_event?";

	if(strcmp((const char *)data,typeMessage)==0){
		process_start(&sender_process,NULL);
	}
}
/*----------------------------------------------------------------------------*/
PROCESS_THREAD(broadcast_unicast_process, ev, data)
{
	static struct etimer periodic_timer;
	static struct etimer send_timer;
	uip_ipaddr_t addr;

	PROCESS_BEGIN();

	simple_udp_register(&broadcast_connection, UDP_PORT,
			NULL, UDP_PORT,receiver);

	etimer_set(&periodic_timer, SEND_INTERVAL);

	while(1) {

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
		etimer_reset(&periodic_timer);
		etimer_set(&send_timer, SEND_TIME);

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));

		is_an_event=((double) rand() / (RAND_MAX)) + 1;

		printf("Numero gerado: %d\n",is_an_event);

		if(is_an_event==1){
			printf("Sending broadcast\n");
			uip_create_linklocal_allnodes_mcast(&addr);
			simple_udp_sendto(&broadcast_connection,"Some_event?",11,&addr);
		}
		process_start(&receiver_process,NULL);

	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(sender_process, ev, data)
{
	uip_ipaddr_t *addr;

	PROCESS_BEGIN();

	servreg_hack_init();

	simple_udp_register(&sender_connection, UDP_PORT,
			NULL, UDP_PORT, receiver);

	addr = address;

	address=NULL;

	if(addr != NULL) {
		char buf[3];

		printf("Sending unicast to ");
		uip_debug_ipaddr_print(addr);
		printf("\n");
		if(is_an_event==1){
	sprintf(buf,"Yes");
		}else{
	sprintf(buf,"No");
		}
		simple_udp_sendto(&sender_connection, buf, strlen(buf), addr);
	} else {
		printf("Service %d not found\n", SERVICE_ID);
	}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
static uip_ipaddr_t *
set_global_address(void)
{
  static uip_ipaddr_t ipaddr;
  int i;
  uint8_t state;

  uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

  printf("IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      uip_debug_ipaddr_print(&uip_ds6_if.addr_list[i].ipaddr);
      printf("\n");
    }
  }
  return &ipaddr;
}
/*---------------------------------------------------------------------------*/
static void
create_rpl_dag(uip_ipaddr_t *ipaddr)
{
  struct uip_ds6_addr *root_if;

  root_if = uip_ds6_addr_lookup(ipaddr);

  if(root_if != NULL) {
    rpl_dag_t *dag;
    uip_ipaddr_t prefix;
    
    rpl_set_root(RPL_DEFAULT_INSTANCE, ipaddr);
    dag = rpl_get_any_dag();
    uip_ip6addr(&prefix, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
    rpl_set_prefix(dag, &prefix, 64);
    PRINTF("created a new RPL dag\n");
  } else {
    PRINTF("failed to create a new RPL DAG\n");
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(receiver_process, ev, data)
{
  uip_ipaddr_t *ipaddr;

  PROCESS_BEGIN();

  servreg_hack_init();

  ipaddr = set_global_address();

  create_rpl_dag(ipaddr);

  servreg_hack_register(SERVICE_ID, ipaddr);

  simple_udp_register(&receiver_connection, UDP_PORT,
                      NULL, UDP_PORT, receiver);

  while(1) {
    PROCESS_WAIT_EVENT();
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
