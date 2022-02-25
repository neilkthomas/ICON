/*
 * main.c
 *
 *     Created on: Feb 5, 2022
 *     Author: Neil
 */


#include "icon_mqx_os.h"
#include "icon_ds.h"
#include <mqx.h>
#include <message.h>
#include <mutex.h>

struct ICON_DS         main_task_ds;
struct ICON_GENERAL main_task_gs;

struct netif fsl_netif0;
rtc_datetime_t date;
extern _pool_id message_pool;
MUTEX_STRUCT   main_ds_mutex;
extern MUTEX_STRUCT   io_ds_mutex;

#if defined(FSL_RTOS_MQX)
TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
	{ MAIN_TASK_ID, main_task,  MQX_MAIN_TASK_STACKSIZE, MQX_MAIN_TASK_PRIORITY, "Main", MQX_AUTO_START_TASK|MQX_FLOATING_POINT_TASK},
	{ IO_TASK_ID, io_manage_task, IO_MANAGE_TASK_STACKSIZE, IO_MANAGE_TASK_PRIORITY , "IO_MANAGE", MQX_AUTO_START_TASK|MQX_FLOATING_POINT_TASK},
	{ SYS_TASK_ID, sys_health_task,SYS_HEALTH_TASK_STACKSIZE, SYS_HEALTH_TASK_PRIORITY, "SYS_HLTH", MQX_AUTO_START_TASK},
	{ 0 }
};
#endif

/*TCP Threads create (HTTP Server, Modbus, Config App)*/
void tcpthread_init(void)
{
    sys_thread_new("modbus_thread", modbus_thread, NULL, TCPMODBUS_STACKSIZE, TCPMODBUS_PRIORITY);
    sys_thread_new("application_thread", application_thread, NULL, TCPAPP_STACKSIZE, TCPAPP_PRIORITY);
    sys_thread_new("http_server_netconn", http_server_netconn_thread, NULL, HTTPSRVR_STACKSIZE, HTTPSRVR_PRIORITY);

    OSA_Start();
}

/* Init function for peripherals/modules  initialization*/
static void app_low_level_init(void)
{

    /* Init hardware */
    hardware_init();

    CLOCK_SYS_SetRtcOutSrc(kClockRtcoutSrc1Hz);

    /* Open ENET clock gate */
    CLOCK_SYS_EnableEnetClock(CLOCK_INSTANCE);
    /* Select PTP timer outclk */
    CLOCK_SYS_SetEnetTimeStampSrc(CLOCK_INSTANCE, kClockTimeSrcOsc0erClk);


    PRINTF("RTC example: set up time to wake up an alarm\r\n");

    /* Init RTC */
    RTC_DRV_Init(RTC_INSTANCE);
    /* Set a start date time and start RT. */
    date.year   = YEAR;
    date.month  = MONTH;
    date.day    = DAY;
    date.hour   = HOUR;
    date.minute = MINUTE;
    date.second = SECOND;

    /* Set RTC time to default */
    RTC_DRV_SetDatetime(RTC_INSTANCE, &date);

    /* Disable the mpu */
    MPU_BWR_CESR_VLD(MPU, 0);
}




/* Main task function which create TCP threads.
 * Does main activities of ICON controller
 * Manages ICON Data Structures */
