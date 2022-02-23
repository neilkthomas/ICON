/*
 * app_thread.c
 *
 *     Created on: Feb 5, 2022
 *     Author: Neil
 */

#include "json.h"
#include "stdlib.h"
#include "icon_mqx_os.h"
#include "json_manager.h"
#include "icon_ds.h"

extern struct netif fsl_netif0;
extern struct ICON_DS         main_task_ds;
extern struct ICON_GENERAL main_task_gs;

u16_t req_id;
extern rtc_datetime_t date;

ip_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;

char ip_address[15];
char net_mask[15];
char gate_way[15];

void get_req_id(char* data)
{

    char* token=NULL;

    token  =strtok((char*)data,":");
    token  =strtok(NULL,",");
    req_id =strtol(token,NULL,0);

    printf("Req-ID:%d\n",req_id);

}

/* Save clock settings */
bool save_clock_value(json_value *pstPointsList)
{
    uint16_t check=0;
    bool nRetVal = false;

        check = sscanf(pstPointsList->u.string.ptr,TIME_FORMAT,&date.year,
                                                   &date.month,&date.day,
                                                   &date.hour,&date.minute,
                                                   &date.second);

    if(check == 6)
    {
        nRetVal = RTC_DRV_SetDatetime(0, &date);
    }

    return nRetVal;
}

/* Save general settings */
bool save_general_setting(json_value *pstPointsList)
{
    bool nRetVal = false;
    json_value *pstValueNode    = NULL;

    pstValueNode = get_value_node("Device Name" , pstPointsList);
    if(NULL != pstValueNode && NULL != main_task_gs.name)
    {
        strcpy(main_task_gs.name , pstValueNode->u.string.ptr);
        nRetVal = true;
    }

    pstValueNode = get_value_node( "Date & Time" , pstPointsList);
     if(NULL != pstValueNode)
     {
         nRetVal = save_clock_value(pstValueNode);
     }


    return nRetVal;
}

/* Load input settings */
void load_input_array(struct netconn *newconn , err_t err , int caseIndex, char *pstStr, char *temp)
{
    u8_t mIndex;
    u16_t len;

    sprintf(pstStr,"{\"Index\":%d,\"InputList\":[",caseIndex);
    for(mIndex = 0 ; mIndex < LOOP_NO ; mIndex++)
    {
        sprintf(temp , "{\"ChannelNo\":\"%d\"", mIndex+1);
        strcat(pstStr,temp);
        strcat(pstStr,",");

        sprintf(temp,"\"Name\":\"%s\"",main_task_ds.inputCard[mIndex].ipchannelname);
        strcat(pstStr,temp);
        strcat(pstStr,",");

        if(main_task_ds.inputCard[mIndex].iptype == T2)
        {
            sprintf(temp,"\"Type\":\"%s\"","Thermistor 2");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }
        else if(V10 == main_task_ds.inputCard[mIndex].iptype)
        {
            sprintf(temp,"\"Type\":\"%s\"","10V-Analog");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }
        else if(DIGITAL == main_task_ds.inputCard[mIndex].iptype)
        {
            sprintf(temp,"\"Type\":\"%s\"","Digital");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }
        else if(ANALOGFIXED == main_task_ds.inputCard[mIndex].iptype)
        {
            sprintf(temp,"\"Type\":\"%s\"","Analog_Fixed");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }
        else
        {
            sprintf(temp,"\"Type\":\"%s\"","NA");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }

        sprintf(temp,"\"Min\":\"%0.2f\"",main_task_ds.inputCard[mIndex].minval);
        strcat(pstStr,temp);
        strcat(pstStr,",");

        sprintf(temp,"\"Max\":\"%0.2f\"",main_task_ds.inputCard[mIndex].maxval);
        strcat(pstStr,temp);
        strcat(pstStr,",");

        sprintf(temp,"\"Offset\":\"%0.2f\"",main_task_ds.inputCard[mIndex].pvoffset);
        strcat(pstStr,temp);
        strcat(pstStr,",");

        sprintf(temp,"\"Unit\":\"%s\"",main_task_ds.inputCard[mIndex].ipunit);
        strcat(pstStr,temp);
        strcat(pstStr,"}");

        if( mIndex < 2 )
        {
            strcat(pstStr,",");
        }
    }
    sprintf(temp,"]}\n");
    strcat(pstStr,temp);
    len = strlen(pstStr);
    printf("******%d-Reply********\n%s\n\n",caseIndex,pstStr);
    err = netconn_write(newconn, pstStr, len, NETCONN_COPY);
}

