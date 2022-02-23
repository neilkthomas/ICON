/*
 * icon_mqx_os.h
 *
 *     Created on: Feb 5, 2022
 *     Author: Neil
 */

#ifndef INCLUDE_ICON_MQX_OS_H_
#define INCLUDE_ICON_MQX_OS_H_


#include "lwip/opt.h"
#include "stdlib.h"


// Standard C Included Files
#include <stdio.h>
// lwip Included Files
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"
// SDK Included Files
#include "fsl_clock_manager.h"
#include "fsl_uart_driver.h"
#include "fsl_device_registers.h"
#include "fsl_port_hal.h"
#include "fsl_sim_hal.h"
#include "fsl_os_abstraction.h"
#include "ethernetif.h"
#include "board.h"

#include "flexTimer1.h"
#include "fsl_adc16_driver.h"
#include "fsl_ftm_driver.h"
#include "fsl_pmc_hal.h"
#include "fsl_rtc_driver.h"
#include "gpio_pins.h"



#define ADC16_INSTANCE                (0)   // ADC instance
#define ADC16_INSTANCE1               (1)


#define ADC_THM1_PIN_ADC0_SE12        (12)
#define ADC_THM2_PIN_ADC0_SE13        (13)
#define ADC_CHN2_PIN_ADC1_SE14        (14)
#define ADC_CHN3_PIN_ADC1_SE15        (15)
#define ADC_CHN1_PIN_ADC1_SE18        (18)

#define FTM0_CH1_PIN_PTC2             (1U)
#define FTM0_CH3_PIN_PTC4             (3U)

#define UINT16_3V3_CONVERT            (19859.09)
#define ADC_NO_SAMPLES                (20)

#define CLOCK_INSTANCE                (0)   // CLOCK instance
#define RTC_INSTANCE                  (0)   // RTC instance

#define ADC16_CHN_GROUP               (0)   // ADC group configuration selection

#define MAIN_TASK_ID  1
#define IO_TASK_ID    2
#define SYS_TASK_ID   3

#define IO_MANAGE_TASK_PRIORITY       (20)
#define SYS_HEALTH_TASK_PRIORITY      (18)


#define MQX_MAIN_TASK_STACKSIZE      12000
#define IO_MANAGE_TASK_STACKSIZE     1000
#define SYS_HEALTH_TASK_STACKSIZE     500

 /* Queue IDs */
 #define SERVER_QUEUE      8
 #define CLIENT_QUEUE_BASE 9

/* Number of clients */
#define NUM_CLIENTS           (1)

#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG         LWIP_DBG_ON
#endif

#define HTTPSRVR_STACKSIZE        4000
#define HTTPSRVR_PRIORITY          3
#ifndef TCPECHO_DBG
#define TCPECHO_DBG     LWIP_DBG_ON
#endif


#define TCPAPP_STACKSIZE        4000
#define TCPMODBUS_STACKSIZE     4000

#define TCPAPP_PRIORITY          3
#define TCPMODBUS_PRIORITY       3

#define WATCHDOG_EXPIRE_MS      4000


#endif /* INCLUDE_ICON_MQX_OS_H_ */
