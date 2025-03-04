/*
 * ProtocolConnect.h
 *
 *  Created on: Nov 7, 2024
 *      Author: minim
 */

#ifndef PROTOCOLCONNECT_PROTOCOLCONNECT_H_
#define PROTOCOLCONNECT_PROTOCOLCONNECT_H_

#include "main.h"

/* Board Tx*/
void ProtocolConnect_sensor_detect_ctl(uint8_t axleId, uint8_t cwSen, uint8_t ccwSen);
void ProtocolConnect_RspAcAbsoBatteryOk(uint8_t axleId);
void ProtocolConnect_RspSensorInitSuccess(uint8_t axleId,int64_t absoCnt);
void ProtocolConnect_RspDefPosi(uint8_t axleId, uint8_t initFlag);
void ProtocolConnect_EventSensorDetect(uint8_t axleId, uint16_t value);


#endif /* PROTOCOLCONNECT_PROTOCOLCONNECT_H_ */
