/*HEADER******************************************************************************************
*
* Comments:
*
*
**END********************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "SW_Timer.h"
#include "Bluetooth.h"
#include "MiscFunctions.h"
#include "StateMachine.h"
#include "LightTimer_Protocol.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////

#define LIGHTTIMER_PROTOCOL_SIZE								(8)

#define LIGHTTIMER_PROTOCOL_SOF									(0xAA)

#define LIGHTTIMER_PROTOCOLL_SOF_OFFSET							(0x0)

#define LIGHTTIMER_PROTOCOL_CMD_OFFSET							(0x1)

#define LIGHTTIMER_PROTOCOL_OUTPUT_OFFSET						(0x2)

#define LIGHTTIMER_PROTOCOL_TIMER_BYTE_1_OFFSET					(0x3)

#define LIGHTTIMER_PROTOCOL_TIMER_BYTE_2_OFFSET					(0x4)

#define LIGHTTIMER_PROTOCOL_TIMER_BYTE_3_OFFSET					(0x5)

#define LIGHTTIMER_PROTOCOL_TIMER_BYTE_4_OFFSET					(0x6)

#define LIGHTTIMER_PROTOCOL_CS_OFFSET							(0x7)

#define LIGHTTIMER_PROTOCOL_CHECK_DEVICE_STATUS					(500)

#define LIGHTTIMER_PROTOCOL_SET_STATUS(x)						(LightTimerProtocol_bStatus |= x)

#define LIGHTTIMER_PROTOCOL_CHECK_STATUS(x)						(LightTimerProtocol_bStatus & x)

#define LIGHTTIMER_PROTOCOL_CLEAR_STATUS(x)						(LightTimerProtocol_bStatus &= ~x)
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	LIGHTTIMER_PROTOCOL_IDLE_STATE = 0,
	LIGHTTIMER_PROTOCOL_RECEIVE_DATA_STATE,
	LIGHTTIMER_PROTOCOL_PARSE_DATA_STATE,
	LIGHTTIMER_PROTOCOL_MAX_STATES
}eLightTimerStates;

enum
{
	LIGHTTIMER_PROTOCOL_DEVICE_DETECTED = 0,
}eLightTimerStatus;

enum
{
	LIGHTTIMER_PROTOCOL_DEVICE_DETECTED_MASK = (1<<LIGHTTIMER_PROTOCOL_DEVICE_DETECTED),
	
}eLightTimerStatusMask;
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section                 
///////////////////////////////////////////////////////////////////////////////////////////////////

static void LightTimerProtocol_vfnDeviceStatusCallback(void);
static void LightTimerProtocol_vfnIdleState(void);
static void LightTimerProtocol_vfnReceiveDataState(void);
static void LightTimerProtocol_vfnParseDataState(void);

static void (*LightTimerProtocol_vfnapStateMachine[LIGHTTIMER_PROTOCOL_MAX_STATES])(void) =
{
		LightTimerProtocol_vfnIdleState,
		LightTimerProtocol_vfnReceiveDataState,
		LightTimerProtocol_vfnParseDataState
};

extern void LIGHTTIMER_PROTOCOL_CALLBACK (LightTimerCommand_t * );
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////

static uint8_t LightTimerProtocol_gbProtocolBuffer[LIGHTTIMER_PROTOCOL_SIZE];

static uint8_t LightTimerProtocol_bBufferOffset = 0;

static state_machine_t LightTimerProtocol_statemachine;

static uint8_t LightTimerProtocol_DeviceStatusTimer = 0;

static uint8_t LightTimerProtocol_Checksum = 0;

static LightTimerCommand_t LightTimerProtocol_NewCommand;

static uint8_t LightTimerProtocol_bStatus = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section                       
///////////////////////////////////////////////////////////////////////////////////////////////////

void vfnLightTimerProtocol_vfnInit(void)
{
	LightTimerProtocol_DeviceStatusTimer = SWTimer_vfnAllocateChannel(LIGHTTIMER_PROTOCOL_CHECK_DEVICE_STATUS,LightTimerProtocol_vfnDeviceStatusCallback);
	
	SWTimer_vfnEnableTimer(LightTimerProtocol_DeviceStatusTimer);
	
	LightTimerProtocol_statemachine.bCurrentState = LIGHTTIMER_PROTOCOL_IDLE_STATE;
}

void vfnLightTimerProtocol_vfnProtocol(void)
{
	LightTimerProtocol_vfnapStateMachine[LightTimerProtocol_statemachine.bCurrentState]();
}

static void LightTimerProtocol_vfnIdleState(void)
{
	
}

static void LightTimerProtocol_vfnReceiveDataState(void)
{
	if(LightTimerProtocol_bBufferOffset<LIGHTTIMER_PROTOCOL_SIZE)
	{
		if(BLUETOOTH_CHECK_STATUS(BLUETOOTH_DATA_READY))
		{
			LightTimerProtocol_gbProtocolBuffer[LightTimerProtocol_bBufferOffset] = Bluetooth_bfnReadData();
			
			if(LightTimerProtocol_bBufferOffset != LIGHTTIMER_PROTOCOL_CS_OFFSET)
			{
				LightTimerProtocol_Checksum += LightTimerProtocol_gbProtocolBuffer[LightTimerProtocol_bBufferOffset]; 
			}
			
			LightTimerProtocol_bBufferOffset++;
		}
	}
	else
	{
		LightTimerProtocol_bBufferOffset = 0;
		
		LightTimerProtocol_statemachine.bCurrentState = LIGHTTIMER_PROTOCOL_PARSE_DATA_STATE;
	}
}

static void LightTimerProtocol_vfnParseDataState(void)
{
	
	LightTimerProtocol_Checksum = ~LightTimerProtocol_Checksum;
	
	if(LightTimerProtocol_Checksum == LightTimerProtocol_gbProtocolBuffer[LIGHTTIMER_PROTOCOL_CS_OFFSET])
	{
		
		LightTimerProtocol_NewCommand.bCommand = LightTimerProtocol_gbProtocolBuffer[LIGHTTIMER_PROTOCOL_CMD_OFFSET];
		LightTimerProtocol_NewCommand.bOutput = LightTimerProtocol_gbProtocolBuffer[LIGHTTIMER_PROTOCOL_OUTPUT_OFFSET];
		LightTimerProtocol_NewCommand.dwTimer = LightTimerProtocol_gbProtocolBuffer[LIGHTTIMER_PROTOCOL_TIMER_BYTE_1_OFFSET]<<24;
		LightTimerProtocol_NewCommand.dwTimer |= LightTimerProtocol_gbProtocolBuffer[LIGHTTIMER_PROTOCOL_TIMER_BYTE_2_OFFSET]<<16;
		LightTimerProtocol_NewCommand.dwTimer |= LightTimerProtocol_gbProtocolBuffer[LIGHTTIMER_PROTOCOL_TIMER_BYTE_3_OFFSET]<<8;
		LightTimerProtocol_NewCommand.dwTimer |= LightTimerProtocol_gbProtocolBuffer[LIGHTTIMER_PROTOCOL_TIMER_BYTE_4_OFFSET];
		
		LIGHTTIMER_PROTOCOL_CALLBACK(&LightTimerProtocol_NewCommand);
	}
	
	LightTimerProtocol_Checksum = 0;

	LightTimerProtocol_statemachine.bCurrentState = LIGHTTIMER_PROTOCOL_RECEIVE_DATA_STATE;
}

static void LightTimerProtocol_vfnDeviceStatusCallback(void)
{
	/* Confirm there's a BT device connected */
	if(BLUETOOTH_CHECK_STATUS(BLUETOOTH_CONNECTED))
	{
		if(!LIGHTTIMER_PROTOCOL_CHECK_STATUS(LIGHTTIMER_PROTOCOL_DEVICE_DETECTED_MASK))
		{
			LightTimerProtocol_statemachine.bCurrentState = LIGHTTIMER_PROTOCOL_RECEIVE_DATA_STATE;
			
			LIGHTTIMER_PROTOCOL_SET_STATUS(LIGHTTIMER_PROTOCOL_DEVICE_DETECTED_MASK);
		}
	}
	else
	{
		if(LIGHTTIMER_PROTOCOL_CHECK_STATUS(LIGHTTIMER_PROTOCOL_DEVICE_DETECTED_MASK))
		{
			LightTimerProtocol_statemachine.bCurrentState = LIGHTTIMER_PROTOCOL_IDLE_STATE;
			LightTimerProtocol_bBufferOffset = 0;
			LightTimerProtocol_Checksum = 0;
			LIGHTTIMER_PROTOCOL_CLEAR_STATUS(LIGHTTIMER_PROTOCOL_DEVICE_DETECTED_MASK);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
