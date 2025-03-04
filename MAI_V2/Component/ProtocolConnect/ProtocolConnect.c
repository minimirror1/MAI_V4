/*
 * ProtocolConnect.c
 *
 *  Created on: Nov 7, 2024
 *      Author: minim
 */

#include "main.h"
#include "ProtocolConnect.h"
#include "mal_motor_acPanasonic.h"

extern MAL_MOTOR_PanasonicHandleTypeDef mpanasonic;

/* Board Tx*/
void ProtocolConnect_sensor_detect_ctl(uint8_t axleId, uint8_t cwSen, uint8_t ccwSen){
			
    app_tx_sensor_sub_pid_detect_ctl(
        0,
        0,
        my_can_id_data.id,
        MASTER_CAN_ID,
        my_can_id_data.sub_id[0],
        0,
        cwSen,
        ccwSen);
}

void ProtocolConnect_RspAcAbsoBatteryOk(uint8_t axleId)
{
    uint32_t absoCount = 0;
	if (mpanasonic.setting.flag == MAL_SEN_INIT_OK) {
		absoCount = mpanasonic.setting.absoCount;
	} else {
		absoCount = 0;
	}

	app_tx_init_sub_pid_status_rsp(
			0,
			0,
			my_can_id_data.id,
			MASTER_CAN_ID,
			my_can_id_data.sub_id[0],
			0,
			ABSOLUTE_BATTERY,
			absoCount
			);
}

void ProtocolConnect_RspSensorInitSuccess(uint8_t axleId,int32_t absoCnt)
{
	app_tx_init_sub_pid_absolute_battery_ctl(
			0,
			0,
			my_can_id_data.id,
			MASTER_CAN_ID,
			my_can_id_data.sub_id[0],
			0,
			absoCnt
			);
}

void ProtocolConnect_RspDefPosi(uint8_t axleId, uint8_t initFlag)
{
	app_tx_init_sub_pid_status_ctl(
			0,
			0,
			my_can_id_data.id,
			MASTER_CAN_ID,
			my_can_id_data.sub_id[0],
			0,
			MOVE_INIT_POSITION,
			initFlag);
}

void ProtocolConnect_EventSensorDetect(uint8_t axleId, uint16_t value)
{
	app_tx_sensor_sub_pid_detect_ctl(
			0,
			0,
			my_can_id_data.id,
			MASTER_CAN_ID,
			my_can_id_data.sub_id[0],
			0,
			MAL_SENSOR_GetDetection(mpanasonic.ccwSen),
			MAL_SENSOR_GetDetection(mpanasonic.cwSen)
			//MAL_SENSOR_GetDetection(mpanasonic.cwSen),
			//MAL_SENSOR_GetDetection(mpanasonic.ccwSen)
			);
}

/* Board Rx */
void app_rx_init_sub_pid_move_sensor_ctl(uint8_t num, prtc_header_t *pPh, uint8_t *pData)
{
	if((my_can_id_data.id != pPh->target_id)||(my_can_id_data.sub_id[0] != pPh->target_sub_id))
		return;

	MAL_Motor_AcPanasonic_StartSenPosi(&mpanasonic);	

	app_tx_init_sub_pid_move_sensor_rsp(
				num,
				0,
				my_can_id_data.id,
				MASTER_CAN_ID,
				my_can_id_data.sub_id[0],
				0,
				0);
}

void app_rx_motion_sub_pid_adc_ctl(uint8_t num, prtc_header_t *pPh, prtc_data_ctl_motion_adc_t *pData)
{
	if((my_can_id_data.id != pPh->target_id)||(my_can_id_data.sub_id[0] != pPh->target_sub_id))
			return;

	uint16_t location;
	prtc_data_ctl_motion_adc_t *temp = (prtc_data_ctl_motion_adc_t *)pData;
	location = (uint16_t)temp->adc_val;

    MAL_Motor_AcPanasonic_SetChangeTarget(&mpanasonic, location);
}

