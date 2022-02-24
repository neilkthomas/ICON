/*
 * Io_manage.c
 *
 *  Created on: Feb 5, 2022
 *      Author: Neil
 */


#include "icon_mqx_os.h"
#include "icon_ds.h"
#include <mqx.h>
#include <message.h>
#include <mutex.h>


/* declaration of a global message pool */
_pool_id   message_pool;
_pool_id fp_message_pool;

extern MUTEX_STRUCT   main_ds_mutex;
MUTEX_STRUCT   io_ds_mutex;

/* Read ADC values and convert into float*/
float adc_read(char channel,char instance)
{
	uint32_t adcValue = 0;
    adc16_chn_config_t chnConfig;
    chnConfig.chnIdx     =  channel;
    ADC16_DRV_ConfigConvChn(instance, ADC16_CHN_GROUP, &chnConfig);
    /* Wait for the conversion to be done. */
    ADC16_DRV_WaitConvDone(instance, ADC16_CHN_GROUP);
    /* Fetch the conversion value. */
    adcValue = ADC16_DRV_GetConvValueRAW(instance, ADC16_CHN_GROUP);
    return(adcValue/UINT16_3V3_CONVERT);
}

/*Task to read ADC channels and control PWM Duty, Digital outputs*/
/* Send IO values to main task and read output configuration values from it
 * Data exchange via message queue*/