#if defined(FSL_RTOS_MQX)
void main_task(uint32_t param)
#else
int main(void)
#endif
{
    ip_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;

    /* Message Pool Creation for data exchange between IO Manage task and Main Task thread*/

    SERVER_MESSAGE_PTR msg_ptr;
    _queue_id          client_qid;
    bool            result;

    client_qid  = _msgq_open((_queue_number)(CLIENT_QUEUE_BASE), 0);

    if (client_qid == 0) {
       printf("\nCould not open a client message queue\n");
    }

    /*********************** End message queue creation **********************************/

    MUTEX_ATTR_STRUCT mutexattr;

    /* Initialize mutex attributes */
    if (_mutatr_init(&mutexattr) != MQX_OK) {
       printf("Initialize mutex attributes failed.\n");
    }

    /* Initialize the main ds mutex */
    if (_mutex_init(&main_ds_mutex, &mutexattr) != MQX_OK) {
       printf("Initialize main DS mutex failed.\n");
    }

    /* Initialize the main ds mutex */
    if (_mutex_init(&io_ds_mutex, &mutexattr) != MQX_OK) {
       printf("Initialize IO DS mutex failed.\n");
    }

    /* Initialize ICON_DS structures with default values*/
    init_ds_structure(&main_task_ds);

    app_low_level_init();
    OSA_Init();

    /* Statically assigning IP Address*/
    network.ip[3]=IP_ADS_3;
    network.ip[2]=IP_ADS_2;
    network.ip[1]=IP_ADS_1;
    network.ip[0]=IP_ADS_0;

    network.gateway[3]=GW_IP_ADS_3;
    network.gateway[2]=GW_IP_ADS_2;
    network.gateway[1]=GW_IP_ADS_1;
    network.gateway[0]=GW_IP_ADS_0;

    network.netmask[3]=NM_3;
    network.netmask[2]=NM_2;
    network.netmask[1]=NM_1;
    network.netmask[0]=NM_0;
    printf("%c.%c.%c.%c\n",network.ip[3],network.ip[2],network.ip[1],network.ip[0]);


    LWIP_DEBUGF(TCPECHO_DBG, ("TCP/IP initializing...\r\n"));
    tcpip_init(NULL,NULL);
    LWIP_DEBUGF(TCPECHO_DBG, ("TCP/IP initializing...\r\n"));

    IP4_ADDR(&fsl_netif0_ipaddr, network.ip[3],network.ip[2],network.ip[1],network.ip[0]);
    IP4_ADDR(&fsl_netif0_netmask, network.netmask[3],network.netmask[2],network.netmask[1],network.netmask[0]);
    IP4_ADDR(&fsl_netif0_gw, network.gateway[3],network.gateway[2],network.gateway[1],network.gateway[0]);
    netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init, tcpip_input);
    netif_set_default(&fsl_netif0);
    tcpthread_init();

    /*allocate a message*/
    msg_ptr = (SERVER_MESSAGE_PTR)_msg_alloc(message_pool);
    if (msg_ptr == NULL) {
       printf("\nCould not allocate a message\n");
    }

    while(1)
    {



        msg_ptr->HEADER.SOURCE_QID = client_qid;
        msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, SERVER_QUEUE);

        msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) + sizeof(struct ICON_DS) + 1;

        /** Copying Main Task DS structure to Message Queue payload **/
        if (_mutex_lock(&main_ds_mutex) != MQX_OK) {
           printf("Main DS Mutex lock failed.\n");
        }
        main_task_msg_ds_update(&msg_ptr->DS,main_task_ds);
        _mutex_unlock(&main_ds_mutex);
        /**********************************************************/

        /** Message queue to send ICON DS output control data to IO Task **/

        result = _msgq_send(msg_ptr);

        if (result != TRUE) {
           printf("\nCould not send a message\n");
        }

        /**End  Message queue to send ICON DS output control data to IO Task **/


        /** Message queue to receive IO DS input data to Main Task **/

        /* wait for a return message */
        msg_ptr = _msgq_receive(client_qid, 0);

        if (msg_ptr == NULL) {
           printf("\nCould not receive a message\n");
        }else
        {
            /** Copying Message Queue payload to  Main Task DS structure **/
            if (_mutex_lock(&io_ds_mutex) != MQX_OK) {
               printf("IO DS Mutex lock failed.\n");
            }
        	io_task_msg_ds_update(&main_task_ds,msg_ptr->DS);
            _mutex_unlock(&io_ds_mutex);
            /**********************************************************/
        }

        /** End Message queue to receive IO DS input data to Main Task **/

        _time_delay(1);
    }
}