void app_rx_init_sub_pid_driver_data1_ctl(uint8_t num, prtc_header_t *pPh, prtc_data_ctl_init_driver_data1_t *pData)
{
	if((my_can_id_data.id != pPh->target_id)||(my_can_id_data.sub_id[0] != pPh->target_sub_id))
			return;

	uint8_t SensorDirection;
	uint16_t OppositeLimit;
	uint16_t DefaultLocation;
	uint16_t ReductionRatio;

	prtc_data_ctl_init_driver_data1_t *temp = (prtc_data_ctl_init_driver_data1_t *)pData;

	SensorDirection = (uint8_t)temp->direction;
	OppositeLimit = (uint16_t)temp->angle;
	DefaultLocation = (uint16_t)temp->init_position;
	ReductionRatio = (uint16_t)temp->reducer_ratio;

	//제거 2022.04.18
	//home sensor 고정
    //	if(SensorDirection == MAL_SENSOR_CW)
    //	{
    //		MAL_Motor_AcPanasonic_SensorLimRegInit(
    //					&mpanasonic,
    //					&sensor[1],
    //					&sensor[0]);
    //	}
	
    MAL_Motor_AcPanasonic_SetSettingVal_AbsoluteVal(&mpanasonic, SensorDirection, OppositeLimit, DefaultLocation, ReductionRatio);

	app_tx_init_sub_pid_driver_data1_rsp(
			num,
			0,
			my_can_id_data.id,
			MASTER_CAN_ID,
			my_can_id_data.sub_id[0],
			0,
			SensorDirection,
			OppositeLimit,
			DefaultLocation,
			ReductionRatio);
}

void app_rx_init_sub_pid_absolute_battery_ctl(uint8_t num, prtc_header_t *pPh, prtc_data_ctl_init_absolute_battery_t *pData)
{
	uint32_t absoData = 0;
	if((my_can_id_data.id != pPh->target_id)||(my_can_id_data.sub_id[0] != pPh->target_sub_id))
			return;

	prtc_data_ctl_init_absolute_battery_t *temp = (prtc_data_ctl_init_absolute_battery_t *)pData;
	absoData = (uint32_t)temp->save_data;

	mpanasonic.setting.absoCount = absoData;

	app_tx_init_sub_pid_absolute_battery_rsp(
			num,
			0,
			my_can_id_data.id,
			MASTER_CAN_ID,
			my_can_id_data.sub_id[0],
			0,
			absoData);

}

void app_rx_init_sub_pid_move_init_position_ctl(uint8_t num, prtc_header_t *pPh, uint8_t *pData)
{
	if((my_can_id_data.id != pPh->target_id)||(my_can_id_data.sub_id[0] != pPh->target_sub_id))
			return;

	if(mpanasonic.setting.flag ==  MAL_SEN_INIT_OK)
	{
		mpanasonic.setting.DefultLocTempCnt = mpanasonic.status.position.now;
	}
	mpanasonic.setting.flag = MAL_SEN_DEF_LOCATION;

	app_tx_init_sub_pid_move_init_position_rsp(
			num,
			0,
			my_can_id_data.id,
			MASTER_CAN_ID,
			my_can_id_data.sub_id[0],
			0);
}
void app_rx_init_sub_pid_move_init_position_rqt(uint8_t num, prtc_header_t *pPh, uint8_t *pData)
{
	if((my_can_id_data.id != pPh->target_id)||(my_can_id_data.sub_id[0] != pPh->target_sub_id))
			return;

	app_tx_init_sub_pid_move_init_position_rsp(
			num,
			0,
			my_can_id_data.id,
			MASTER_CAN_ID,
			my_can_id_data.sub_id[0],
			0);
}
void app_rx_motion_sub_pid_direction_ctl(uint8_t num, prtc_header_t *pPh, prtc_data_ctl_motion_direction_t *pData)
{
	int16_t counter;
	if((my_can_id_data.id != pPh->target_id)||(my_can_id_data.sub_id[0] != pPh->target_sub_id))
			return;

	prtc_data_ctl_motion_direction_t *temp = (prtc_data_ctl_motion_direction_t *)pData;

	if(temp->direction == MOTION_DIRECTION_CCW)
		counter = -temp->val;
	else
	{
		counter = temp->val;
	}

	mpanasonic.jogCtr.activeflag = SET;
	mpanasonic.jogCtr.jogCnt = counter;
	mpanasonic.jogCtr.tJogMove = MAL_SysTimer_GetTickCount();
}


