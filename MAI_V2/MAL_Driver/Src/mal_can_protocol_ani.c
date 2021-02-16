/*
 * mal_can_protocol_ani.c
 *
 *  Created on: 2020. 3. 11.
 *      Author: shin
 */

#include "mal_board_info.h"
#include "main.h"
#include "mal_can.h"
#include "mal_can_protocol_ani.h"
#include "mal_motor.h"

#ifdef HAL_CAN_MODULE_ENABLED


extern MAL_CAN_HandleTypeDef mcan1;

extern MAL_MOTOR_HandleTypeDef mmotor[MOTOR_AXLE_CNT];

extern MAL_MOTOR_ManagerHandleTypeDef motorManager;



/*===================================================================
 * MAL_Protocol_Rsp  CAN
===================================================================*/
//200611
void MAL_Protocol_Ani_AlmSensorDetection(uint8_t axleId, uint8_t cwSen, uint8_t ccwSen)
{
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_DataHeader)+sizeof(MAL_CAN_Protocol_Ani_AlmSensorDetection)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_AlmSensorDetection *pData = (MAL_CAN_Protocol_Ani_AlmSensorDetection *)pdataHeader->payload;

	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_RESPONE;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_SENSOR_DETECTION;

	pdataHeader->length = sizeof(pTemp);
#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetReloadValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif
	pData->axleId = axleId + 1;

	pData->cwSen = cwSen;
	pData->ccwSen = ccwSen;

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}
void MAL_Protocol_Ani_RspDefPosi(uint8_t axleId, uint8_t initFlag)
{
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_DataHeader)+sizeof(MAL_CAN_Protocol_Ani_RspSensorInit)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_RspSensorInitSuccess *pData = (MAL_CAN_Protocol_Ani_RspSensorInitSuccess *)pdataHeader->payload;

	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_RESPONE;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_SET_INITPOSI;

	pdataHeader->length = sizeof(pTemp);
#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetReloadValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif
	pData->axleId = axleId + 1;

	pData->flag = initFlag;

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}


void MAL_Protocol_Ani_RspSensorInitSuccess(uint8_t axleId, uint32_t absoCnt) {
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_DataHeader)+sizeof(MAL_CAN_Protocol_Ani_RspSensorInit)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_RspSensorInitSuccess *pData = (MAL_CAN_Protocol_Ani_RspSensorInitSuccess *)pdataHeader->payload;

	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_RESPONE;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_SET_AC;

	pdataHeader->length = sizeof(pTemp);
#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetReloadValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif
	pData->axleId = axleId + 1;

	pData->flag = MAL_Motor_GetSettingFlag(axleId);

	pData->absoCnt = absoCnt;

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}

void MAL_Protocol_Ani_RspBldcInfo(uint8_t axleId)
{
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_DataHeader)+sizeof(MAL_CAN_Protocol_Ani_RspSensorInit)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_RspSensorInit *pData = (MAL_CAN_Protocol_Ani_RspSensorInit *)pdataHeader->payload;

	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_RESPONE;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_BLDC_INIT;

	pdataHeader->length = sizeof(pTemp);
#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetReloadValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif

	pData->axleId = axleId + 1;
	pData->flag = MAL_Motor_GetSettingFlag(axleId);

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}

void MAL_Protocol_Ani_RspSensorInit(uint8_t axleId)
{
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_DataHeader)+sizeof(MAL_CAN_Protocol_Ani_RspSensorInit)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_RspSensorInit *pData = (MAL_CAN_Protocol_Ani_RspSensorInit *)pdataHeader->payload;

	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_RESPONE;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_SET_AC;

	pdataHeader->length = sizeof(pTemp);
#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetReloadValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif

	pData->axleId = axleId + 1;
	pData->flag = MAL_Motor_GetSettingFlag(axleId);

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}

void MAL_Protocol_Ani_RspSensorInit_Abso(uint8_t axleId,uint8_t status)
{
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_DataHeader)+sizeof(MAL_CAN_Protocol_Ani_RspSensorInit)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_RspSensorInit *pData = (MAL_CAN_Protocol_Ani_RspSensorInit *)pdataHeader->payload;

	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_RESPONE;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_SET_AC;

	pdataHeader->length = sizeof(pTemp);
#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetReloadValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif

	pData->axleId = axleId + 1;
	pData->flag = MAL_Motor_GetSettingFlag(axleId);

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}

