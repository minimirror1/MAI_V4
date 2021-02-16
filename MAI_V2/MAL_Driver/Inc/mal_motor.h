/*
 * mal_motor.h
 *
 *  Created on: Mar 11, 2020
 *      Author: shin
 */

#ifndef INC_MAL_MOTOR_H_
#define INC_MAL_MOTOR_H_

#include "mal_uart.h"
#include "mal_can.h"
#include "main.h"


#ifdef HAL_CAN_MODULE_ENABLED




typedef enum __MAL_MOTOR_RotationTypeDef{
	MAL_RO_CW = 0,
	MAL_RO_CCW,
	MAL_RO_STOP
}MAL_MOTOR_RotationTypeDef;

typedef enum __MAL_MOTOR_SensorInit_TypeDef{
	MAL_SEN_DEINIT = 0,
	MAL_SEN_INIT_ING,
	MAL_SEN_INIT_OK,
	MAL_SEN_DEF_LOCATION,
	MAL_SEN_INIT_INGWAIT,
	MAL_SEN_EMERGENCY_STOP,
	MAL_SEN_OFFSETMOVE
}MAL_MOTOR_SensorInit_TypeDef;


typedef enum __MAL_MOTOR_TypeTypeDef{
	MAL_MOTOR_BLDC_MD,
	MAL_MOTOR_AC_PANASONIC

}MAL_MOTOR_TypeTypeDef;



typedef struct __MAL_MOTOR_LocationTypeDef
{
	int32_t cwLimit;
	int32_t ccwLimit;
	int32_t motor;
}MAL_MOTOR_LocationTypeDef;


typedef struct __MAL_MOTOR_HandleTypeDef
{
	uint32_t *ctrHandle; //컨르롤 헨들러 muart,mcan,mpulse

	MAL_MOTOR_LocationTypeDef Location;

	uint8_t axleActive;	//축 활성 플래그 //0: 비활성 , 1: 활성

	/*call back*/
	//set
	void (*mal_motor_setLocationCallBack)(uint32_t *ctrHandle, uint16_t location);
	void (*mal_motor_setBldcInfoCallBack)(uint32_t *ctrHandle, uint16_t count, uint16_t rpm);
	void (*mal_motor_setSettingCallBack)(uint32_t *ctrHandle, uint8_t SensorDirection,uint16_t OppositeLimit,uint16_t DefaultLocation,uint8_t ReductionRatio);
	uint8_t (*mal_motor_setSetting_AbsoluteCallBack)(uint32_t *ctrHandle, uint8_t SensorDirection,uint16_t OppositeLimit,uint16_t DefaultLocation,uint8_t ReductionRatio);
	void (*mal_motor_setDefaultLocationCallBack)(uint32_t *ctrHandle);
	void (*mal_motor_setJogCounter)(uint32_t *ctrHandle, int16_t counter);

	void (*mal_motor_setLoadAbsoCnt)(uint32_t *ctrHandle, uint32_t counter);//20201103

	//get
	uint8_t (*mal_motor_getSettingFlagCallBack)(uint32_t *ctrHandle);
	uint8_t (*mal_motor_getAbsoStatusCallBack)(uint32_t *ctrHandle);
	uint8_t (*mal_motor_getAbsoStatusOkCallBack)(uint32_t *ctrHandle);
	uint32_t (*mal_motor_getAbsoCountOkCallBack)(uint32_t *ctrHandle);

}MAL_MOTOR_HandleTypeDef;


typedef struct __MAL_MOTOR_ManagerHandleTypeDef
{
	MAL_CAN_HandleTypeDef *mcan; //프로토콜 통신

	uint8_t senInitFlag[10];	//센서 초기화 데이터 수신 플래그
}MAL_MOTOR_ManagerHandleTypeDef;




extern void MAL_Motor_CallBackInit(MAL_MOTOR_HandleTypeDef *mmotor, uint32_t *ctrHandle, MAL_MOTOR_TypeTypeDef motorType );
extern void MAL_Motor_ManagerInit(uint32_t * commHandle);

/*===================================================================
 * 모터 제어
===================================================================*/
extern void MAL_Motor_SetAllLocation(uint16_t location);
extern void MAL_Motor_SetLocation(uint8_t axleId, uint16_t location);
extern void MAL_Motor_SetJogCounter(uint8_t axleId, int16_t counter);

extern void MAL_Motor_SetLoadAbsoCnt(uint8_t axleId, uint32_t loadAbsoCnt);//20201103

extern void MAL_Motor_SetBldcInfo(uint8_t axleId, uint16_t count, uint16_t rpm);
extern void MAL_Motor_SetSetting(
		uint8_t axleId,
		uint8_t SensorDirection,
		uint16_t OppositeLimit,
		uint16_t DefaultLocation,
		uint8_t ReductionRatio);
extern void MAL_Motor_SetDefaultLocation(uint8_t axleId);
//get
extern uint8_t MAL_Motor_GetSettingFlag(uint8_t axleId);
extern uint8_t MAL_Motor_GetAxleNum(void);

extern uint8_t MAL_Motor_GetAbsoStatus(uint8_t axleId);//20201103
extern uint8_t MAL_Motor_GetAbsoStatusOk(uint8_t axleId);
extern uint32_t MAL_Motor_GetAbsoCountOk(uint8_t axleId);
#endif

#endif /* INC_MAL_MOTOR_H_ */
