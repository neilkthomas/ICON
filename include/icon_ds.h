/*
 * icon_ds.h
 *     Created on: Feb 5, 2022
 *     Author: Neil
 */

#ifndef INCLUDE_ICON_DS_H_
#define INCLUDE_ICON_DS_H_

#include "icon_common.h"
#include <message.h>

#define ADCRANGE              3.1
#define analog_10v_mul_factor 5
#define analog_5v_mul_factor  5

#define NET         (4)
#define LOOP_NO     (3)
#define LOOP_NO_EVT (4)
#define LOOP_NO_OP  (4)
#define MAX_AOUT    (2)
#define MAX_DOUT    (2)

#define MAX_PROGRAM_COUNT (1)
#define MAX_COUNTER1_NUMBER (2)
#define MAX_TIMER_NUMBER (2)
#define MAX_SETPOINT_NUMBER   (2)
#define MAX_SCHEDULE_NUMBER (4)
#define MAX_ALARM_NUMBER      (4)

#define IP_UNIT_SZ    POINT_UNIT_BUFF_SIZE
#define OP_UNIT_SZ    POINT_UNIT_BUFF_SIZE
#define CH_NAME_SZ    POINT_NAME_BUFF_SIZE
#define P_NAME_SZ     POINT_NAME_BUFF_SIZE
#define EV_NAME_SZ    POINT_NAME_BUFF_SIZE
#define EV_COMMENT_SZ POINT_INFO_COMMENT_BUFF_SIZE

#define IP_NOT_CONFIGURED (100)

// Program logic
#define P   (1)
#define PI  (2)
#define PID (3)

// Output logic
#define DIRECT  (0)
#define REVERSE (1)

// Input point type
#define V5  (1)
#define V10 (2)
#define T2  (3)
#define NA  (6)

#define NAME POINT_NAME_BUFF_SIZE

struct ICON_GENERAL
{
    char          name[NAME];
    unsigned char id;
};


typedef enum
{
	PROGRAM_NORMAL_STATE,
	PROGRAM_RUNNING_STATE,
	PROGRAM_PASSIVE_STATE,
	PROGRAM_PASSIVE_FINISHED_STATE
}PROGRAM_STATE_e;

typedef enum
{
	PROGRAM_LOGIC_DIRECT,
	PROGRAM_LOGIC_REVERSE
}PROGRAM_LOGIC_e;

struct ICON_DS
{
    struct _ICON_INPUT_t
	{
		char   ipchannelname[CH_NAME_SZ];
		char   ipunit[OP_UNIT_SZ];
		float  tolerance;
		float  maxval;
    	float  minval;
		float  pvoffset;
		float  pvvalue;
 		int8_t iptype;   // Analog or Digital
    	int8_t dgipstat; // ON/OFF
    	int8_t iprange;  // 5V, 10V or T2

	}inputCard[LOOP_NO];

	struct _ICON_OUTPUT_t
	{
		char    opchannelname[CH_NAME_SZ];
		char    opunit[OP_UNIT_SZ];
		float   setpoint;
		float   previous_setpoint;
		float   outputVoltage;
		uint8_t opchannel;
		int8_t  optype;
    	int8_t  cntalgo;
		int8_t  dgopstat;
		int8_t  logic;
		int8_t  cnttype; // Manual or Auto

	}outputCard[LOOP_NO_OP];

};

struct ICON_NETWORK
{
    char ip[NET];
    char netmask[NET];
    char gateway[NET];
 }network;




 /* This structure contains a data field and a message header structure */
 typedef struct server_message
 {
    MESSAGE_HEADER_STRUCT   HEADER;
    //unsigned char                   DATA[5];
    struct ICON_DS         DS;
 } SERVER_MESSAGE, * SERVER_MESSAGE_PTR;


void main_task(uint32_t param);
void sys_health_task(uint32_t param);
void io_manage_task(uint32_t param);
void application_thread(void *arg);
void modbus_thread(void *arg);
void http_server_netconn_thread(void *arg);

void init_ds_structure(struct ICON_DS *main_task_ds);
void main_task_msg_ds_update(struct ICON_DS *msg_ds,struct ICON_DS src_ds);
void io_task_msg_ds_update(struct ICON_DS *msg_ds,struct ICON_DS src_ds);

#endif /* INCLUDE_ICON_DS_H_ */