/* Load output settings */
void load_output_array(struct netconn *newconn , err_t err , int caseIndex, char *pstStr, char *temp)
{
    u8_t mIndex;
    u16_t len;

    sprintf(temp," {\"Index\":%d,\"OutputList\":[",caseIndex);
    strcpy(pstStr,temp);
    for(mIndex = 0 ; mIndex < LOOP_NO_OP;mIndex++)
    {
        sprintf(temp,"{\"ChannelNo\":\"%d\"",mIndex+1);
        strcat(pstStr,temp);
        strcat(pstStr,",");

        sprintf(temp,"\"Name\":\"%s\"",main_task_ds.outputCard[mIndex].opchannelname);
        strcat(pstStr,temp);
        strcat(pstStr,",");

        if(main_task_ds.outputCard[mIndex].optype == ANALOG)
        {
            sprintf(temp,"\"Type\":\"%s\"","Analog");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }
        else if(main_task_ds.outputCard[mIndex].optype == ANALOGFIXED)
        {
            sprintf(temp,"\"Type\":\"%s\"","Analog-Fixed");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }
        else if(main_task_ds.outputCard[mIndex].optype == DIGITAL)
        {
            sprintf(temp,"\"Type\":\"%s\"","Digital");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }
        else
        {
            sprintf(temp,"\"Type\":\"%s\"","NA");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }

        if(main_task_ds.outputCard[mIndex].logic == REVERSE)
        {
            sprintf(temp,"\"Logic\":\"%s\"","Reverse");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }
        else if(main_task_ds.outputCard[mIndex].logic == DIRECT)
        {
            sprintf(temp,"\"Logic\":\"%s\"","Direct");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }
        else
        {
            sprintf(temp,"\"Logic\":\"%s\"","NA");
            strcat(pstStr,temp);
            strcat(pstStr,",");
        }

        sprintf(temp,"\"Unit\":\"%s\"",main_task_ds.outputCard[mIndex].opunit);
        strcat(pstStr,temp);
        strcat(pstStr,",");

        sprintf(temp,"\"Setpoint\":\"%0.2f\"",main_task_ds.outputCard[mIndex].setpoint);
        strcat(pstStr,temp);
        strcat(pstStr,"}");

        if(mIndex<3)
        {
            strcat(pstStr,",");
        }
    }
    sprintf(temp,"]}\n");
    strcat(pstStr,temp);
    len = strlen(pstStr);
    printf("******%d-Reply********\n%s\n\n",caseIndex,pstStr);
    err = netconn_write(newconn, pstStr, len, NETCONN_COPY);
}

