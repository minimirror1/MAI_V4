/*
 * mal_motor.c
 *
 *  Created on: Mar 11, 2020
 *      Author: shin
 */

#include "main.h"
#include "mal_board_info.h"
#include "mal_motor.h"
#include "mal_motor_acPanasonic.h"
#include "mal_motor_bldcMd.h"

#ifdef HAL_MOTOR_MODULE_ENABLED
extern MAL_MOTOR_HandleTypeDef mmotor[MOTOR_AXLE_CNT];

MAL_MOTOR_ManagerHandleTypeDef motorManager;

void MAL_Motor_CallBackInit(MAL_MOTOR_HandleTypeDef *mmotor, uint32_t *ctrHandle, MAL_MOTOR_TypeTypeDef motorType);

/*===================================================================
 * 모터 초기화
 ===================================================================*/
#if 0  //init.c 양식
void MAL_Motor_Init(void)
{
	//모터 구조체 초기화
	//MAL_Motor_CallBackInit(&mmotor[0], MAL_MOTOR_AC_PANASONIC );

	MAL_Motor_CallBackInit(&mmotor[0], (uint32_t *)&muart4, MAL_MOTOR_BLDC_MD );

}
#endif

void MAL_Motor_CallBackInit(MAL_MOTOR_HandleTypeDef *mmotor, uint32_t *ctrHandle, MAL_MOTOR_TypeTypeDef motorType)
{
	mmotor->ctrHandle = ctrHandle;

	switch (motorType)
	{
		case MAL_MOTOR_BLDC_MD:
#ifdef HAL_MOTOR_MLDC_MD_MODULE_ENABLED
			mmotor->mal_motor_setLocationCallBack = MAL_Motor_BldcMd_SetLocation;
			mmotor->mal_motor_setBldcInfoCallBack = MAL_Motor_BldcMd_SetBldcInfo;
			mmotor->mal_motor_setSettingCallBack = MAL_Motor_BldcMd_SetSetting;
			mmotor->mal_motor_getSettingFlagCallBack = MAL_Motor_BldcMd_GetSettingFlag;
			mmotor->mal_motor_setDefaultLocationCallBack = MAL_Motor_BldcMd_SetDefaultLocation;

			mmotor->mal_motor_setJogCounter = MAL_Motor_BldcMd_SetJogCounter;

#endif
			break;
		case MAL_MOTOR_AC_PANASONIC:
#ifdef HAL_MOTOR_AC_MODULE_ENABLED
		mmotor->mal_motor_setLocationCallBack = MAL_Motor_AcPanasonic_SetLocation;
		mmotor->mal_motor_setBldcInfoCallBack = MAL_Motor_AcPanasonic_NullFunction;//빈함수
		mmotor->mal_motor_setSettingCallBack = MAL_Motor_AcPanasonic_SetSetting;
		mmotor->mal_motor_setSetting_AbsoluteCallBack = MAL_Motor_AcPanasonic_SetSetting_Absolute;
		mmotor->mal_motor_getSettingFlagCallBack = MAL_Motor_AcPanasonic_GetSettingFlag;
		mmotor->mal_motor_setDefaultLocationCallBack = MAL_Motor_AcPanasonic_SetDefaultLocation;
		mmotor->mal_motor_setJogCounter = MAL_Motor_AcPanasonic_SetCounter;

		mmotor->mal_motor_setLoadAbsoCnt = MAL_Motor_AcPanasonic_SetLoadAbsoCnt;//20201103
		mmotor->mal_motor_getAbsoStatusCallBack = MAL_Motor_AcPanasonic_GetAbsoStatus;//20201103
		mmotor->mal_motor_getAbsoStatusOkCallBack = MAL_Motor_AcPanasonic_GetAbsoStatusOk;//
		mmotor->mal_motor_getAbsoCountOkCallBack = MAL_Motor_AcPanasonic_GetAbsoCountOk;
#endif
			break;
	}
}

void MAL_Motor_ManagerInit(uint32_t *commHandle)
{
	motorManager.mcan = (MAL_CAN_HandleTypeDef*) commHandle;
}

/*===================================================================
 * 모터 제어
 ===================================================================*/

void MAL_Motor_SetAllLocation(uint16_t location)
{
	uint8_t i = 0;

	for (i = 0; i < MOTOR_AXLE_CNT; i++)
	{
		mmotor[i].mal_motor_setLocationCallBack(mmotor[i].ctrHandle, location);
	}
}
void MAL_Motor_SetLocation(uint8_t axleId, uint16_t location)
{
	if (MOTOR_AXLE_CNT <= axleId) return;
	//xxx 변경해야함
	mmotor[axleId].mal_motor_setLocationCallBack(mmotor[axleId].ctrHandle, location);
}

