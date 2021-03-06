# ICON Controller

Low cost industrial controller with multiple programable input, output channels.


# Features:-
#############################################################
* 3 Input Channels(Analog/Digital)
* 2 Output Channels(Analog)
* 2 Output Channels(Digital)
* Modbus over TCP/IP
* Controller Programming software over TCP/IP
* Status Monitoring Web page


# Implementation:-
#############################################################
* ICON Firmware implemented using Freescale MQX RTOS and TCP/IP LWIP Stack.
* Freescale Kinetis Design Studio (KDS IDE) used for the development.
* SDK - KSDK_1.2.0, MQX RTOS 4.x
* Example tcpecho_rtos ( C:\Freescale\KSDK_1.2.0\examples\frdmk64f\demo_apps\lwip\lwip_tcpecho_demo\tcpecho_rtos )  used as reference.
-------------------------------------------------------------- 
        Source code organisation given below
	|
	|-source              // Contains ICON specific source code implementation. Which manages tasks,device driver configuration, IO read &write 
	|-include             // Headers files of ICON source files 
	|-web                 // Status web page 
	|-json-parser         // 3rd Party Json library 
	|-board               // Board specific hardware/peripherals configurations 
	|-config_cli_app_test // ICON COnfig CLI App 
--------------------------------------------------------------


# Peripherals/Modules:-
#############################################################

List of peripherals
* GPIO'S
* ADC (16 BIT) 
* FTM  (PWM)
* ETHERNET
* RTC
* DEBUG UART
* WATCHDOG
* FPU (Floating point unit)


# Software Architecture & Communication
#############################################################

    |=====================================|         |============================|    |=====================================|
    |              MAIN TASK              |         |     SYSTEM HEALTH TASK     |    |            IO MANAGE TASK           |
    |=====================================|	        |============================|    |=====================================|
    | |=========| |=========| |=========| | 1 EVENT | |=========|  |=========|   |    | |=========| |=========| |=========| |
    | | CONFIG  | | MODBUS  | |   WEB   |-|---------|-| WATCHDOG|  |  HEART  |   |    | |   GPIO  | |   ADC   | |  PWM    | |
    | |   APP   | |   APP   | | SERVER  | |         | | CHECKER |  |   BEAT  |   |    | | CONTROL | |  READ   | |  DUTY   | |
    | | THREAD  | | THREAD  | | THREAD  | | 2 EVENT | |    &    |  |   LED   |   |    | |         | |         | | CONTROL | |
    | |         | |         |-|---------|-|---------|-| THRIGGER|  |=========|   |    | |         | |         | |         | |
    | |         |-|---------|-|---------|-|---------|-|         |----------------|----| |         | |         | |         | |
    | |=========| |=========| |=========| | 3 EVENT | |=========|  4 EVENT (IO)  |    | |=========| |=========| |=========| |
    |         ^        ^          ^       |         |         ^                  |    |         ^        ^          ^       |
    |         |        |          |       |         |         |                  |    |         |        |          |       |
    |         |        |          |       |         |         |                  |    |         |        |          |       |
    |      ++++++++++++++++++++++++++     |         | ++++++++++++++++++++++++++ |    |      ++++++++++++++++++++++++++     |
    |      |  ICON DATA STRCUCTURE  |     |         | |  TASK STATE STRCUCTURE | |    |      |  IO DATA STRCUCTURE    |     |
    |      |++++++++++++++++++++++++|     |         | |++++++++++++++++++++++++| |    |      |++++++++++++++++++++++++|     |
    |      | ipchannelname          |     |         | | task_name              | |    |      | ipchannelname          |     |
    |      | ipunit                 |     |         | | task_id                | |    |      | ipunit                 |     |
    |      | pvvalue                |     |         | | tast_state             | |    |      | pvvalue                |     |
    |      | dgipstat               |     |         | | is_watchdog_triggered  | |    |      | dgipstat               |     |
    |      | opchannelname          |     |         | | time                   | |    |      | opchannelname          |     |
    |      | opunit                 |     |         | |                        | |    |      | opunit                 |     |
    |      | setpoint               |     |         | |                        | |    |      | setpoint               |     |
    |      | dgopstat               |     |         | |                        | |    |      | dgopstat               |     |
    |      |++++++++++++++++++++++++|     |         | |++++++++++++++++++++++++| |    |      |++++++++++++++++++++++++|     |
    |=====================================|         |============================|    |=====================================|
                              ||      ||                                                ||   ||
                              ||      || MESSAGE QUEUE - IO CONTROL DATA TO IO TASK     ||   ||
                              ||       ==================================================    ||
                              ||                                                             ||
                              ||    MESSAGE QUEUE - REDA IO DATA FROM IO TASK                ||
                               ===============================================================