/* Save input settings */
void process_input_array(json_value *pstPointsList)
{
    int listLength;
    int nIndex;
    int nRetval = 1;

    nRetval = pstPointsList != NULL && pstPointsList->type == json_array;

    if(nRetval)
    {
        listLength  = pstPointsList->u.array.length;
        nRetval = 0 < listLength;
    }

    if(nRetval)
    {
        for (nIndex = 0; nIndex < listLength ; nIndex++)
        {
            if(NULL != pstPointsList->u.array.values[nIndex]->u.object.values[1].value->u.string.ptr)
            {
                strcpy(main_task_ds.inputCard[nIndex].ipchannelname , pstPointsList->u.array.values[nIndex]->u.object.values[1].value->u.string.ptr);
            }

            if(strcmp("Thermistor 2",pstPointsList->u.array.values[nIndex]->u.object.values[2].value->u.string.ptr) == 0)
            {
                main_task_ds.inputCard[nIndex].iptype = T2;
            }
            else if(strcmp("Digital",pstPointsList->u.array.values[nIndex]->u.object.values[2].value->u.string.ptr) == 0)
            {
                main_task_ds.inputCard[nIndex].iptype = DIGITAL;
            }
            else if(strcmp("Analog",pstPointsList->u.array.values[nIndex]->u.object.values[2].value->u.string.ptr) == 0)
            {
                main_task_ds.inputCard[nIndex].iptype = ANALOG;
            }
            else if(strcmp("5V-Analog",pstPointsList->u.array.values[nIndex]->u.object.values[2].value->u.string.ptr) == 0)
            {
                main_task_ds.inputCard[nIndex].iptype = V5;
            }
            else if(strcmp("10V-Analog",pstPointsList->u.array.values[nIndex]->u.object.values[2].value->u.string.ptr) == 0)
            {
                main_task_ds.inputCard[nIndex].iptype= V10;
            }
            else if(strcmp("Analog_Fixed",pstPointsList->u.array.values[nIndex]->u.object.values[2].value->u.string.ptr) == 0)
            {
                main_task_ds.inputCard[nIndex].iptype= ANALOGFIXED;
            }
            else
            {
                main_task_ds.inputCard[nIndex].iptype = NA;
            }

            main_task_ds.inputCard[nIndex].minval = get_float_val(pstPointsList->u.array.values[nIndex]->u.object.values[3].value);

            main_task_ds.inputCard[nIndex].maxval = get_float_val(pstPointsList->u.array.values[nIndex]->u.object.values[4].value);

            main_task_ds.inputCard[nIndex].pvoffset= get_float_val(pstPointsList->u.array.values[nIndex]->u.object.values[5].value);

            strcpy(main_task_ds.inputCard[nIndex].ipunit , pstPointsList->u.array.values[nIndex]->u.object.values[6].value->u.string.ptr);

        }

    }
}

/* Save output settings */
void process_output_array(json_value *pstPointsList)
{
    int listLength;
    int nIndex;
    int nRetval = 1;

    nRetval = pstPointsList != NULL && pstPointsList->type == json_array;

    if(nRetval)
    {
        listLength  = pstPointsList->u.array.length;
        nRetval = 0 < listLength;
    }

    if(nRetval)
    {
        for (nIndex = 0; nIndex < listLength; nIndex++)
        {
            if(NULL != pstPointsList->u.array.values[nIndex]->u.object.values[1].value->u.string.ptr)
            {
                strcpy(main_task_ds.outputCard[nIndex].opchannelname , pstPointsList->u.array.values[nIndex]->u.object.values[1].value->u.string.ptr);
            }

            if(strcmp("Analog",pstPointsList->u.array.values[nIndex]->u.object.values[2].value->u.string.ptr) == 0)
            {
                main_task_ds.outputCard[nIndex].optype = ANALOG;
            }
            else if(strcmp("Analog-Fixed",pstPointsList->u.array.values[nIndex]->u.object.values[2].value->u.string.ptr) == 0)
            {
                main_task_ds.outputCard[nIndex].optype = ANALOGFIXED;
            }
            else if(strcmp("Digital",pstPointsList->u.array.values[nIndex]->u.object.values[2].value->u.string.ptr) == 0)
            {
                main_task_ds.outputCard[nIndex].optype = DIGITAL;
            }
            else
            {
                main_task_ds.outputCard[nIndex].optype = NA;
            }

            if(strcmp("Direct",pstPointsList->u.array.values[nIndex]->u.object.values[3].value->u.string.ptr) == 0)
            {
                main_task_ds.outputCard[nIndex].logic = DIRECT;
            }
            else if(strcmp("Reverse",pstPointsList->u.array.values[nIndex]->u.object.values[3].value->u.string.ptr) == 0)
            {
                main_task_ds.outputCard[nIndex].logic = REVERSE;
            }

            strcpy(main_task_ds.outputCard[nIndex].opunit , pstPointsList->u.array.values[nIndex]->u.object.values[4].value->u.string.ptr);

            main_task_ds.outputCard[nIndex].setpoint = get_float_val(pstPointsList->u.array.values[nIndex]->u.object.values[5].value);

        }
    }
}

