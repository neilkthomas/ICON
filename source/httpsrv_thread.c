/*
 * httpsrv_thread.c
 *
 *     Created on: Feb 5, 2022
 *     Author: Neil
 */


#include "icon_mqx_os.h"
#include "icon_ds.h"


#include "web_pages.h"

extern struct netif fsl_netif0;
extern struct ICON_DS         main_task_ds;

const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";



void handle_request(struct netconn *conn, const char *buf_in);
int32_t parse_configs(const char *buf, char *name, char *var_in, int32_t var_in_len);
bool index_address(const char *buf, const char *sub_str);



bool index_address(const char *buf, const char *sub_str)
{
    return (strstr(buf, sub_str) != NULL);
}

/* Function to handle web server requests (CGI) */
void handle_request(struct netconn *conn, const char *buf_in)
{

    char   temp[128] ="";
    int8_t nIndex    = 0;



    if (index_address(buf_in, "GET /"))
    {
        // Header first.
        netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);
        if (index_address(buf_in, "GET / "))
        {
        	netconn_write(conn, status_html, sizeof(status_html)-1, NETCONN_NOCOPY);
        }
        else if (index_address(buf_in, "GET /icon_status.cgi"))
          {
              char* ptr=NULL;
              ptr = (char*)malloc(1024);
              memset(ptr, 0,  1024);


              for(nIndex = 0; nIndex < LOOP_NO; ++nIndex)
              {
                     sprintf(temp,"CHN%d," , nIndex + 1);  //channel
                     strcat(ptr,temp);
                     sprintf(temp,"%s," , main_task_ds.inputCard[nIndex].ipchannelname);  //name  main_task_ds.inputCard[0].ipchannelname
                     strcat(ptr,temp);

                     if(DIGITAL == main_task_ds.inputCard[nIndex].iptype)
                     {
                      sprintf(temp,"%d," , main_task_ds.inputCard[nIndex].dgipstat);  //Status main_task_ds.inputCard[0].pvvalue
                      strcat(ptr,temp);
                     }
                     else
                     {
                      sprintf(temp,"%0.2f," , main_task_ds.inputCard[nIndex].pvvalue);  //Status main_task_ds.inputCard[0].pvvalue
                      strcat(ptr,temp);
                     }

                     sprintf(temp,"%s," , main_task_ds.inputCard[nIndex].ipunit);  //Unit     main_task_ds.inputCard[0].ipunit
                     strcat(ptr,temp);


                     if(T2==main_task_ds.inputCard[nIndex].iptype)
                      {
                          sprintf(temp,"%s" , "Thermistor 2");
                          strcat(ptr,temp);
                      }
                     else if(V5==main_task_ds.inputCard[nIndex].iptype)
                      {
                          sprintf(temp,"%s" , "5V-Analog");
                          strcat(ptr,temp);
                      }
                     else if(V10==main_task_ds.inputCard[nIndex].iptype)
                      {
                          sprintf(temp,"%s" , "10V-Analog");
                          strcat(ptr,temp);
                      }
                      else if(DIGITAL==main_task_ds.inputCard[nIndex].iptype)
                      {
                          sprintf(temp,"%s" , "Digital");
                          strcat(ptr,temp);
                      }
                      else if(ANALOGFIXED==main_task_ds.inputCard[nIndex].iptype)
                      {
                          sprintf(temp,"%s" , "Analog_Fixed");
                          strcat(ptr,temp);
                      }
                      else
                      {
                          sprintf(temp,"%s" , "NA");
                          strcat(ptr,temp);
                      }


                     if(nIndex < (LOOP_NO-1))
                     {
                     sprintf(temp,"%s" , "|");
                     strcat(ptr,temp);
                     }
              }
              sprintf(temp,"%s" , "^");
              strcat(ptr,temp);

              for(nIndex = 0; nIndex < 2; ++nIndex)
              {
                  sprintf(temp,"CHN%d," , nIndex + 1);  //channel
                  strcat(ptr,temp);
                  sprintf(temp,"%s," , main_task_ds.outputCard[nIndex].opchannelname);

                  strcat(ptr,temp);


                  if(DIGITAL == main_task_ds.outputCard[nIndex].optype)
                  {
                      sprintf(temp,"%d," , main_task_ds.outputCard[nIndex].dgopstat);
                      strcat(ptr,temp);
                  }
                  else
                  {
                      sprintf(temp,"%0.2f," , main_task_ds.outputCard[nIndex].setpoint);
                      strcat(ptr,temp);
                  }

                  sprintf(temp,"%s," , main_task_ds.outputCard[nIndex].opunit);
                  strcat(ptr,temp);

                  if(ANALOG==main_task_ds.outputCard[nIndex].optype)
                  {
                      sprintf(temp,"%s" , "Analog");
                      strcat(ptr,temp);
                  }
                  else if(DIGITAL==main_task_ds.outputCard[nIndex].optype)
                  {
                      sprintf(temp,"%s" , "Digital");
                      strcat(ptr,temp);
                  }
                  else if(ANALOGFIXED==main_task_ds.outputCard[nIndex].optype)
                  {
                      sprintf(temp,"%s" , "Analog_Fixed");
                      strcat(ptr,temp);
                  }
                  else
                  {
                      sprintf(temp,"%s" , "NA");
                      strcat(ptr,temp);
                  }


                  if(nIndex < (1))
                  {
                      sprintf(temp,"%s" , "|");
                      strcat(ptr,temp);
                  }
              }

              sprintf(temp,"%s" , "^");
              strcat(ptr,temp);
              for(nIndex = 2; nIndex < 4; ++nIndex)
              {
                  sprintf(temp,"CHN%d," , nIndex + 1);  //channel
                  strcat(ptr,temp);

                  sprintf(temp,"%s," , main_task_ds.outputCard[nIndex].opchannelname);

                  strcat(ptr,temp);
                  if(DIGITAL == main_task_ds.outputCard[nIndex].optype)
                  {
                      if(1==main_task_ds.outputCard[nIndex].dgopstat){
                    	  sprintf(temp,"%s" ,"OFF");
                      }else{
                    	  sprintf(temp,"%s" ,"ON");
                      }

                      strcat(ptr,temp);
                  }
                  else
                  {
                      sprintf(temp,"%d" , main_task_ds.outputCard[nIndex].outputVoltage);
                      strcat(ptr,temp);
                  }


                  if(nIndex < (3))
                  {
                  sprintf(temp,"%s" , "|");
                  strcat(ptr,temp);
                  }
              }

              sprintf(temp,"%s" , "^");
              strcat(ptr,temp);

  #ifndef counter

              for(nIndex = 0; nIndex < MAX_COUNTER1_NUMBER; ++nIndex)
              {

                  sprintf(temp,"%s,%s," , "","");
                  strcat(ptr,temp);

                  if(nIndex < (MAX_COUNTER1_NUMBER-1))
                  {
                      sprintf(temp,"%s" , "|");
                      strcat(ptr,temp);
                  }
              }

  #endif
              sprintf(temp,"%s" , "^");
              strcat(ptr,temp);


  #ifndef  timer
              for(nIndex = 0; nIndex < MAX_TIMER_NUMBER; ++nIndex)
              {

                  sprintf(temp,"%s,%s," , "","");
                  strcat(ptr,temp);

                  if(nIndex < (MAX_TIMER_NUMBER-1))
                  {
                      sprintf(temp,"%s" , "|");
                      strcat(ptr,temp);
                  }
              }
  #endif
              sprintf(temp,"%s" , "^");
              strcat(ptr,temp);

              printf("size of ptr[%d]",strlen(ptr));
              printf("\n CGI output : [%s] \n",ptr);
              netconn_write(conn, ptr, strlen(ptr), NETCONN_NOCOPY);
              free(ptr);

          }

    }

}