# Input and output pins used in Freedom K64 Board for ICON
###########################################################

Module  & PINS
* CHN1      ADC1_SE18
* CHN2      PTB10/ADC1_SE14
* CHN3      PTB 11/ADC1_SE15
* PWM1      PTC4
* PWM2      PTC2
* On board tricolor LED's ( Red & Green Led Pins - Relay outs, Blue Led as Heart beat led(alive indication))
* THM1      ADC0_SE12 // To be done
* THM2      ADC0_SE13 // To be done


# Testing:-
#############################################################
1) Configure static IP in icon_common.h (IP_ADS_x)
2) Open web browser and enter IP address of ICON. Refresh the web page to see the updates.
2) Install MODBUS POLL software. Configure ICON IP address and ICON default MODBUS port (7).
   MODBUS Function code 1,2 only implemented.
3) Install python 2.7 to run ICON CLI Config App.
   Ex:-  

-------------------------------------
ICon - Command Line Configuration App
=====================================
		KeyCode - Option
		LGN  - Load General Settings
		SGN  - Save General Settings
		LINP - Load Input Settings
		SINP - Save Input Settings
		LOUT - Load Output Settings
		SOUT - Save Output Settings

-------------------------------------
	 
	 $ python cli.py LGN  
LGN
Load:     {"Index":11111}
 {"Index":11111,"MAC Address":"00:CF:52:35:00:03","Date & Time":"2022-02-16 19:50:34","Firmware Version":"Icon.02.22","Device Name":""}

connection close 1
					  
	$ python cli.py SGN
SGN
Save:     {"Index": 11112,"MAC Address": "00:CF:52:35:00:03","Date & Time": "2022-02-06 17:16:00","Firmware Version": "Icon.02.22","Device Name": "Icon"}

{"Index":11112,"MAC Address":"00:CF:52:35:00:03","Date & Time":"2022-02-06 17:16:00","Firmware Version":"Icon.02.22","Device Name":"Icon"}

	$ python cli.py SINP
SINP
Save:     {"Index":11931,"InputList":[{"ChannelNo":"1","Name":"5V CHN1","Type":"5V-Analog","Min":"0.00","Max":"5.00","Offset":"0.00","Unit":"V"},{"ChannelNo":"2","Name":"5v CHN2","Type":"5V-Analog","Min":"0.00","Max":"5.00","Offset":"0.00","Unit":"V"},{"ChannelNo":"3","Name":"5V CHN3","Type":"5V-Analog","Min":"0.00","Max":"5.00","Offset":"0.00","Unit":"V"}]}

{"Index":11931,"InputList":[{"ChannelNo":"1","Name":"5V CHN1","Type":"NA","Min":"0.00","Max":"5.00","Offset":"0.00","Unit":"V"},{"ChannelNo":"2","Name":"5v CHN2","Type":"NA","Min":"0.00","Max":"5.00","Offset":"0.00","Unit":"V"},{"ChannelNo":"3","Name":"5V CHN3","Type":"NA","Min":"0.00","Max":"5.00","Offset":"0.00","Unit":"V"}]}

	$ python cli.py SOUT
SOUT
Save:     {"Index":11941,"OutputList":[{"ChannelNo":"1","Name":"5V CHN1","Type":"5V-Analog","Logic":"Direct","Unit":"V","Setpoint":"20.00"},{"ChannelNo":"2","Name":"5V CHN2","Type":"5V-Analog","Logic":"Direct","Unit":"V","Setpoint":"80.00"},{"ChannelNo":"3","Name":"Green Relay","Type":"Digital","Logic":"Direct","Unit":"V","Setpoint":"1.00"},{"ChannelNo":"4","Name":"Red Relay","Type":"Digital","Logic":"Direct","Unit":"V","Setpoint":"0.00"}]}

 {"Index":11941,"OutputList":[{"ChannelNo":"1","Name":"5V CHN1","Type":"NA","Logic":"Direct","Unit":"V","Setpoint":"20.00"},{"ChannelNo":"2","Name":"5V CHN2","Type":"NA","Logic":"Direct","Unit":"V","Setpoint":"80.00"},{"ChannelNo":"3","Name":"Green Relay","Type":"Digital","Logic":"Direct","Unit":"V","Setpoint":"1.00"},{"ChannelNo":"4","Name":"Red Relay","Type":"Digital","Logic":"Direct","Unit":"V","Setpoint":"0.00"}]}
		  
