/*
 * Imonitor_master.c
 *
 *  Created on: May 3, 2021
 *      Author: shin
 */


#include "main.h"
#include "Imonitor.h"

#ifdef IMONITOR_MASTER
Imonitor_TypeDef imonitor = {0,};


void Imonitor_polling(uint8_t pollNum)
{
	uint8_t txBuff[8] = {0,};
	IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef *)txBuff;

	pTxPacket->cmd = pollNum;
	app_tx_inspection_sub_pid_data_ctl(0, 0, 0, 0xFF, 0, txBuff);

	imonitor.f_tx = SET;
}


//t1_1
void Imonitor_canTest(void)
{
	uint8_t txBuff[8] = {0,};
	IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef *)txBuff;

	pTxPacket->cmd = I_CMD_CAN_TEST;
	app_tx_inspection_sub_pid_data_ctl(0, 0, 0, 0xFF, 0, txBuff);

	imonitor.v1_comm.f_new = SET;
	imonitor.v1_comm.txCnt++;

	imonitor.f_tx = SET;
}
//t1_2
void Imonitor_extCommTest(void)
{
	uint8_t txBuff[8] = {0,};
	IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef *)txBuff;

	pTxPacket->cmd = I_CMD_EXT_COMM;
	app_tx_inspection_sub_pid_data_ctl(0, 0, 0, 0xFF, 0, txBuff);

	imonitor.f_tx = SET;
}
//t2
void Imonitor_t2_test(uint8_t testNum)
{
	uint8_t txBuff[8] = {0,};
	IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef *)txBuff;
	t2_Packet_TypeDef *data = (t2_Packet_TypeDef *)&pTxPacket->payload[0];

	pTxPacket->cmd = I_CMD_SEN_TEST;
	data->testNum = testNum;
	app_tx_inspection_sub_pid_data_ctl(0, 0, 0, 0xFF, 0, txBuff);

	imonitor.f_tx = SET;
}
//t3
void Imonitor_t3_test(void)
{
	uint8_t txBuff[8] = {0,};
	IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef *)txBuff;
	t3_Packet_TypeDef *data = (t3_Packet_TypeDef *)&pTxPacket->payload[0];

	pTxPacket->cmd = I_CMD_ID_TEST;
	app_tx_inspection_sub_pid_data_ctl(0, 0, 0, 0xFF, 0, txBuff);

	imonitor.f_tx = SET;
}

//t5_1
void Imonitor_v5_1_setPosi(uint32_t posiVal)
{
	app_tx_motion_sub_pid_adc_ctl(0, 0, 0, 0xFF, 0, posiVal);
	imonitor.f_tx = SET;
}

//t6
void Imonitor_v6_getAbso(uint8_t btNum)
{
	uint8_t txBuff[8] = {0,};
	IheadPacket_TypeDef *pTxPacket = (IheadPacket_TypeDef *)txBuff;
	t6_Packet_TypeDef *data = (t6_Packet_TypeDef *)&pTxPacket->payload[0];

	pTxPacket->cmd = I_CMD_ABSO_TEST;
	data->btNum = btNum;
	app_tx_inspection_sub_pid_data_ctl(0, 0, 0, 0xFF, 0, txBuff);

	imonitor.f_tx = SET;
}


//=======================================================================================
//rx func
void pars_inspection(uint8_t canNum, uint8_t tar_id, uint8_t *pData)
{
	IheadPacket_TypeDef *pRxPacket = (IheadPacket_TypeDef *)pData;


	switch(pRxPacket->cmd)
	{
		case I_CMD_BOARD_INFO:
		{
			boardInfo_Packet_TypeDef *data = (boardInfo_Packet_TypeDef *)&pRxPacket->payload[0];
			imonitor.v0_main.f_new = SET;
			imonitor.v0_main.boardType = data->boardType;

			imonitor.v1_comm.f_new = RESET;
			imonitor.v1_comm.rxCnt = 0;
			imonitor.v1_comm.txCnt = 0;
		}
		break;
		case I_CMD_CAN_TEST :
		{
			imonitor.v1_comm.f_new = SET;
			imonitor.v1_comm.rxCnt++;
		}
		break;
		case I_CMD_EXT_COMM:
		{
			t1_2_Packet_TypeDef *data = (t1_2_Packet_TypeDef *)&pRxPacket->payload[0];
			imonitor.v1_2extComm.f_new = SET;
			imonitor.v1_2extComm.extRxstatus = data->errorFlag;
		}
		break;
		case I_CMD_SEN_TEST:
		{
			t2_Packet_TypeDef *data = (t2_Packet_TypeDef *)&pRxPacket->payload[0];
			imonitor.v2_sen.f_new[data->testNum] = SET;
			imonitor.v2_sen.f_status[data->testNum] = data->testVal;
		}
		break;
		case I_CMD_ID_TEST:
		{
			t3_Packet_TypeDef *data = (t3_Packet_TypeDef *)&pRxPacket->payload[0];

			if(data->id <= 15)
			{
				imonitor.v3_id.f_new[data->id] = SET;
				imonitor.v3_id.f_id[data->id] = SET;
			}
		}
		break;
		case I_CMD_ABSO_TEST:
		{
			t6_Packet_TypeDef *data = (t6_Packet_TypeDef*) &pRxPacket->payload[0];

			if (data->btNum <= 1) {
				imonitor.v6_abso.f_new[data->btNum] = SET;
				imonitor.v6_abso.f_absoVal[data->btNum] = data->absoVal;
			}
		}
	}
}



//=======================================================================================
/*
void app_rx_inspection_sub_pid_data_ctl(uint8_t num, prtc_header_t *pPh, uint8_t *pData)
{
pars_inspection(num, pPh->souce_id, pData);
}
*/

void app_rx_inspection_sub_pid_data_rsp(uint8_t num, prtc_header_t *pPh, uint8_t *pData)
{
	pars_inspection(num, pPh->souce_id, pData);
	imonitor.f_rx = SET;
}

/*void app_rx_inspection_sub_pid_data_rqt(uint8_t num, prtc_header_t *pPh, uint8_t *pData)
{
	//pars_inspection(num, pPh->souce_id, pData);
}*/
//=======================================================================================

#endif