/* Config App over TCP Thread */
/* Manages JSON GET/POST */
void application_thread(void *arg)
{
    struct netconn *conn;
    struct netconn *newconn;
    err_t err;

    LWIP_UNUSED_ARG(arg);
    netif_set_up(&fsl_netif0);
    /* Create a new connection identifier */
    conn = netconn_new(NETCONN_TCP);

    /* Bind connection to well known port number */
    netconn_bind(conn, NULL, CONFIG_APP_PORT);

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
            u16_t len;
            int integer = 0;

            json_value *pstJsonRootNode = NULL;
            json_value *pstValueNode    = NULL;

            while ((err = netconn_recv(newconn, &buf)) == ERR_OK)
            {
                do {
                	data=NULL;
                    netbuf_data(buf, &data, &len);

                    /*Print Received Config App data */
                    printf("DATA:[%s]\n",(char*) data);
                    pstJsonRootNode = json_parse((char*)data , len);


                    if(NULL != pstJsonRootNode)
                    {
                        pstValueNode = get_value_node("Index", pstJsonRootNode);
                    }

                    if(NULL != pstValueNode)
                    {
                        integer = get_int_val(pstValueNode);
                    }

                   /*Print JSON KEY Code */
                    printf("\nTask Index:%d\n", integer);

                    switch(integer)
                    {
                        char str[1024];
                        char temp[128];

                        /* Load general settings */
                        case LGN:
                        {
                            RTC_DRV_GetDatetime(0, &date);
                            sprintf(str," {\"Index\":11111,\"MAC Address\":\"00:CF:52:35:00:03\",\"Date & Time\":\"%04hd-%02hd-%02hd %02hd:%02hd:%02hd\",\"Firmware Version\":\"Icon.02.22\",\"Device Name\":\"%s\"}\n",date.year,date.month,date.day,date.hour,date.minute,date.second,main_task_gs.name);
                            err = netconn_write(newconn, str, strlen(str), NETCONN_COPY);
                        }
                        break;
                        /* Save general settings */
                        case SGN:
                        {
                            bool nRetVal;
                            nRetVal = save_general_setting(pstJsonRootNode);
                            if(1==nRetVal){
                            sprintf(str," {\"Index\":11112,\"MAC Address\":\"00:CF:52:35:00:03\",\"Date & Time\":\"%04hd-%02hd-%02hd %02hd:%02hd:%02hd\",\"Firmware Version\":\"Icon.02.22\",\"Device Name\":\"%s\"}\n",date.year,date.month,date.day,date.hour,date.minute,date.second,main_task_gs.name);
                            err = netconn_write(newconn, str, strlen(str), NETCONN_COPY);
                            }else{
                            sprintf(str,"{}");
                            err = netconn_write(newconn, str, strlen(str), NETCONN_COPY);
                            }
                        }
                        break;
                        /* Load input settings */
                        case LINP:
                        {
                            load_input_array(newconn,err,11930, str, temp);
                        }
                        break;
                        /* Save input settings */
                        case SINP:
                        {
                            pstValueNode = get_value_node("InputList" , pstJsonRootNode);
                            process_input_array(pstValueNode);
                            load_input_array(newconn,err,11931, str, temp);
                        }
                        break;
                        /* Load output settings */
                        case LOUT:
                        {
                            load_output_array(newconn,err,11940, str, temp);
                        }
                        break;
                        /* Save output settings */
                        case SOUT:
                        {
                            pstValueNode = get_value_node("OutputList" , pstJsonRootNode);
                            process_output_array(pstValueNode);
                            load_output_array(newconn,err,11941, str, temp);
                        }
                        break;

                        default:
                        {
                            err = netconn_write(newconn, data, len, NETCONN_COPY);
                        }
                        break;
                    }

                } while (netbuf_next(buf) >= 0);

                json_value_free(pstJsonRootNode);
                netbuf_delete(buf);
            }
            /* Close connection and discard connection identifier. */
            netconn_close(newconn);
            netconn_delete(newconn);
        }

    }
}
