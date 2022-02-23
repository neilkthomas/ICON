/*
 * general_functions.c
 *
 *  Created on: Feb 15, 2022
 *      Author: Neil
 */


#include "icon_mqx_os.h"
#include "icon_ds.h"

/* Update ICON DS with initial data*/
void init_ds_structure(struct ICON_DS *main_task_ds){


strcpy(main_task_ds->inputCard[0].ipchannelname,INP_CHN_1);
strcpy(main_task_ds->inputCard[1].ipchannelname,INP_CHN_2);
strcpy(main_task_ds->inputCard[2].ipchannelname,INP_CHN_3);
strcpy(main_task_ds->inputCard[0].ipunit,VUNIT);
strcpy(main_task_ds->inputCard[1].ipunit,VUNIT);
strcpy(main_task_ds->inputCard[2].ipunit,VUNIT);
main_task_ds->inputCard[0].minval=MINVAL;
main_task_ds->inputCard[1].minval=MINVAL;
main_task_ds->inputCard[2].minval=MINVAL;
main_task_ds->inputCard[0].maxval=MAX3V3;
main_task_ds->inputCard[1].maxval=MAX3V3;
main_task_ds->inputCard[2].maxval=MAX3V3;
main_task_ds->inputCard[0].iptype = ANALOG;
main_task_ds->inputCard[1].iptype = ANALOG;
main_task_ds->inputCard[2].iptype = ANALOG;
main_task_ds->inputCard[0].pvoffset =OFFSETZERO;
main_task_ds->inputCard[1].pvoffset =OFFSETZERO;
main_task_ds->inputCard[2].pvoffset =OFFSETZERO;

strcpy(main_task_ds->outputCard[0].opchannelname,OUT_CHN_1);
strcpy(main_task_ds->outputCard[1].opchannelname,OUT_CHN_2);
strcpy(main_task_ds->outputCard[2].opchannelname,RLY_CHN_1);
strcpy(main_task_ds->outputCard[3].opchannelname,RLY_CHN_2);
strcpy(main_task_ds->outputCard[0].opunit,VUNIT);
strcpy(main_task_ds->outputCard[1].opunit,VUNIT);
strcpy(main_task_ds->outputCard[2].opunit,VUNIT);
strcpy(main_task_ds->outputCard[3].opunit,VUNIT);
main_task_ds->outputCard[0].optype = ANALOG;
main_task_ds->outputCard[1].optype = ANALOG;
main_task_ds->outputCard[2].optype = DIGITAL;
main_task_ds->outputCard[3].optype = DIGITAL;
main_task_ds->outputCard[0].setpoint =PWM1_DEFAULT_DUTY;
main_task_ds->outputCard[1].setpoint =PWM2_DEFAULT_DUTY;
main_task_ds->outputCard[2].setpoint =OFF;
main_task_ds->outputCard[3].setpoint =OFF;

}


/* Update Output message data*/
void main_task_msg_ds_update(struct ICON_DS *msg_ds,struct ICON_DS src_ds){
msg_ds->inputCard[0].iptype = src_ds.inputCard[0].iptype;
msg_ds->inputCard[1].iptype = src_ds.inputCard[1].iptype;
msg_ds->inputCard[2].iptype = src_ds.inputCard[2].iptype;
msg_ds->outputCard[0].setpoint =src_ds.outputCard[0].setpoint;
msg_ds->outputCard[1].setpoint =src_ds.outputCard[1].setpoint;
msg_ds->outputCard[2].setpoint =src_ds.outputCard[2].setpoint;
msg_ds->outputCard[3].setpoint =src_ds.outputCard[3].setpoint;
}


/* Update Input message data*/
void io_task_msg_ds_update(struct ICON_DS *msg_ds,struct ICON_DS src_ds){

msg_ds->inputCard[0].pvvalue = src_ds.inputCard[0].pvvalue;
msg_ds->inputCard[1].pvvalue = src_ds.inputCard[1].pvvalue;
msg_ds->inputCard[2].pvvalue = src_ds.inputCard[2].pvvalue;
msg_ds->outputCard[0].dgopstat = src_ds.outputCard[0].dgopstat;
msg_ds->outputCard[1].dgopstat = src_ds.outputCard[1].dgopstat;
msg_ds->outputCard[2].dgopstat = src_ds.outputCard[2].dgopstat;
msg_ds->outputCard[3].dgopstat = src_ds.outputCard[3].dgopstat;

}


