#include <stdint.h>
#include "grrlib.h"
#include "ogc/lwp_watchdog.h"
#include "pollTest.h"
#include <string>
#include "../globals/fonts.h"
#include "../helpers/controller.h"
#include "../config/config.h"
#include "ogc/pad.h"
/*Enum declaring packet types
 *
 */
enum PACKET_TYPE{
	INVALID_PACKET,
	METADATA_PACKET,
	ACK_PACKET,
	DATA_PACKET,
};

#define getType(x) ((x&0b10)>>1)
#define getShort(x,y) ((x<<8)|y)
#define getInt(x,y,z,a) ((x<<24) | (y<<16) | (z<<8) | a)
#define getLatch(x) (x&0b1)

uint16_t simpleChecksum(const uint8_t *data, uint16_t len);
uint8_t dataBuffer[1200];
uint16_t dataRead = 0;
uint16_t dataLength = 50;

int getPacketType(uint8_t* packet){
	if(simpleChecksum(packet,4) == getShort(packet[4],packet[5])){
		int type = (packet[0] & 0b10) >> 1;
		if(type == 0){
			return METADATA_PACKET;
		}
		else if(type == 1){
			return DATA_PACKET;
		}
	}
	return INVALID_PACKET;
}

void getPacket(uint8_t* buffer){
	buffer[0] = PAD_StickX(0) + 127;
	buffer[1] = PAD_StickY(0) + 127;
	buffer[2] = (uint8_t)PAD_SubStickX(0) + 127;
	buffer[3] = (uint8_t)PAD_SubStickY(0) + 127;
	buffer[4] = (uint8_t)PAD_TriggerL(0);
	buffer[5] = (uint8_t)PAD_TriggerR(0);
	return;
}
uint16_t simpleChecksum(const uint8_t *data, uint16_t len){
    uint8_t checksumTemp = 0;
    uint16_t calculatedChecksum = 0;
    for(int i = 0; i < len; i++){
        checksumTemp ^= data[i];
    }
    calculatedChecksum =  (checksumTemp << 8);
    calculatedChecksum = calculatedChecksum | ((~checksumTemp)&0xff);
    
    return calculatedChecksum;
}

void toggleMotor(bool a){
	if(a){
		PAD_ControlMotor(0,PAD_MOTOR_RUMBLE);
	}
	else{
		PAD_ControlMotor(0,PAD_MOTOR_STOP);
	}
}

void pollingCallback(uint32_t a,void* b){
	static bool inTransfer = false;
	static bool latch = false;
	uint8_t packetData[6];
	getPacket(packetData);
	int type = getPacketType(packetData);
	if(inTransfer==false){
		if(type == METADATA_PACKET){
			dataLength = getShort(packetData[1],packetData[2]);
			dataRead = 0;	
			inTransfer=true;
			latch = true;
			toggleMotor(latch);
		}
	}
	else if(inTransfer){
		bool latchRead = packetData[0]&0b1;
		if(type == DATA_PACKET && latch == latchRead){
			latch = !latch;
			for(int i = 0; i < 3; i++){
				dataBuffer[dataRead++] = packetData[1+i];
			}
			toggleMotor(latch);
		}
		if(dataRead >= dataLength){
			inTransfer = false;
		}
	}
	char buff[100];
	for(int i = 0; i < 6; i++){
		sprintf(buff,"%d",packetData[i]);
		GRRLIB_PrintfTTF (30,55+16*i, menuFont, buff,12, 0xFFFFFFFF);
	}
	uint16_t sum = simpleChecksum(packetData,4);
	sprintf(buff,"%d",sum >> 8);
	GRRLIB_PrintfTTF (30,170, menuFont, buff,12, 0xFFFFFFFF);
	sprintf(buff,"%d",sum&0xff);
	GRRLIB_PrintfTTF (30,190, menuFont, buff,12, 0xFFFFFFFF);
}







void runSerialReader(){

	//Clear origin
	uint8_t perfectOrigin[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	auto origin = getOrigins();
	memcpy(origin[0],perfectOrigin,8);

	//Set callback function
//	SI_RegisterPollingHandler(pollingCallback);
//	SI_SetSamplingRate(1);//Set to 1ms poll sped	

	while(1){
		memcpy(origin[0],perfectOrigin,8);
		PAD_ScanPads();
		updateController();
		GRRLIB_2dMode();
		pollingCallback(0,NULL);
		GRRLIB_PrintfTTF (60,55, menuFont, "Begin configuration dump on controller in first port",12, 0xFFFFFFFF);
		//Print recorded data
		for(int i = 0; i < dataLength; i++){
			char buffer[500];
			sprintf(buffer,"%d",dataBuffer[i]);
			int y = 80 + 12*i % 300;
			int x = 200 + ((int)((12*i)/300)) * 48;
			GRRLIB_PrintfTTF (x,y, menuFont, buffer,12, 0xFFFFFFFF);
		}

		if(exitPressed()){
			break;
		}
		GRRLIB_Render();
	}
	return;
}