void MAL_Protocol_Ani_RspSensorDeinit(uint8_t axleId)
{
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_DataHeader)+sizeof(MAL_CAN_Protocol_Ani_RspSensorInit)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_RspSensorInit *pData = (MAL_CAN_Protocol_Ani_RspSensorInit *)pdataHeader->payload;

	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_CONTROL;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_SENSOR_DEINIT;

	pdataHeader->length = sizeof(pTemp);
#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetReloadValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif
	pData->axleId = axleId + 1;
	pData->flag = MAL_Motor_GetSettingFlag(axleId);

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}

void MAL_Protocol_Ani_RspAllSensorInitStatus(uint8_t axleId)
{
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_DataHeader)+sizeof(MAL_CAN_Protocol_Ani_RspSensorInit)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_RspSensorInit *pData = (MAL_CAN_Protocol_Ani_RspSensorInit *)pdataHeader->payload;

	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_RESPONE;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_RQS_SENSOR_STATUS_ALL_REQ;

	pdataHeader->length = sizeof(pTemp);
#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetReloadValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif
	pData->axleId = axleId + 1;
	pData->flag = MAL_Motor_GetSettingFlag(axleId);
	pData->loadAbsoCnt = MAL_Motor_GetAbsoCountOk(axleId);

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}

void MAL_Protocol_Ani_RspFwVer(void)
{
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_RspFwVer)+sizeof(MAL_CAN_Protocol_Ani_DataHeader)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_RspFwVer *pData = (MAL_CAN_Protocol_Ani_RspFwVer *)pdataHeader->payload;

	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_RESPONE;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_RQS_VER;

	pdataHeader->length = sizeof(pTemp);

#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetReloadValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif

	pData->ver1 = MAL_Board_Version_GetValueMajer();
	pData->ver2 = MAL_Board_Version_GetValueMinor();
	pData->ver3 = MAL_Board_Version_GetValueBuild();

	//mboard

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}

void MAL_Protocol_Ani_RspAcAbsoBattery(MAL_CAN_Protocol_Ani_ExtIdTypedef *extId, uint8_t *pdata)
{
	//rx 해석
	MAL_CAN_Protocol_Ani_DataHeader *pDataHeader = (MAL_CAN_Protocol_Ani_DataHeader*) pdata;
	MAL_CAN_Protocol_Ani_RspAcAbsoBattery *pParsingData = (MAL_CAN_Protocol_Ani_RspAcAbsoBattery*) pDataHeader->payload;

	//tx 생성
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_RspAcAbsoBattery)+sizeof(MAL_CAN_Protocol_Ani_DataHeader)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_RspAcAbsoBattery *pData = (MAL_CAN_Protocol_Ani_RspAcAbsoBattery *)pdataHeader->payload;



	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_RESPONE;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_BATTERY;

	pdataHeader->length = sizeof(pTemp);

#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetReloadValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif

	pData->axleId = pParsingData->axleId;
	pData->status = MAL_Motor_GetAbsoStatus(pParsingData->axleId-1);
	MAL_Motor_SetLoadAbsoCnt(pParsingData->axleId-1,pParsingData->loadAbsoCnt);

	//mboard

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}

void MAL_Protocol_Ani_RspAcAbsoBatteryOk(uint8_t axleId)
{
	//tx 생성
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_RspAcAbsoBattery)+sizeof(MAL_CAN_Protocol_Ani_DataHeader)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_RspAcAbsoBattery *pData = (MAL_CAN_Protocol_Ani_RspAcAbsoBattery *)pdataHeader->payload;



	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_RESPONE;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_BATTERY;

	pdataHeader->length = sizeof(pTemp);

#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetReloadValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif

	pData->axleId = axleId+1;
	pData->status = MAL_Motor_GetAbsoStatusOk(axleId);
//	MAL_Motor_SetLoadAbsoCnt(paxleId+1,pParsingData->loadAbsoCnt);

	//mboard

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}
/*===================================================================
 * MAL_Protocol_Event  CAN
===================================================================*/
void MAL_Protocol_Ani_EventBootAlm(void)
{
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_DataHeader)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;

	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_MOTOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_CONTROL;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_BOOT;

	pdataHeader->length = 0;
	pdataHeader->src_id = MAL_Board_ID_GetValue();

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}