void MAL_Motor_SetBldcInfo(uint8_t axleId, uint16_t count, uint16_t rpm)
{
	if (MOTOR_AXLE_CNT <= axleId) return;
	mmotor[axleId].mal_motor_setBldcInfoCallBack(mmotor[axleId].ctrHandle, count, rpm);

	motorManager.senInitFlag[axleId] = SET;
}

void MAL_Motor_SetSetting(uint8_t axleId, uint8_t SensorDirection, uint16_t OppositeLimit, uint16_t DefaultLocation, uint8_t ReductionRatio)
{
	if (MOTOR_AXLE_CNT <= axleId) return;
	mmotor[axleId].mal_motor_setSettingCallBack(mmotor[axleId].ctrHandle, SensorDirection, OppositeLimit, DefaultLocation, ReductionRatio);

	motorManager.senInitFlag[axleId] = SET;
}

uint8_t MAL_Motor_SetSetting_Absolute(uint8_t axleId, uint8_t SensorDirection, uint16_t OppositeLimit, uint16_t DefaultLocation, uint8_t ReductionRatio)
{
	if (MOTOR_AXLE_CNT <= axleId) return 0;
	return mmotor->mal_motor_setSetting_AbsoluteCallBack(mmotor[axleId].ctrHandle, SensorDirection, OppositeLimit, DefaultLocation, ReductionRatio);

}

void MAL_Motor_SetDefaultLocation(uint8_t axleId)
{
	if (MOTOR_AXLE_CNT <= axleId) return;
	mmotor[axleId].mal_motor_setDefaultLocationCallBack(mmotor[axleId].ctrHandle);
}
void MAL_Motor_SetJogCounter(uint8_t axleId, int16_t counter)
{
	if (MOTOR_AXLE_CNT <= axleId) return;
	mmotor[axleId].mal_motor_setJogCounter(mmotor[axleId].ctrHandle, counter);
}

//if(mmotor[axleId].axleActive !=

void MAL_Motor_SetAxleEnable(uint8_t axleId, uint8_t flag)
{
	if (MOTOR_AXLE_CNT <= axleId) return;

	mmotor[axleId].axleActive = flag;
}

//20201103
void MAL_Motor_SetLoadAbsoCnt(uint8_t axleId, uint32_t loadAbsoCnt)
{
	if (MOTOR_AXLE_CNT <= axleId) return;
		mmotor[axleId].mal_motor_setLoadAbsoCnt(mmotor[axleId].ctrHandle, loadAbsoCnt);
}
/*===================================================================
 * 모터 상태
 ===================================================================*/
uint8_t MAL_Motor_GetSettingFlag(uint8_t axleId)
{
	if (MOTOR_AXLE_CNT <= axleId) return 0;
	uint8_t ret = mmotor[axleId].mal_motor_getSettingFlagCallBack(mmotor[axleId].ctrHandle);
	return ret;
}

uint8_t MAL_Motor_GetAxleNum(void)
{
	return MOTOR_AXLE_CNT;
}

uint8_t MAL_Motor_GetSensorInitRecvFlag(uint8_t axleId)
{
	return motorManager.senInitFlag[axleId];
}

//20201103
uint8_t MAL_Motor_GetAbsoStatus(uint8_t axleId)
{
	if (MOTOR_AXLE_CNT <= axleId) return 0;
	uint8_t ret = mmotor[axleId].mal_motor_getAbsoStatusCallBack(mmotor[axleId].ctrHandle);
	return ret;
}

uint8_t MAL_Motor_GetAbsoStatusOk(uint8_t axleId)
{
	if (MOTOR_AXLE_CNT <= axleId) return 0;
	uint8_t ret = mmotor[axleId].mal_motor_getAbsoStatusOkCallBack(mmotor[axleId].ctrHandle);
	return ret;
}

uint32_t MAL_Motor_GetAbsoCountOk(uint8_t axleId)
{
	if (MOTOR_AXLE_CNT <= axleId) return 0;
	uint32_t ret = mmotor[axleId].mal_motor_getAbsoCountOkCallBack(mmotor[axleId].ctrHandle);
	return ret;
}

// event
void MAL_Protocol_Ani_EventBootAlm(void)
{
	app_tx_init_sub_pid_boot_ctl(PRIORITY_HIGH, MAL_Board_ID_GetValue(), MASTER_CAN_ID, MOTOR_AXLE_CNT);
}

void MAL_Protocol_Ani_AlmSensorDetection(uint8_t axleId, uint8_t cwSen, uint8_t ccwSen)
{

}

void MAL_Protocol_Ani_RspAcAbsoBatteryOk(uint8_t axleId)
{

}

void MAL_Protocol_Ani_RspDefPosi(uint8_t axleId, uint8_t initFlag)
{

}

void MAL_Protocol_Ani_EventSensorDetect(uint8_t axleId, uint16_t value)
{

}
#endif