#ifdef FUN_DISABLE
void msg_ds_update(struct ICON_DS *msg_ds,struct ICON_DS src_ds){


strcpy(msg_ds->inputCard[0].ipchannelname,src_ds.inputCard[0].ipchannelname);
strcpy(msg_ds->inputCard[1].ipchannelname,src_ds.inputCard[1].ipchannelname);
strcpy(msg_ds->inputCard[2].ipchannelname,src_ds.inputCard[2].ipchannelname);
strcpy(msg_ds->inputCard[0].ipunit,src_ds.inputCard[0].ipunit);
strcpy(msg_ds->inputCard[1].ipunit,src_ds.inputCard[1].ipunit);
strcpy(msg_ds->inputCard[2].ipunit,src_ds.inputCard[2].ipunit);
msg_ds->inputCard[0].minval=src_ds.inputCard[0].minval;
msg_ds->inputCard[1].minval=src_ds.inputCard[1].minval;
msg_ds->inputCard[2].minval=src_ds.inputCard[2].minval;
msg_ds->inputCard[0].maxval=src_ds.inputCard[0].maxval;
msg_ds->inputCard[1].maxval=src_ds.inputCard[1].maxval;
msg_ds->inputCard[2].maxval=src_ds.inputCard[2].maxval;
msg_ds->inputCard[0].iptype = src_ds.inputCard[0].iptype;
msg_ds->inputCard[1].iptype = src_ds.inputCard[1].iptype;
msg_ds->inputCard[2].iptype = src_ds.inputCard[2].iptype;
msg_ds->inputCard[0].pvoffset =src_ds.inputCard[0].pvoffset;
msg_ds->inputCard[1].pvoffset =src_ds.inputCard[1].pvoffset;
msg_ds->inputCard[2].pvoffset =src_ds.inputCard[2].pvoffset;
msg_ds->inputCard[0].pvvalue = src_ds.inputCard[0].pvvalue;
msg_ds->inputCard[1].pvvalue = src_ds.inputCard[1].pvvalue;
msg_ds->inputCard[2].pvvalue = src_ds.inputCard[2].pvvalue;


strcpy(msg_ds->outputCard[0].opchannelname,src_ds.outputCard[0].opchannelname);
strcpy(msg_ds->outputCard[1].opchannelname,src_ds.outputCard[1].opchannelname);
strcpy(msg_ds->outputCard[2].opchannelname,src_ds.outputCard[2].opchannelname);
strcpy(msg_ds->outputCard[3].opchannelname,src_ds.outputCard[3].opchannelname);
strcpy(msg_ds->outputCard[0].opunit,src_ds.outputCard[0].opunit);
strcpy(msg_ds->outputCard[1].opunit,src_ds.outputCard[1].opunit);
strcpy(msg_ds->outputCard[2].opunit,src_ds.outputCard[2].opunit);
strcpy(msg_ds->outputCard[3].opunit,src_ds.outputCard[3].opunit);
msg_ds->outputCard[0].optype = src_ds.outputCard[0].optype;
msg_ds->outputCard[1].optype = src_ds.outputCard[1].optype;
msg_ds->outputCard[2].optype = src_ds.outputCard[2].optype;
msg_ds->outputCard[3].optype = src_ds.outputCard[3].optype;
msg_ds->outputCard[0].setpoint =src_ds.outputCard[0].setpoint;
msg_ds->outputCard[1].setpoint =src_ds.outputCard[1].setpoint;
msg_ds->outputCard[2].setpoint =src_ds.outputCard[2].setpoint;
msg_ds->outputCard[3].setpoint =src_ds.outputCard[3].setpoint;

msg_ds->outputCard[0].dgopstat = src_ds.outputCard[0].dgopstat;
msg_ds->outputCard[1].dgopstat = src_ds.outputCard[1].dgopstat;
msg_ds->outputCard[2].dgopstat = src_ds.outputCard[2].dgopstat;
msg_ds->outputCard[3].dgopstat = src_ds.outputCard[3].dgopstat;

}

#endif
