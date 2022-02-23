/*
 * modbus_thread.c
 *
 *     Created on: Feb 5, 2022
 *     Author: Neil
 */

#include "icon_mqx_os.h"
#include "icon_ds.h"

extern struct netif fsl_netif0;
extern struct ICON_DS         main_task_ds;
ip_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;

/* Read Analog values (Function code 1)*/
void ang_out(void *dat)
{
    u8_t d_out;

    d_out=*((u8_t*)(dat+9));

    *((u8_t *)(dat+5)) = ACTUAL_PACKET_SIZE;  /* altering size of packet */
    *((u8_t *)(dat+8)) = ACTUAL_DATA_SIZE;  /* altering size of data */

    switch(d_out)
    {
    case ANALOG_CHN_1:
       /* If Input Channel-1 is configured as Analog/ Read Coil */
       /* *((u8_t *)(dat+9)) = main_task_ds.outputCard[0].setpoint;  */
        *((u8_t *)(dat+9)) = MODBUS_TEST_DAT_1;
        break;
    case ANALOG_CHN_2:
        /* If Input Channel-2 is configured as Analog/ Read Coil */
        /* *((u8_t *)(dat+9)) = main_task_ds.outputCard[1].setpoint;  */
        *((u8_t *)(dat+9)) = MODBUS_TEST_DAT_2;
        break;
    }
}


/* Read Discrete values (Function code 2) */
void dig_in(void *dat)
{
    u8_t d_in;

    d_in=*((u8_t*)(dat+9));

    *((u8_t *)(dat+5)) = ACTUAL_PACKET_SIZE;  /* altering size of packet */
    *((u8_t *)(dat+8)) = ACTUAL_DATA_SIZE;  /* altering size of data */


    switch(d_in)
    {
    /* If Input Channel-1 is configured as Digital/ Read Discrete Input */
    case DIGITAL_CHN_1:
        *((u8_t *)(dat+9)) =  main_task_ds.inputCard[1].pvvalue;
        break;
    /* If Input Channel-2 is configured as Digital/ Read Discrete Input */
    case DIGITAL_CHN_2:
        *((u8_t *)(dat+9)) =  main_task_ds.inputCard[2].pvvalue;
        break;
    }
}

/* TCP Modbus Thread */
/* Read and write implementation of Modbus Function code*/
void modbus_thread(void *arg)
{
	struct netconn *conn, *newconn;
	err_t err;
	LWIP_UNUSED_ARG(arg);
	netif_set_up(&fsl_netif0);

	/* Create a new connection identifier */
	conn = netconn_new(NETCONN_TCP);

	/* Bind connection to well known port number */
	netconn_bind(conn, NULL, MODBUS_PORT);

	/* Tell connection to go into listening mode */
	netconn_listen(conn);

	while (1)
	{
		/* Grab new connection. */
		err = netconn_accept(conn, &newconn);
		/* Process the new connection. */
		if (err == ERR_OK)
		{

			struct netbuf *buf;
			void *data;
			unsigned int i;
			u16_t len;

			while ((err = netconn_recv(newconn, &buf)) == ERR_OK)
			{
				do {
					netbuf_data(buf, &data, &len);

					for(i=0; i<len; ++i)

						if( MODBUS_FUNCTION_CODE_1 == (*(u8_t*)(data+7))  )
						{
							ang_out((u8_t*)data);
						}
						else if( MODBUS_FUNCTION_CODE_2 == (*(u8_t*)(data+7)) )
						{
							dig_in((u8_t*)data);
						}
						else
						{
							printf("MODBUS Function code not supported \n");

						}

					err = netconn_write(newconn, data, len, NETCONN_COPY);

					/* Let the other task run */
					_time_delay(1);

				} while (netbuf_next(buf) >= 0);
				netbuf_delete(buf);
			}
			/* Close connection and discard connection identifier. */
			netconn_close(newconn);
			netconn_delete(newconn);
		}
	}
}