void MAL_Protocol_Ani_EventSensorDetect(uint8_t axleId, uint16_t value)
{
	//xxx 수정해야됨
	MAL_CAN_Protocol_Ani_ExtIdTypedef Header;

	uint8_t pTemp[sizeof(MAL_CAN_Protocol_Ani_Pwm)+sizeof(MAL_CAN_Protocol_Ani_DataHeader)] = {0,};
	MAL_CAN_Protocol_Ani_DataHeader *pdataHeader = (MAL_CAN_Protocol_Ani_DataHeader *)pTemp;
	MAL_CAN_Protocol_Ani_Pwm *pData = (MAL_CAN_Protocol_Ani_Pwm *)pdataHeader->payload;


	Header.emergency = CAN_EMERGENCY;
	Header.Class = CAN_CLASS_SENSOR;
	Header.page_num = CAN_PAGE_NUM_0;
	Header.tar_id = CAN_MASTER_ID;
	Header.Case = CAN_CASE_CONTROL;
	Header.payload_en = CAN_PAGE_DEN;
	Header.sub_cmd = SUB_CMD_SET_ADC;

	pdataHeader->length = sizeof(MAL_CAN_Protocol_Ani_Pwm);

#ifdef MAL_TEST_MODE
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#else
	pdataHeader->src_id = MAL_Board_ID_GetValue();
#endif

	pData->axleId = axleId;
	pData->val = value;

	MAL_CAN_SendAddQueue_ExtData(motorManager.mcan,Header.can_header_32, pTemp, sizeof(pTemp));
}

/*===================================================================
 * MAL_Protocol_Ani_Control
===================================================================*/
void MAL_Protocol_Ani_Ctr_SetPwmVal(MAL_CAN_Protocol_Ani_ExtIdTypedef *extId, uint8_t *pdata)
{

	MAL_CAN_Protocol_Ani_DataHeader *pDataHeader = (MAL_CAN_Protocol_Ani_DataHeader *) pdata;
	MAL_CAN_Protocol_Ani_Pwm *pParsingData = (MAL_CAN_Protocol_Ani_Pwm *) pDataHeader->payload;

	uint16_t val = pParsingData->val;

	if(pParsingData->axleId == MOTOR_BROADCAST_ID)
	{
		MAL_Motor_SetAllLocation(val);
		return;
	}


	pParsingData->axleId--;

	if (MAL_Motor_GetSettingFlag(pParsingData->axleId) != MAL_SEN_INIT_OK) {

		MAL_Protocol_Ani_RspSensorDeinit(pParsingData->axleId);
		return; //센서 초기화 안됨
	}

	//todo  디버깅용 삭제
//	if(val > 3900)
//	{
//		val = 4095;
//	}
//	else if(val <200)
//	{
//		val = 0;
//	}
	//
	MAL_Motor_SetLocation(pParsingData->axleId, val);

}

void MAL_Protocol_Ani_Ctr_SetBldcInfo(MAL_CAN_Protocol_Ani_ExtIdTypedef *extId, uint8_t *pdata)
{
	MAL_CAN_Protocol_Ani_DataHeader *pDataHeader = (MAL_CAN_Protocol_Ani_DataHeader *) pdata;
	can_protocol_cmd_bldc_init_t *pParsingData = (can_protocol_cmd_bldc_init_t *) pDataHeader->payload;
	pParsingData->axleId--;

		if (pParsingData->axleId < 4) {

			MAL_Motor_SetBldcInfo(
					pParsingData->axleId,
					pParsingData->count,
					pParsingData->rpm);

			MAL_Protocol_Ani_RspBldcInfo(pParsingData->axleId);
		}
}

void MAL_Protocol_Ani_Ctr_SetSensorInit(MAL_CAN_Protocol_Ani_ExtIdTypedef *extId, uint8_t *pdata)
{
	MAL_CAN_Protocol_Ani_DataHeader *pDataHeader = (MAL_CAN_Protocol_Ani_DataHeader *) pdata;
	MAL_CAN_Protocol_Ani_AcInit *pParsingData = (MAL_CAN_Protocol_Ani_AcInit *) pDataHeader->payload;
	pParsingData->id--;

		if (pParsingData->id < 4) {

			MAL_Motor_SetSetting(
					pParsingData->id,
					pParsingData->SensorDirection,
					pParsingData->OppositeLimit,
					pParsingData->DefaultLocation,
					pParsingData->ReductionRatio);

			MAL_Protocol_Ani_RspSensorInit(pParsingData->id);
		}
}