void io_manage_task(uint32_t param)
{

    char count_adc;
    volatile float adc_thm1 =0;
    volatile float adc_thm2=0;
    volatile float adc_chn1=0;
    volatile float adc_chn2=0;
    volatile float adc_chn3=0;

    adc16_converter_config_t adcUserConfig;

    static float prev_chn1_val=0;
    static float prev_chn2_val=0;
    static struct ICON_DS         io_task_ds;

    /* Message Pool Creation for data exchange between IO Manage task and Main Task thread*/

    SERVER_MESSAGE_PTR msg_ptr;
    _queue_id          server_qid;
    bool            result;

    /* open a message queue */
    server_qid = _msgq_open(SERVER_QUEUE, 0);

    if (server_qid == 0) {
       printf("\nCould not open the server message queue\n");
    }

    /* create a message pool */
    message_pool = _msgpool_create(sizeof(SERVER_MESSAGE),NUM_CLIENTS, 0, 0);

    if (message_pool == MSGPOOL_NULL_POOL_ID) {
       printf("\nCount not create a message pool\n");
    }

    /***********************************************/

    /* Set Digital Outputs / Tricolor LED in FRDM Board */
    GPIO_DRV_Init(NULL,ledPins);
    GPIO_DRV_WritePinOutput(kGpioLED1,1); //green 0-ON,  1 off
    GPIO_DRV_WritePinOutput(kGpioLED2,1); //red

    /* Read Analog Inputs */
    ADC16_DRV_StructInitUserConfigDefault(&adcUserConfig);
    adcUserConfig.resolution = kAdc16ResolutionBitOf16;
    ADC16_DRV_Init(ADC16_INSTANCE, &adcUserConfig);
    ADC16_DRV_Init(ADC16_INSTANCE1, &adcUserConfig);

    /* Initialize FTM module */
    memset(&flexTimer1_InitConfig0, 0, sizeof(flexTimer1_InitConfig0));
    flexTimer1_InitConfig0.syncMethod = kFtmUseSoftwareTrig;
    FTM_DRV_Init(BOARD_FTM_INSTANCE, &flexTimer1_InitConfig0);
    FTM_DRV_SetClock(BOARD_FTM_INSTANCE, kClock_source_FTM_SystemClk, kFtmDividedBy1);

    flexTimer1_ChnConfig0.uDutyCyclePercent = PWM1_DEFAULT_DUTY;
    FTM_DRV_PwmStart(BOARD_FTM_INSTANCE, &flexTimer1_ChnConfig0, 3U);
    flexTimer1_ChnConfig0.uDutyCyclePercent = PWM2_DEFAULT_DUTY;
    FTM_DRV_PwmStart(BOARD_FTM_INSTANCE, &flexTimer1_ChnConfig0, 1U);

     /* Software trigger to update registers. */
    FTM_HAL_SetSoftwareTriggerCmd(g_ftmBase[BOARD_FTM_INSTANCE], true);


/*
 *   INPUT & OUTPUT PIN, MODULE details
 *
 *   THM1  ADC0_SE12
 *   THM2  ADC0_SE13
 *   CHN1  ADC1_SE18
 *   CHN2  PTB10/ADC1_SE14
 *   CHN3  PTB 11/ADC1_SE15
 *   PWM1  PTC4
 *   PWM2  PTC2
 *
 */

    while(1)
    {

    	/******* Message Queue to receive ICON DS from Main Task ********/

        msg_ptr = _msgq_receive(server_qid, 0);

        if (msg_ptr == NULL) {
           printf("\nCould not receive a message\n");
        }else
        {
            /** Copying Message Queue payload to IO Task DS structure**/
            if (_mutex_lock(&main_ds_mutex) != MQX_OK) {
               printf("Main DS Mutex lock failed.\n");
            }
        	main_task_msg_ds_update(&io_task_ds,msg_ptr->DS);
            _mutex_unlock(&main_ds_mutex);
            /**********************************************************/
        }

        /******End Message Queue to receive ICON DS from Main Task ******/


        adc_thm1=0;
        adc_thm2=0;
        adc_chn1=0;
        adc_chn2=0;
        adc_chn3=0;

            /* ADC Thermistor 1 ADC0_SE12 */
            count_adc=ADC_NO_SAMPLES;
            while(count_adc)
            {
                adc_thm1 = adc_thm1 + (adc_read(ADC_THM1_PIN_ADC0_SE12,ADC16_INSTANCE));
                count_adc--;
            }

        	/* ADC CHN 2  ADC1_SE14 */
            count_adc = ADC_NO_SAMPLES;
            while(count_adc)
            {
                adc_chn2 = adc_chn2 + adc_read(ADC_CHN2_PIN_ADC1_SE14,ADC16_INSTANCE1);
                count_adc--;
            }

            if(T2 == io_task_ds.inputCard[1].iptype)
            {
                io_task_ds.inputCard[1].pvvalue = (adc_thm1/ADC_NO_SAMPLES);
            }
            else if(DIGITAL == io_task_ds.inputCard[1].iptype)
            {
                if((adc_chn2/ADC_NO_SAMPLES)>DIGITAL_IN_THRESHOLD)
                {
                    io_task_ds.inputCard[1].pvvalue=ON;
                }
                else
                {
                    io_task_ds.inputCard[1].pvvalue=OFF;
                }
            }
            else if(V10 == io_task_ds.inputCard[1].iptype)
            {
                io_task_ds.inputCard[1].pvvalue = (adc_chn2/ADC_NO_SAMPLES);
            }
            else if(V5 == io_task_ds.inputCard[1].iptype)
            {
                io_task_ds.inputCard[1].pvvalue = (adc_chn2/ADC_NO_SAMPLES);

            }


            /* ADC Thermistor 2  ADC0_SE13 */
            count_adc = ADC_NO_SAMPLES;
            while(count_adc)
            {
                adc_thm2 = adc_thm2 + (adc_read(ADC_THM2_PIN_ADC0_SE13,ADC16_INSTANCE));
                count_adc--;
            }

            /* ADC CHN 3  ADC1_SE15 */
             count_adc = ADC_NO_SAMPLES;
             while(count_adc)
             {
                 adc_chn3 = adc_chn3 + adc_read(ADC_CHN3_PIN_ADC1_SE15,ADC16_INSTANCE1);
                 count_adc--;
             }

            if(T2 == io_task_ds.inputCard[2].iptype)
            {
                io_task_ds.inputCard[2].pvvalue = (adc_thm2/ADC_NO_SAMPLES);
            }
            else if(DIGITAL == io_task_ds.inputCard[2].iptype)
            {
                if((adc_chn3/ADC_NO_SAMPLES)>DIGITAL_IN_THRESHOLD)
                {
                    io_task_ds.inputCard[2].pvvalue = ON;
                }
                else
                {
                    io_task_ds.inputCard[2].pvvalue = OFF;
                }
            }
            else if(V10 == io_task_ds.inputCard[2].iptype)
            {
                io_task_ds.inputCard[2].pvvalue = (adc_chn3/ADC_NO_SAMPLES);
            }
            else if(V5 == io_task_ds.inputCard[2].iptype)
            {
                io_task_ds.inputCard[2].pvvalue = (adc_chn3/ADC_NO_SAMPLES);
            }

            /* ADC CHN1 / ADC1_SE18 */
            count_adc = ADC_NO_SAMPLES;
            while(count_adc)
            {
                adc_chn1 = adc_chn1 + adc_read(ADC_CHN1_PIN_ADC1_SE18,ADC16_INSTANCE1);
                count_adc--;
            }

            io_task_ds.inputCard[0].pvvalue = (adc_chn1/ADC_NO_SAMPLES);

           if(prev_chn1_val!=io_task_ds.outputCard[0].setpoint) {

			   flexTimer1_ChnConfig0.uDutyCyclePercent = (uint32_t)io_task_ds.outputCard[0].setpoint;
				// Start PWM mode with updated duty cycle.
			   FTM_DRV_PwmStart(BOARD_FTM_INSTANCE, &flexTimer1_ChnConfig0, FTM0_CH3_PIN_PTC4);
			   // Software trigger to update registers.
			   FTM_HAL_SetSoftwareTriggerCmd(g_ftmBase[BOARD_FTM_INSTANCE], true);
			   prev_chn1_val=io_task_ds.outputCard[0].setpoint;
           }

           if(prev_chn2_val !=io_task_ds.outputCard[1].setpoint){

			   flexTimer1_ChnConfig0.uDutyCyclePercent = (uint32_t)io_task_ds.outputCard[1].setpoint;
			   FTM_DRV_PwmStart(BOARD_FTM_INSTANCE, &flexTimer1_ChnConfig0, FTM0_CH1_PIN_PTC2);
				// Software trigger to update registers.
			   FTM_HAL_SetSoftwareTriggerCmd(g_ftmBase[BOARD_FTM_INSTANCE], true);
			   prev_chn2_val =io_task_ds.outputCard[1].setpoint;
           }

            if(RELAY_ON == io_task_ds.outputCard[2].setpoint){
                GPIO_DRV_WritePinOutput(kGpioLED1,LED_ON);
            }else{
                GPIO_DRV_WritePinOutput(kGpioLED1,LED_OFF);
            }
            if(RELAY_ON == io_task_ds.outputCard[3].setpoint){
                GPIO_DRV_WritePinOutput(kGpioLED2,LED_ON);
            }else{
                GPIO_DRV_WritePinOutput(kGpioLED2,LED_OFF);
            }

            io_task_ds.outputCard[2].dgopstat=(int8_t)GPIO_DRV_ReadPinInput(kGpioLED1);
            io_task_ds.outputCard[3].dgopstat=(int8_t)GPIO_DRV_ReadPinInput(kGpioLED2);

            /*********** Message Queue to send IO DS to Main Task *****/

            /** Copying IO Task DS structure to Message Queue payload **/
            if (_mutex_lock(&io_ds_mutex) != MQX_OK) {
               printf("IO DS Mutex lock failed.\n");
            }
            io_task_msg_ds_update(&msg_ptr->DS,io_task_ds);
            _mutex_unlock(&io_ds_mutex);
            /**********************************************************/

            /* return the message */
            msg_ptr->HEADER.TARGET_QID = msg_ptr->HEADER.SOURCE_QID;
            msg_ptr->HEADER.SOURCE_QID = server_qid;

            result = _msgq_send(msg_ptr);
            if (result != TRUE) {
               printf("\nCould not send a message\n");
            }

        	/********* End Message Queue to send IO DS to Main Task ******/


        /* Let the other task run */
        _time_delay(1);
   }
}