void app_rx_inspection_sub_pid_data_ctl(uint8_t num, prtc_header_t *pPh, prtc_data_ctl_inspection_data_t *pData)
{
	switch(pData->index)
	{
		case INSPECTION_PING:
		{
			prtc_data_ctl_inspection_ping_data_t *pCdcipd = (prtc_data_ctl_inspection_ping_data_t *)pData->payload;
			app_tx_inspection_sub_pid_data_ping_rsp(
					num,
					PRIORITY_NOMAL,
					my_can_id_data.id,
					pPh->souce_id,
					my_can_id_data.sub_id[0],
					pPh->souce_sub_id,
					INSPECTION_PING,
					pCdcipd->count);
		}
		break;
		case INSPECTION_VER:
		{
			prtc_data_rsp_inspection_ver_data_t device_ver_rec;
			device_ver_rec.ver1 = MAL_VERSION_MAJER;
			device_ver_rec.ver2 = MAL_VERSION_MINOR;
			device_ver_rec.ver3 = MAL_VERSION_BUILD;
			app_tx_inspection_sub_pid_data_ver_rsp(
					num,
					PRIORITY_NOMAL,
					my_can_id_data.id,
					pPh->souce_id,
					my_can_id_data.sub_id[0],
					pPh->souce_sub_id,
					INSPECTION_VER,
					device_ver_rec.ver);
		}
		break;
	}
}
//=============================================================================================================

void app_rx_init_sub_pid_absolute_battery_rqt(uint8_t num, prtc_header_t *pPh, uint8_t *pData)
{

	if((my_can_id_data.id != pPh->target_id)||(my_can_id_data.sub_id[0] != pPh->target_sub_id))
			return;

	uint32_t absoCount = 0;

    if(my_can_id_data.sub_id[0] != my_can_id_data.sub_id[0]){
        app_tx_init_sub_pid_absolute_battery_rsp(
        num,
        0,
        my_can_id_data.id,
        MASTER_CAN_ID,
        my_can_id_data.sub_id[0],
        0,			
        0);
    }else{
        if (mpanasonic.setting.flag == MAL_SEN_INIT_OK) {
            absoCount =  mpanasonic.setting.absoCount;
        } else {
            absoCount = 0;
        }	

        app_tx_init_sub_pid_absolute_battery_rsp(
        num,
        0,
        my_can_id_data.id,
        MASTER_CAN_ID,
        my_can_id_data.sub_id[0],
        0,			
        absoCount);
    }
}

void app_rx_init_sub_pid_status_rqt(uint8_t num, prtc_header_t *pPh, prtc_data_rqt_init_status_t *pData)
{
	uint8_t status = 0;

	if((my_can_id_data.id != pPh->target_id)||(my_can_id_data.sub_id[0] != pPh->target_sub_id))
			return;

	prtc_data_rqt_init_status_t *temp = (prtc_data_rqt_init_status_t *) pData;

	switch(temp->step)
	{
	//init step 1 : 1. vattery check
	case ABSOLUTE_BATTERY:
    	if(mpanasonic.setting.absoReadFlag == RESET)
        {
            mpanasonic.setting.absoStatus = 0;
            mpanasonic.setting.absoReadFlag = SET;
        }
        status = mpanasonic.setting.absoStatus;

		app_tx_init_sub_pid_status_rsp(
				num,
				0,
				my_can_id_data.id,
				MASTER_CAN_ID,
				my_can_id_data.sub_id[0],
				0,
				ABSOLUTE_BATTERY,
				status);
		break;
	case DRIVER_DATA1:

		break;
	case DRIVER_DATA2:

		break;
	case MOVE_SENSOR:
        
        status = mpanasonic.setting.flag;
		
		if(status == MAL_SEN_INIT_OK)
			status = 1;//ok
		else if(status == MAL_SEN_DEINIT)
			status = 2;//error
		else if(status == MAL_SEN_EMERGENCY_STOP)
			status = 2;//error
		else
			status = 0;//wait

		app_tx_init_sub_pid_status_rsp(
						num,
						0,
						my_can_id_data.id,
						MASTER_CAN_ID,
						my_can_id_data.sub_id[0],
						0,
						MOVE_SENSOR,
						status);

		break;
	case MOVE_INIT_POSITION:

		status = mpanasonic.setting.flag;
		if(status == MAL_SEN_INIT_OK)
			status = 1;//ok
		else if(status == MAL_SEN_DEINIT)
			status = 2;//error
		else if(status == MAL_SEN_EMERGENCY_STOP)
			status = 2;//error
		else
			status = 0;//wait

		//idtest
		app_tx_init_sub_pid_status_rsp(
						num,
						0,
						my_can_id_data.id,
						MASTER_CAN_ID,
						my_can_id_data.sub_id[0],
						0,
						MOVE_INIT_POSITION,
						status);

		break;
	}
}