void MAL_Protocol_Ani_Ctr_SetSensorInit_Abso(MAL_CAN_Protocol_Ani_ExtIdTypedef *extId, uint8_t *pdata)
{
	MAL_CAN_Protocol_Ani_DataHeader *pDataHeader = (MAL_CAN_Protocol_Ani_DataHeader*) pdata;
	MAL_CAN_Protocol_Ani_AcInit *pParsingData = (MAL_CAN_Protocol_Ani_AcInit*) pDataHeader->payload;
	pParsingData->id--;

	if (pParsingData->id < 4) {

		if (MAL_Motor_SetSetting_Absolute(
				pParsingData->id,
				pParsingData->SensorDirection,
				pParsingData->OppositeLimit,
				pParsingData->DefaultLocation,
				pParsingData->ReductionRatio) != 0) {
			MAL_Protocol_Ani_RspSensorInit(pParsingData->id);
		}
	}
}

void MAL_Protocol_Ani_Ctr_SetInitPosi(MAL_CAN_Protocol_Ani_ExtIdTypedef *extId, uint8_t *pdata)
{
	MAL_CAN_Protocol_Ani_DataHeader *pDataHeader = (MAL_CAN_Protocol_Ani_DataHeader *) pdata;
	MAL_CAN_Protocol_Ani_AcInit *pParsingData = (MAL_CAN_Protocol_Ani_AcInit *) pDataHeader->payload;

	//uint8_t i = 0;

	pParsingData->id--;

	if(pParsingData->id <4)
	{
		if (MAL_Motor_GetSettingFlag(pParsingData->id) == MAL_SEN_INIT_OK)
		{
			MAL_Motor_SetDefaultLocation(pParsingData->id);
			//MAL_Protocol_Ani_RspDefPosi(i,MAL_SEN_INIT_ING);
		}
		else if (MAL_Motor_GetSettingFlag(pParsingData->id) == MAL_SEN_DEF_LOCATION)
		{
			MAL_Protocol_Ani_RspDefPosi(pParsingData->id, MAL_SEN_INIT_ING);
		}
	}

//	for(i = 0; i < MOTOR_AXLE_CNT; i++)
//	{
//		if(MAL_Motor_GetSettingFlag(i) == MAL_SEN_INIT_OK)
//		{
//			MAL_Motor_SetDefaultLocation(i);
//			//MAL_Protocol_Ani_RspDefPosi(i,MAL_SEN_INIT_ING);
//		}
//		else if(MAL_Motor_GetSettingFlag(i) == MAL_SEN_DEF_LOCATION)
//		{
//			MAL_Protocol_Ani_RspDefPosi(i,MAL_SEN_INIT_ING);
//		}
//
//	}
}

void MAL_Protocol_Ani_Ctr_SetAcCount(MAL_CAN_Protocol_Ani_ExtIdTypedef *extId, uint8_t *pdata)
{

	MAL_CAN_Protocol_Ani_DataHeader *pDataHeader = (MAL_CAN_Protocol_Ani_DataHeader *) pdata;
	MAL_CAN_Protocol_Ani_Pwm *pParsingData = (MAL_CAN_Protocol_Ani_Pwm *) pDataHeader->payload;

	int16_t val = pParsingData->val;

/*	if(pParsingData->id == MOTOR_BROADCAST_ID)
	{
		MAL_Motor_SetAllLocation(val);
		return;
	}*/


	pParsingData->axleId--;


	MAL_Motor_SetJogCounter(pParsingData->axleId, val);
}

void MAL_Protocol_Ani_Ctr_SetAxleEnable(MAL_CAN_Protocol_Ani_ExtIdTypedef *extId, uint8_t *pdata)
{
	MAL_CAN_Protocol_Ani_DataHeader *pDataHeader = (MAL_CAN_Protocol_Ani_DataHeader *) pdata;
	MAL_CAN_Protocol_Ani_AxleEnable *pParsingData = (MAL_CAN_Protocol_Ani_AxleEnable *) pDataHeader->payload;

	pParsingData->id--;




}

/*===================================================================
 * MAL_Protocol_Ani_Request
===================================================================*/

void MAL_Protocol_Ani_SensorInitStatus_AllReq(void)
{
	uint8_t axleNum = MAL_Motor_GetAxleNum();
	uint8_t i = 0;

	HAL_Delay(MAL_Board_ID_GetValue()*2);
	for(i = 0; i < axleNum ; i++)
	{
		MAL_Protocol_Ani_RspAllSensorInitStatus(i);
	}
}


