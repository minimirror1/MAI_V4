/*
 * mal_motor_acPana232Func.c
 *
 *  Created on: 2020. 10. 23.
 *      Author: shin
 */

#include "mal_motor_acPana232_v2.h"
#include "mal_motor_acPana232Func.h"
#include "mal_motor_acPanasonic.h"

//#include "eeprom.h"
//#include "eeprom_data.h"

//extern EEPemul_Data_TypeDef EepData;
MAL_MOTOR_ACPANA232_FuncTypeDef mAc232_Fnc = {0,};
extern MAL_MOTOR_PanasonicHandleTypeDef mpanasonic;

//=========================데이터 길이 반환======

uint8_t MAL_Motor_AcPanasonic_232_GetDataLen(MAL_MOTOR_ACPANA232_DataBundleTypeDef *pData)
{
	uint8_t ret = 0;

	switch (pData->cmd)
	{
	case 0://=========================================================

		break;//=======================================================

	case 1://=========================================================

		break;//=======================================================

	case 2://=========================================================
		switch(pData->mode)
		{
		case 1:
			ret = sizeof(MAL_MOTOR_ACPANA232_PacketC2M1TypeDef);
			break;

		case 0x0D:
			ret = sizeof(MAL_MOTOR_ACPANA232_PacketC2MdTypeDef);
			break;
		}

		break;//=======================================================

	case 9://=========================================================
		switch(pData->mode)
		{
		case 0x04:
			ret = sizeof(MAL_MOTOR_ACPANA232_PacketC9M4TypeDef);
			break;
		case 0x0B:
			ret = sizeof(MAL_MOTOR_ACPANA232_PacketC9MbTypeDef);
			break;
		}
		break;//=======================================================
	}

/*	if(pData->cmd == 0)
	{

	}
	else if(pData->cmd == 1)
	{

	}
	else if(pData->cmd == 2)
	{
		if(pData->mode == 1)
		{
			ret = sizeof(MAL_MOTOR_ACPANA232_PacketC2M1TypeDef);
		}
		else if(pData->mode == 0x0D)
		{
			ret = sizeof(MAL_MOTOR_ACPANA232_PacketC2MdTypeDef);
		}
	}
	else if(pData->cmd == 9)
	{
		if(pData->mode == 0x0B)
		{
			ret = 0;//데이터 없음, 길이 0
		}
	}*/
	return ret;
}

//==========================================

//=============



//=================================================================================================
//=============command 2
//=================================================================================================

//지령펄스 카운터 읽기 cmd : 2, mode : 1-------------------------------------------------------------
void MAL_Motor_AcPanasonic_232_GetPulseCounter_CallBack(uint8_t *data)
{
	MAL_MOTOR_ACPANA232_PacketDataHeaderTypeDef *header = (MAL_MOTOR_ACPANA232_PacketDataHeaderTypeDef*) data;
	MAL_MOTOR_ACPANA232_PacketC2M1TypeDef *pData = (MAL_MOTOR_ACPANA232_PacketC2M1TypeDef*) &header->payload[0];

	if(header->cmd != 2)
		return;

	if(header->mode != 1)
		return;

	if(pData->errorCode != 0)
		return;



	mAc232_Fnc.C2M1.initFlag = SET;
	mAc232_Fnc.C2M1.readCnt = pData->counter;

	if (mAc232_Fnc.C2M1.readFlag == RESET) {// 초기위치 성공 플래그, 초기위치 읽기 실패할경우 동작중에 읽지않기위함
		return;
	}

	if (mAc232_Fnc.C2M1.startCnt != mpanasonic.status.position.now) {
		return;
	}

	//모터 동기화


	if (mpanasonic.control.runStatus == MAL_PANASONIC_STOP) {
		if (mpanasonic.setting.flag == MAL_SEN_INIT_OK) {
			if (mpanasonic.status.position.now == mpanasonic.status.position.target) {
				mAc232_Fnc.C2M1.calcCnt = mAc232_Fnc.C2M1.readCnt - mpanasonic.setting.ampZeroCnt;
				//mAc232_Fnc.C2M1.calcCnt += mpanasonic.setting.AbsoOffsetCnt;
				mpanasonic.status.position.now = mpanasonic.status.position.target = mAc232_Fnc.C2M1.calcCnt;
			}
		}
	}

}
void MAL_Motor_AcPanasonic_232_GetPulseCounter(void)
{
	MAL_MOTOR_ACPANA232_DataBundleTypeDef tData;

	if (mpanasonic.control.runStatus == MAL_PANASONIC_STOP) {
		if (mpanasonic.status.position.now == mpanasonic.status.position.target) {

			tData.cmd = 2;
			tData.mode = 1;
			tData.mal_getDataProcess_CallBack = MAL_Motor_AcPanasonic_232_GetPulseCounter_CallBack;

			MAL_Motor_AcPanasonic_232_GetDataAddQueue(&tData);

			mAc232_Fnc.C2M1.startCnt = mpanasonic.status.position.now;
		}
	}
}
//---------------------------------------------------------------------------------------------

