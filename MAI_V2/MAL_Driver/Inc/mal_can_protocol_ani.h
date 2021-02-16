/*
 * mal_can_protocol_ani.h
 *
 *  Created on: 2020. 3. 11.
 *      Author: shin
 */

#ifndef INC_MAL_CAN_PROTOCOL_ANI_H_
#define INC_MAL_CAN_PROTOCOL_ANI_H_



#define CAN_MASTER_ID		0

#define CAN_EMERGENCY 		0
#define CAN_UNEMERGENCY 	1

#define CAN_CLASS_MOTOR 	0
#define CAN_CLASS_SENSOR 	1

#define CAN_PAGE_NUM_0		0

#define CAN_CASE_CONTROL 	0
#define CAN_CASE_REQUEST 	1
#define CAN_CASE_RESPONE 	2

#define CAN_PAGE_DEN		0
#define CAN_PAGE_EN			1


#define CAN_SUB_CMD_PWM 	1

#define CAN_SUB_CMD_JOGMOVE 	0x10

#define    CMD_SETTING    0x53 // S
#define    CMD_REQUEST    0x52 // R
#define    CMD_RESPONSE    0x45 // E
/*******************************************************************/
#define		SUB_CMD_SET_ADC          			0x01
#define		SUB_CMD_SET_BUTTON      			0x02
#define		SUB_CMD_SET_SD          			0x03
#define 	SUB_CMD_BLDC_INIT         			0x0F
#define	    SUB_CMD_SET_AC         		 		0x05

#define		SUB_CMD_BOOT						0x08
#define		SUB_CMD_SENSOR_INIT_SUCCESS			0x09
#define		SUB_CMD_SENSOR_DEINIT				0x0A
#define		SUB_CMD_SET_INITPOSI				0x0C		//기본위치로

#define		SUB_CMD_SET_REINIT_POSI				0x0D	//191130 동작중 센서 보정
#define		SUB_CMD_MOTOR_ENABLE				0x11	//200611 축 활성 프로토콜 추가
#define     SUB_CMD_SENSOR_DETECTION			0x12	//200611 동작중 센서알람
/*******************************************************************/
#define  	SUB_CMD_RQS_DIMMING       			0x01
#define		SUB_CMD_RQS_FADING       			0x02
#define    	SUB_CMD_RQS_SD          			0x03
#define  	SUB_CMD_RQS_AC          			0x05

#define		SUB_CMD_RQS_SENSOR_STATUS_ALL_REQ	0x0B
#define 	SUB_CMD_RQS_VER						0x0E

#define 	SUB_CMD_BATTERY          			0x14//20201003
#define 	SUB_CMD_AC_NON_SEN       			0x15//20201003


/*******************************************************************/
#define     SUB_CMD_RSP_DIMMING       			0x01
#define     SUB_CMD_RSP_FADING       			0x02
#define     SUB_CMD_RSP_SD          			0x03
#define     SUB_CMD_RSP_AC          			0x05

#define		SUB_CMD_RSP_SENSORINIT				0x0C

/*******************************************************************/

#define BOARD_BROADCAST_ID	0xFF
#define MOTOR_BROADCAST_ID	0xFF


//=======================================================================================



#pragma pack(4)
typedef struct __MAL_CAN_Protocol_Ani_ExtIdTypedef
{
	union{
		struct{
			uint32_t sub_cmd	: 7;
			uint32_t payload_en	: 1;
			uint32_t Case	 	: 2;
			uint32_t tar_id 	: 8;
			uint32_t page_num 	: 8;
			uint32_t Class 		: 2;
			uint32_t emergency 	: 1;
			uint32_t Null		: 3;
		};
		uint32_t can_header_32;
	};
}MAL_CAN_Protocol_Ani_ExtIdTypedef;
#pragma pack()


#pragma pack(1)
typedef struct __MAL_CAN_Protocol_Ani_DataHeader{
	uint8_t length;
	uint8_t src_id;
	uint8_t payload[0];
}MAL_CAN_Protocol_Ani_DataHeader;
#pragma pack()

