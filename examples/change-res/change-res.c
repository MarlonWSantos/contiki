#include "contiki.h"
#include <stdio.h> 
#include "resources/res-range-events.h"
#include "rest-engine.h"

extern resource_t res_range_events;

/*---------------------------------------------------------------------------*/
PROCESS(change_res_process, "Change-res");
AUTOSTART_PROCESSES(&change_res_process);
/*---------------------------------------------------------------------------*/
  

PROCESS_THREAD(change_res_process, ev, data)
{
	PROCESS_BEGIN();

  	rest_init_engine();
  	rest_activate_resource(&res_range_events,"res_range_events");

	range=10;
	count=1;

	static struct etimer et;
	while(1) {
		etimer_set(&et, CLOCK_SECOND*5);
		PROCESS_WAIT_EVENT();
		printf("[%d]Range: %d\n",count,range);
		count++;
		if(etimer_expired(&et)) {
			etimer_reset(&et);
		}
	}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
