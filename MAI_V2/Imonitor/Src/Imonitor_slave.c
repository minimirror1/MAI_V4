/*
 * Imonitor_slave.c
 *
 *  Created on: May 3, 2021
 *      Author: shin
 */

#include "main.h"
#include "Imonitor.h"

#ifdef IMONITOR_SLAVE_MAI
#include "mal_motor_acPana232_v2.h"
#include "mal_motor_acPanasonic.h"
#include "mal_board_info.h"
#include "mal_motor_acPanasonic.h"
extern MAL_MOTOR_PanasonicHandleTypeDef mpanasonic;
extern MAL_MOTOR_ACPANA232_PacketHandleTypeDef mAc232;
extern MAL_BOARD_InfoTypeDef mBoradInfo;
extern MAL_MOTOR_PanasonicHandleTypeDef mpanasonic;
#endif


#ifdef IMONITOR_SLAVE


Imonitor_TypeDef imonitor = {0,};

void Imonitor_init(void)//0:mai, 1:mbi
{
#ifdef IMONITOR_SLAVE_MAI
	imonitor.v0_main.boardType = I_DATA_MAI;		//보드 정보
	imonitor.v1_2extComm.extComm = &mAc232.f_error;	//연결에러 플래그 주소 0:정상, 1:비정상

	imonitor.v2_sen.cw_sen = (uint8_t *)&mpanasonic.cwSen->status.detect;
	imonitor.v2_sen.ccw_sen = (uint8_t *)&mpanasonic.ccwSen->status.detect;

	imonitor.v6_abso.absoVal = &mpanasonic.setting.absoCount;
#endif

}

//=======================================================================================
//rx func
void pars_inspection(uint8_t canNum, uint8_t tar_id, uint8_t *pData)
{
	IheadPacket_TypeDef *pRxPacket = (IheadPacket_TypeDef *)pData;
	uint8_t txBuff[8] = {0,};


	switch(pRxPacket->cmd)
	{
		case I_CMD_BOARD_INFO:
		{
			IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef *)&txBuff[0];
			boardInfo_Packet_TypeDef *data = (boardInfo_Packet_TypeDef *)&pTxPacket->payload[0];
			pTxPacket->cmd = I_CMD_BOARD_INFO;
			data->boardType = imonitor.v0_main.boardType;
		}
		break;
		case I_CMD_CAN_TEST:
		{
			IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef *)&txBuff[0];
			t1_1_Packet_TypeDef *data = (t1_1_Packet_TypeDef *)&pTxPacket->payload[0];
			pTxPacket->cmd = I_CMD_CAN_TEST;
		}
		break;
		case I_CMD_EXT_COMM:
		{
		IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef*) &txBuff[0];
		t1_2_Packet_TypeDef *data = (t1_2_Packet_TypeDef*) &pTxPacket->payload[0];
		pTxPacket->cmd = I_CMD_EXT_COMM;
		data->errorFlag = *imonitor.v1_2extComm.extComm;
		}
		break;
		case I_CMD_SEN_TEST:
		{
		IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef*) &txBuff[0];
		t2_Packet_TypeDef *data = (t2_Packet_TypeDef*) &pTxPacket->payload[0];

		t2_Packet_TypeDef *rxData = (t2_Packet_TypeDef *)&pRxPacket->payload[0];

		pTxPacket->cmd = I_CMD_SEN_TEST;
		data->testVal = 2;
		if (rxData->testNum == 0) {
			data->testNum = 0;
#ifdef IMONITOR_SLAVE_MAI

			if((*imonitor.v2_sen.cw_sen==RESET)&&(*imonitor.v2_sen.ccw_sen==RESET))
			{
				data->testVal = 1;
			}
#endif
		} else if (rxData->testNum == 1) {
			data->testNum = 1;
#ifdef IMONITOR_SLAVE_MAI

			if((*imonitor.v2_sen.cw_sen==RESET)&&(*imonitor.v2_sen.ccw_sen==RESET))
			{
				data->testVal = 1;
			}
#endif
		} else if (rxData->testNum == 2) {
			data->testNum = 2;
#ifdef IMONITOR_SLAVE_MAI
			if(*imonitor.v2_sen.cw_sen==SET)
			{
				data->testVal = 1;
			}
#endif
		} else if (rxData->testNum == 3) {
			data->testNum = 3;
#ifdef IMONITOR_SLAVE_MAI
			if(*imonitor.v2_sen.ccw_sen==SET)
			{
				data->testVal = 1;
			}
#endif
		}

		}
		break;
		case I_CMD_ID_TEST:
		{
		IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef*) &txBuff[0];
		t3_Packet_TypeDef *data = (t3_Packet_TypeDef*) &pTxPacket->payload[0];

		pTxPacket->cmd = I_CMD_ID_TEST;
#ifdef IMONITOR_SLAVE_MAI
		MAL_Board_ID_GetReloadValue();
		data->id = mBoradInfo.id.value;
#endif
		}
		break;
		case I_CMD_ABSO_TEST:
		{
		IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef*) &txBuff[0];
		t6_Packet_TypeDef *data = (t6_Packet_TypeDef*) &pTxPacket->payload[0];

		t6_Packet_TypeDef *rxData = (t6_Packet_TypeDef *)&pRxPacket->payload[0];


		pTxPacket->cmd = I_CMD_ABSO_TEST;
#ifdef IMONITOR_SLAVE_MAI
		data->btNum = rxData->btNum;
		data->absoVal = *imonitor.v6_abso.absoVal;
#endif
		}
		break;
	}

	//활성화 해야됨
	//app_tx_inspection_sub_pid_data_rsp(canNum, 0, my_can_id, tar_id, 0, txBuff);
}


//=======================================================================================
//241025 미사용
//void app_rx_inspection_sub_pid_data_ctl(uint8_t num, prtc_header_t *pPh, prtc_data_ctl_inspection_data_t *pData)
//{
//	pars_inspection(num, pPh->souce_id, pData);
//}
/*
void app_rx_inspection_sub_pid_data_rsp(uint8_t num, prtc_header_t *pPh, uint8_t *pData)
{

}

void app_rx_inspection_sub_pid_data_rqt(uint8_t num, prtc_header_t *pPh, uint8_t *pData)
{


}*/
//=======================================================================================

#endif

