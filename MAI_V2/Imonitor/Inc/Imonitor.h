/*
 * Imonitor.h
 *
 *  Created on: May 3, 2021
 *      Author: shin
 */

#ifndef IMONITOR_INC_IMONITOR_H_
#define IMONITOR_INC_IMONITOR_H_

#pragma pack(1)
//=============================================================================
// CMD
#define I_CMD_BOARD_INFO	0	//보드정보
#define I_CMD_CAN_TEST		1	//can통신검사
#define I_CMD_EXT_COMM		2	//외부통신검사  0:정상, 1:비정상
#define I_CMD_SEN_TEST		3	//센서검사
#define I_CMD_ID_TEST		4	//아이디검사

#define I_CMD_ABSO_TEST		6	//엡소카운트유지검사


//=============================================================================
//-------------------------------------------------------------------
//boardInfo
#define I_DATA_NONE	0
#define I_DATA_MAI	1
#define I_DATA_MBI	2
//-------------------------------------------------------------------
//t1_ can test
#define T1_CAN_TEST_CNT 1000

//=============================================================================


//=============================================================================
typedef struct{
	uint8_t cmd;
	uint8_t payload[0];
}IheadPacket_TypeDef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t boardType;
}boardInfo_Packet_TypeDef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t temp;
}t1_1_Packet_TypeDef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t errorFlag;	//0:정상, 1:비정상
}t1_2_Packet_TypeDef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t testNum;
	uint8_t testVal;	//0:정상, 1:비정상
}t2_Packet_TypeDef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t id;
}t3_Packet_TypeDef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t btNum;
	int32_t absoVal;
}t6_Packet_TypeDef;
//=============================================================================



//=============================================================================
//검수 데이터 구조체
typedef struct{
	uint8_t f_new;
	uint8_t boardType;
}boardInfo_Typedef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t f_new;
	uint8_t f_test;
	uint32_t txCnt;
	uint32_t rxCnt;

	float sucR;
}v1_Typedef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t f_new;
	uint8_t f_test;

	uint8_t extRxstatus;
	uint8_t *extComm;
}v1_2_Typedef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t f_new[4];
	uint8_t f_test;
	uint8_t f_status[4];

	uint8_t *cw_sen;
	uint8_t *ccw_sen;
}v2_Typedef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t f_new[16];
	uint8_t f_test;
	uint8_t f_id[16];

	uint16_t tot;
}v3_Typedef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t f_new[3];
	uint8_t f_test;
	uint8_t f_status[3];
}v4_Typedef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t f_new[2];
	uint8_t f_test;
	uint8_t f_status[2];
}v5_Typedef;
//-------------------------------------------------------------------
typedef struct{
	uint8_t f_new[2];
	uint8_t f_test;
	int32_t f_absoVal[2];

	int32_t *absoVal;
}v6_Typedef;
//-------------------------------------------------------------------

typedef struct{
	uint8_t f_rx;
	uint8_t f_tx;

	boardInfo_Typedef	v0_main;
	v1_Typedef			v1_comm;
	v1_2_Typedef		v1_2extComm;
	v2_Typedef			v2_sen;
	v3_Typedef			v3_id;
	v4_Typedef			v4_led;
	v5_Typedef			v5_posi;
	v6_Typedef			v6_abso;
}Imonitor_TypeDef;
//=============================================================================
#pragma pack()

#ifdef IMONITOR_MASTER
#ifdef __cplusplus
extern "C" {
#endif
void Imonitor_polling(uint8_t pollNum);
void Imonitor_canTest(void);
void Imonitor_extCommTest(void);
void Imonitor_t2_test(uint8_t testNum);
void Imonitor_t3_test(void);
void Imonitor_v5_1_setPosi(uint32_t posiVal);
void Imonitor_v6_getAbso(uint8_t btNum);
#ifdef __cplusplus
}
#endif
#endif
#endif /* IMONITOR_INC_IMONITOR_H_ */

