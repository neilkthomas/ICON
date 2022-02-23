/*
 * icon_common.h
 *     Created on: Feb 5, 2022
 *     Author: Neil
 */

#ifndef _ICON_COMMON_H_INCLUDED_
#define _ICON_COMMON_H_INCLUDED_


#define POINT_NAME_BUFF_SIZE (32)
#define POINT_UNIT_BUFF_SIZE (25)
#define POINT_INFO_COMMENT_BUFF_SIZE (25)


#define DIGITAL	    ( 0 )
#define ANALOG      ( 1 )
#define ANALOGFIXED ( 2 )

#define ANALOG_CHN_1 ( 1 )
#define ANALOG_CHN_2 ( 2 )
#define DIGITAL_CHN_1 ( 1 )
#define DIGITAL_CHN_2 ( 2 )

#define PWM1_DEFAULT_DUTY 50
#define PWM2_DEFAULT_DUTY 80

#define DISABLE ( 0 )
#define ENABLE  ( 1 )
#define ON      ( 1 )
#define OFF     ( 0 )
#define AUTO    ( 1 )
#define MANUAL  ( 0 )

#define RELAY_ON (1.0)
#define LED_ON  (0)
#define LED_OFF (1)

static double const digitalThreshold = 0.8;
static double const analog_5v_mul_factor = 2.837114;
static double const analog_10v_mul_factor = 1.418557;
static double const internel_thermistor_offset = 4.0;

// The default network interface
#define NETWORK_INTERFACE "eth0"

// The section titles in drop down list
#define INPUT_SEC_TITLE_JSON  "INPUT"
#define OUTPUT_SEC_TITLE_JSON "OUTPUT"
#define CT2_SEC_TITLE_JSON    "CT2"
#define TMR_SEC_TITLE_JSON    "TMR"
#define EVT_SEC_TITLE_JSON    "EVT"
#define GV_SEC_TITLE_JSON     "GV"
#define SP_SEC_TITLE_JSON     "SP"
#define SCH_SEC_TITLE_JSON    "SCH"
#define ALM_SEC_TITLE_JSON    "ALM"
#define SWT_SEC_TITLE_JSON 	  "SWT"
#define PGM_SEC_TITLE_JSON    "PGM"

#define DIGITAL_IN_THRESHOLD                  (2.5)


#define LGN 11111
#define SGN 11112
#define LINP 11930
#define SINP 11931
#define LOUT 11940
#define SOUT 11941

#define CONFIG_APP_PORT 9000U
#define MODBUS_PORT 7U
#define WEB_PORT 80U

/* Default IP Address*/
#define IP_ADS_3 192
#define IP_ADS_2 168
#define IP_ADS_1 18
#define IP_ADS_0 102

#define GW_IP_ADS_3 192
#define GW_IP_ADS_2 168
#define GW_IP_ADS_1 18
#define GW_IP_ADS_0 1

#define NM_3 255
#define NM_2 255
#define NM_1 255
#define NM_0 0

#define TIME_FORMAT             "%04d-%02d-%02d %02d:%02d:%02d"

#define YEAR 2022U
#define MONTH 02U
#define DAY 16U
#define HOUR 19U
#define MINUTE 50U
#define SECOND 0U

#define ACTUAL_PACKET_SIZE 04
#define ACTUAL_DATA_SIZE 01
#define MODBUS_TEST_DAT_1 22
#define MODBUS_TEST_DAT_2 33

#define MODBUS_FUNCTION_CODE_1 1
#define MODBUS_FUNCTION_CODE_2 2

#define INP_CHN_1 "INP_CHN_1"
#define INP_CHN_2 "INP_CHN_2"
#define INP_CHN_3 "INP_CHN_3"

#define OUT_CHN_1 "OUT_CHN_1"
#define OUT_CHN_2 "OUT_CHN_2"
#define RLY_CHN_1 "RLY_CHN_1"
#define RLY_CHN_2 "RLY_CHN_2"



#define VUNIT "V"
#define MINVAL (0)
#define MAX3V3 (3.3)
#define OFFSETZERO (0)

#endif // _ICON_COMMON_H_INCLUDED_
