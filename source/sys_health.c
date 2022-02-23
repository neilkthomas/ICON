/*
 * sys_health.c
 *
 *  Created on: Feb 14, 2022
 *      Author: Neil
 */


#include "icon_mqx_os.h"
#include "icon_ds.h"
#include <mqx.h>
#include <bsp.h>
#include <watchdog.h>



/*FUNCTION*------------------------------------------------------
*
* Function Name  : handle_watchdog_expiry
* Returned Value : none
* Comments       :
*     This function is called when a watchdog has expired.
*END*-----------------------------------------------------------*/

void handle_watchdog_expiry
   (
      void   *td_ptr
   )
{
	/* Restart Tasks to overcome the fault condition*/
//	   _task_restart( MAIN_TASK_ID,NULL,0);
//	   _task_restart( IO_TASK_ID,NULL,0);
}



void sys_health_task(uint32_t param)
{
	   _mqx_uint       result;

	   result = _watchdog_create_component(BSP_SYSTIMER_INTERRUPT_VECTOR,handle_watchdog_expiry);
	   if (result != MQX_OK) {
	      printf("\nError creating watchdog component.");
	   }

    while(1)
    {

        /* Starting the watchdog */
        result = _watchdog_start(WATCHDOG_EXPIRE_MS);
        /* Implement mechanism to check other task state with Watchdog Timer period */
        /* MQX Events can be used to communicate between tasks get the state information */

        _time_delay(1000);
        GPIO_DRV_TogglePinOutput(kGpioLED3);
        _time_delay(1000);
        _watchdog_stop();

    }
}