//앱솔루트 엔코더 읽기 cmd : 2, mode : D-------------------------------------------------------------
void MAL_Motor_AcPanasonic_232_GetAbsoluteCounter_CallBack(uint8_t *data)
{
	MAL_MOTOR_ACPANA232_PacketDataHeaderTypeDef *header = (MAL_MOTOR_ACPANA232_PacketDataHeaderTypeDef*) data;
	MAL_MOTOR_ACPANA232_PacketC2MdTypeDef *pData = (MAL_MOTOR_ACPANA232_PacketC2MdTypeDef*) &header->payload[0];

	if(header->cmd != 2)
		return;

	if(header->mode != 0x0D)
		return;

	if(pData->errorCode != 0)
		return;

	mAc232_Fnc.C2Md.ctrStatus = RESET;
	mAc232_Fnc.C2Md.endcoderId = pData->encoderId;
	mAc232_Fnc.C2Md.status = pData->status;
	mAc232_Fnc.C2Md.st1_cnt = pData->st1_cnt_1|(pData->st1_cnt_2<<8)|(pData->st1_cnt_3<<16);
	mAc232_Fnc.C2Md.multTurnCnt = pData->multTurnCnt;
	mAc232_Fnc.C2Md.errorCode = pData->errorCode;

	/*eep 테스트 코드*/
	mpanasonic.setting.absoReadOk = SET;



}
void MAL_Motor_AcPanasonic_232_GetAbsoluteCounter(void)
{
	MAL_MOTOR_ACPANA232_DataBundleTypeDef tData;

	tData.cmd = 2;
	tData.mode = 0x0D;
	tData.mal_getDataProcess_CallBack = MAL_Motor_AcPanasonic_232_GetAbsoluteCounter_CallBack;

	MAL_Motor_AcPanasonic_232_GetDataAddQueue(&tData);
}


//=================================================================================================
//=================================================================================================

//=================================================================================================
//=============command 9
//=================================================================================================
//앱소클리어 cmd : 9, mode : 4-------------------------------------------------------------
void MAL_Motor_AcPanasonic_232_SetAlmClear_CallBack(uint8_t *data)
{
	MAL_MOTOR_ACPANA232_PacketDataHeaderTypeDef *header = (MAL_MOTOR_ACPANA232_PacketDataHeaderTypeDef*) data;
	MAL_MOTOR_ACPANA232_PacketC9M4TypeDef *pData = (MAL_MOTOR_ACPANA232_PacketC9M4TypeDef*) &header->payload[0];

	if(header->cmd != 9)
		return;

	if(header->mode != 0x04)
		return;

	if(pData->errorCode != 0)
		return;

	mAc232_Fnc.C9M4.errorCode = pData->errorCode;

	if(mAc232_Fnc.C9M4.errorCode == 0)
	{
		mAc232_Fnc.C9M4.ctrStatus = RESET;
	}

}
void MAL_Motor_AcPanasonic_232_SetAlmClear(void)
{
	MAL_MOTOR_ACPANA232_DataBundleTypeDef tData;

	tData.cmd = 9;
	tData.mode = 4;
	tData.mal_getDataProcess_CallBack = MAL_Motor_AcPanasonic_232_SetAlmClear_CallBack;

	MAL_Motor_AcPanasonic_232_GetDataAddQueue(&tData);
}

//앱소클리어 cmd : 9, mode : B-------------------------------------------------------------
void MAL_Motor_AcPanasonic_232_SetAbsoluteClear_CallBack(uint8_t *data)
{
	MAL_MOTOR_ACPANA232_PacketDataHeaderTypeDef *header = (MAL_MOTOR_ACPANA232_PacketDataHeaderTypeDef*) data;
	MAL_MOTOR_ACPANA232_PacketC9MbTypeDef *pData = (MAL_MOTOR_ACPANA232_PacketC9MbTypeDef*) &header->payload[0];

	if(header->cmd != 9)
		return;

	if(header->mode != 0x0B)
		return;

	if(pData->errorCode != 0)
		return;

	mAc232_Fnc.C9Mb.errorCode = pData->errorCode;

	if(mAc232_Fnc.C9Mb.errorCode == 0)
	{
		mAc232_Fnc.C9Mb.ctrStatus = RESET;
	}

}
void MAL_Motor_AcPanasonic_232_SetAbsoluteClear(void)
{
	MAL_MOTOR_ACPANA232_DataBundleTypeDef tData;

	tData.cmd = 9;
	tData.mode = 0x0B;
	tData.mal_getDataProcess_CallBack = MAL_Motor_AcPanasonic_232_SetAbsoluteClear_CallBack;

	MAL_Motor_AcPanasonic_232_GetDataAddQueue(&tData);
}


//---------------------------------------------------------------------------------------------
//=================================================================================================
//=================================================================================================