/* Serve one HTTP connection accepted in the http thread.*/
void http_server_netconn_serve(struct netconn *conn)
{
    struct netbuf *inbuf;
    char *buf;
    u16_t buflen;
    err_t err;

    /* Read the data from the port, blocking if nothing yet there. */
    /* We assume the request (the part we care about) is in one netbuf */
    err = netconn_recv(conn, &inbuf);

    if (err == ERR_OK)
    {
        netbuf_data(inbuf, (void**)&buf, &buflen);
        handle_request(conn, buf);
    }
    /* Close the connection (server closes in HTTP) */
    netconn_close(conn);

    /* Delete the buffer (netconn_recv gives us ownership, */
    /* so we have to make sure to deallocate the buffer) */
    netbuf_delete(inbuf);
}

/*The HTTP Web server thread.*/
void http_server_netconn_thread(void *arg)
{
    struct netconn *conn, *newconn;
    err_t err;
    LWIP_UNUSED_ARG(arg);
    netif_set_up(&fsl_netif0);
    /* Create a new TCP connection handle */
    conn = netconn_new(NETCONN_TCP);
    LWIP_ERROR("http_server: invalid conn", (conn != NULL), return;);

    /* Bind to port 80 (HTTP) with default IP address */
    netconn_bind(conn, NULL, WEB_PORT);

    /* Put the connection into LISTEN state */
    netconn_listen(conn);

    do {
        err = netconn_accept(conn, &newconn);
        if (err == ERR_OK) {
            http_server_netconn_serve(newconn);
            netconn_delete(newconn);
        }
    } while(err == ERR_OK);
    LWIP_DEBUGF(HTTPD_DEBUG,
    ("http_server_netconn_thread: netconn_accept received error %d, shutting down", err));
    netconn_close(conn);
    netconn_delete(conn);
}