//=============================================================================
//rsp
#pragma pack(1)
typedef struct __MAL_CAN_Protocol_Ani_RspSensorInitSuccess{
	uint8_t axleId;
	uint8_t flag;
	uint32_t absoCnt;
}MAL_CAN_Protocol_Ani_RspSensorInitSuccess;
#pragma pack()


typedef struct __MAL_CAN_Protocol_Ani_AlmSensorDetection{
	uint8_t axleId;
	uint8_t cwSen;
	uint8_t ccwSen;
}MAL_CAN_Protocol_Ani_AlmSensorDetection;
#pragma pack()




#pragma pack(1)
typedef struct __MAL_CAN_Protocol_Ani_RspSensorInit{
	uint8_t axleId;
	uint8_t flag;
	uint32_t loadAbsoCnt;
}MAL_CAN_Protocol_Ani_RspSensorInit;
#pragma pack()

#pragma pack(1)
typedef struct __MAL_CAN_Protocol_Ani_RspFwVer{
	uint8_t ver1;//Majer
	uint8_t ver2;//Minor
	uint8_t ver3;//Build
}MAL_CAN_Protocol_Ani_RspFwVer;
#pragma pack()

#pragma pack(1)
typedef struct __MAL_CAN_Protocol_Ani_RspAcAbsoBattery{
	uint8_t axleId;
	uint8_t status;
	uint32_t loadAbsoCnt;
}MAL_CAN_Protocol_Ani_RspAcAbsoBattery;
#pragma pack()


//=============================================================================

#pragma pack(1)
typedef struct __MAL_CAN_Protocol_Ani_Pwm{
	uint8_t axleId;
	uint16_t val;
}MAL_CAN_Protocol_Ani_Pwm;
#pragma pack()

#pragma pack(1)
typedef struct __MAL_CAN_Protocol_Ani_AxleEnable{
	uint8_t 	id :7;
	uint8_t 	enable :1;		//센서 방향
}MAL_CAN_Protocol_Ani_AxleEnable;
#pragma pack()

#pragma pack(1)
typedef struct __MAL_CAN_Protocol_Ani_AcSetCount{
	uint8_t axleId;
	int16_t val;
}MAL_CAN_Protocol_Ani_AcSetCount;
#pragma pack()

#pragma pack(1)
typedef struct __MAL_CAN_Protocol_Ani_AcInit{
	uint8_t 	id :7;
	uint8_t 	SensorDirection :1;		//센서 방향
   uint16_t   	OppositeLimit;			//반대 제한 각도
   uint16_t    	DefaultLocation;		//초기 위치
   uint8_t      ReductionRatio;			//감속기 비율
}MAL_CAN_Protocol_Ani_AcInit;
#pragma pack()

#pragma pack(1)
__packed typedef struct{
	uint8_t 	axleId;
   uint16_t   count;
   uint16_t   rpm;
}can_protocol_cmd_bldc_init_t;
#pragma pack()

extern void MAL_Protocol_Ani_Process(void);

//event
extern void MAL_Protocol_Ani_EventBootAlm(void);
extern void MAL_Protocol_Ani_EventSensorDetect(uint8_t axleId, uint16_t value);

//rsp
extern void MAL_Protocol_Ani_AlmSensorDetection(uint8_t axleId, uint8_t cwSen, uint8_t ccwSen);//200611
extern void MAL_Protocol_Ani_RspDefPosi(uint8_t axleId, uint8_t initFlag);
extern void MAL_Protocol_Ani_RspSensorInit(uint8_t axleId);
extern void MAL_Protocol_Ani_RspSensorInitSuccess(uint8_t axleId, uint32_t absoCnt);

extern void MAL_Protocol_Ani_RspAcAbsoBatteryOk(uint8_t axleId);

#endif /* INC_MAL_CAN_PROTOCOL_ANI_H_ */