void MAL_Protocol_Ani_FwVerReq(void)
{
	MAL_Protocol_Ani_RspFwVer();
}
void MAL_Protocol_Ani_AcAbsoBatteryReq(MAL_CAN_Protocol_Ani_ExtIdTypedef *extId, uint8_t *pdata)
{

/*	MAL_CAN_Protocol_Ani_DataHeader *pDataHeader = (MAL_CAN_Protocol_Ani_DataHeader *) pdata;
	MAL_CAN_Protocol_Ani_RspAcAbsoBattery *pParsingData = (MAL_CAN_Protocol_Ani_RspAcAbsoBattery *) pDataHeader->payload;*/

	MAL_Protocol_Ani_RspAcAbsoBattery(extId,pdata);
}


/*===================================================================
 * 데이터 취득 및 해석
===================================================================*/
void MAL_Protocol_Ani_Control(MAL_CAN_Protocol_Ani_ExtIdTypedef *extId, uint8_t *pdata)
{
	switch (extId->sub_cmd) {
	case CAN_SUB_CMD_PWM:
		MAL_Protocol_Ani_Ctr_SetPwmVal(extId, pdata);
		break;
	case SUB_CMD_BLDC_INIT:
		MAL_Protocol_Ani_Ctr_SetBldcInfo(extId, pdata);
		break;
	case SUB_CMD_SET_AC:
		MAL_Protocol_Ani_Ctr_SetSensorInit(extId, pdata);
		break;
	case SUB_CMD_SET_INITPOSI:
		MAL_Protocol_Ani_Ctr_SetInitPosi(extId, pdata);
		break;
	case CAN_SUB_CMD_JOGMOVE :
		MAL_Protocol_Ani_Ctr_SetAcCount(extId, pdata);
		break;
	case SUB_CMD_MOTOR_ENABLE :
		MAL_Protocol_Ani_Ctr_SetAxleEnable(extId, pdata);
		break;

	case SUB_CMD_AC_NON_SEN :
		MAL_Protocol_Ani_Ctr_SetSensorInit_Abso(extId, pdata);
		break;

	}

}

void MAL_Protocol_Ani_Request(MAL_CAN_Protocol_Ani_ExtIdTypedef *extId, uint8_t *pdata)
{
	switch (extId->sub_cmd)
	{
	case SUB_CMD_RQS_SENSOR_STATUS_ALL_REQ:
		MAL_Protocol_Ani_SensorInitStatus_AllReq();
		break;

	case SUB_CMD_RQS_VER:
		MAL_Protocol_Ani_FwVerReq();
		break;
	case SUB_CMD_BATTERY:
		MAL_Protocol_Ani_AcAbsoBatteryReq(extId,pdata);
		break;
	}
}

static void MAL_Protocol_Ani_Parsing(CAN_RxHeaderTypeDef *header,uint8_t *pdata)
{
	if(header->IDE != CAN_ID_EXT)
		return;

	MAL_CAN_Protocol_Ani_ExtIdTypedef extId;
	//MAL_CAN_Protocol_Ani_DataHeader *pDataHeader = (MAL_CAN_Protocol_Ani_DataHeader *) pdata;

	extId.can_header_32 = header->ExtId;

	if((extId.tar_id == MAL_Board_ID_GetValue()) || (extId.tar_id == 0xff))
	{
		MAL_CAN_LED_RX_On(motorManager.mcan);
		switch(extId.Case)
		{
		case CAN_CASE_CONTROL:
			MAL_Protocol_Ani_Control(&extId, pdata);
			break;
		case CAN_CASE_REQUEST:
			MAL_Protocol_Ani_Request(&extId, pdata);
			break;
		case CAN_CASE_RESPONE:
			break;
		}
	}
}


void MAL_Protocol_Ani_GetPacket(void)
{

	CAN_RxHeaderTypeDef rxHeader = { 0, };
	uint8_t data[8] = { 0, };

	if (MAL_CAN_GetQueuePacket(motorManager.mcan,&rxHeader, data) == MAL_CONTAIN) {
		MAL_Protocol_Ani_Parsing(&rxHeader, data);
	}

}

void MAL_Protocol_Ani_Process(void)
{
	MAL_Protocol_Ani_GetPacket();
}




#endif





