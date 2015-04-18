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
#include "derivative.h"
#include "LightTimer_Protocol.h"
#include "SW_Timer.h"
#include "LightTimer_App.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////

#define LIGHTTIMER_APP_INITIAL_TIMER	(60000)

enum
{
	LIGHTTIMER_APP_OFF_CMD = 0,
	LIGHTTIMER_APP_ON_CMD,
	LIGHTTIMER_APP_SET_TIMER_CMD,
	LIGHTTIMER_APP_CANCEL_TIMER_CMD,
	LIGHTTIMER_APP_MAX_CMD
}eLightTimerCommands;
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section                 
///////////////////////////////////////////////////////////////////////////////////////////////////

#if LIGHTTIMER_MAX_OUTPUTS > 0
static void LightTimerApp_vfnOutput1Handler(void);
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 1
static void LightTimerApp_vfnOutput2Handler(void);
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 2		
static void LightTimerApp_vfnOutput3Handler(void);
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 3		
static void LightTimerApp_vfnOutput4Handler(void);
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 4		
static void LightTimerApp_vfnOutput5Handler(void);
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 5		
static void LightTimerApp_vfnOutput6Handler(void);
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 6		
static void LightTimerApp_vfnOutput7Handler(void);
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 7		
static void LightTimerApp_vfnOutput8Handler(void);
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 8		
static void LightTimerApp_vfnOutput9Handler(void);
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 9		
static void LightTimerApp_vfnOutput10Handler(void);
#endif

void (* const LightTimerApp_vfnapTimerCallback[])(void) = 
{
#if LIGHTTIMER_MAX_OUTPUTS > 0
		LightTimerApp_vfnOutput1Handler,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 1
		LightTimerApp_vfnOutput2Handler,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 2		
		LightTimerApp_vfnOutput3Handler,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 3		
		LightTimerApp_vfnOutput4Handler,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 4		
		LightTimerApp_vfnOutput5Handler,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 5		
		LightTimerApp_vfnOutput6Handler,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 6		
		LightTimerApp_vfnOutput7Handler,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 7		
		LightTimerApp_vfnOutput8Handler,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 8		
		LightTimerApp_vfnOutput9Handler,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 9		
		LightTimerApp_vfnOutput10Handler,
#endif
};


void LightTimerApp_vfnOnCommand(uint8_t bOutput, uint32_t dwTimer);

void LightTimerApp_vfnOffCommand(uint8_t bOutput, uint32_t dwTimer);

void LightTimerApp_vfnSetTimerCommand(uint8_t bOutput, uint32_t dwTimer);

void LightTimerApp_vfnCancelTimerCommand(uint8_t bOutput, uint32_t dwTimer);

void (* const LightTimerApp_vfnapCommands[LIGHTTIMER_APP_MAX_CMD])(uint8_t , uint32_t) =
{
		LightTimerApp_vfnOffCommand,
		LightTimerApp_vfnOnCommand,
		LightTimerApp_vfnSetTimerCommand,
		LightTimerApp_vfnCancelTimerCommand
};

void LightTimerApp_vfnCallback(LightTimerCommand_t*  tCommandToExecute);

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////
static const uint32_t LightTimer_dwaOutputPin[LIGHTTIMER_MAX_OUTPUTS] =
{
#if LIGHTTIMER_MAX_OUTPUTS > 0		
		(1<<LIGHTTIMER_OUTPUT_0_PIN),
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 1	
		(1<<LIGHTTIMER_OUTPUT_1_PIN),
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 2	
		(1<<LIGHTTIMER_OUTPUT_2_PIN),
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 3	
		(1<<LIGHTTIMER_OUTPUT_3_PIN),
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 4	
		(1<<LIGHTTIMER_OUTPUT_4_PIN),
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 5	
		(1<<LIGHTTIMER_OUTPUT_5_PIN),
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 6	
		(1<<LIGHTTIMER_OUTPUT_6_PIN),
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 7	
		(1<<LIGHTTIMER_OUTPUT_7_PIN),
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 8			
		(1<<LIGHTTIMER_OUTPUT_8_PIN),
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 9			
		(1<<LIGHTTIMER_OUTPUT_9_PIN),
#endif		
};

static volatile uint32_t * const LightTimer_baOutputPort[LIGHTTIMER_MAX_OUTPUTS] =
{
#if LIGHTTIMER_MAX_OUTPUTS > 0		
		&LIGHTTIMER_OUTPUT_0_PORT,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 1	
		&LIGHTTIMER_OUTPUT_1_PORT,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 2	
		&LIGHTTIMER_OUTPUT_2_PORT,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 3	
		&LIGHTTIMER_OUTPUT_3_PORT,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 4	
		&LIGHTTIMER_OUTPUT_4_PORT,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 5	
		&LIGHTTIMER_OUTPUT_5_PORT,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 6	
		&LIGHTTIMER_OUTPUT_6_PORT,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 7	
		&LIGHTTIMER_OUTPUT_7_PORT,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 8			
		&LIGHTTIMER_OUTPUT_8_PORT,
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 9			
		&LIGHTTIMER_OUTPUT_9_PORT,
#endif		
};
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////
static uint8_t LightTimer_baTimersHandlers[LIGHTTIMER_MAX_OUTPUTS];

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section                       
///////////////////////////////////////////////////////////////////////////////////////////////////

void LightTimer_vfnInit(void)
{
	uint8_t bCreateTimersCounter = 0;
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;	
	
	vfnLightTimerProtocol_vfnInit();
	
	while(bCreateTimersCounter < LIGHTTIMER_MAX_OUTPUTS)
	{
		LightTimer_baTimersHandlers[bCreateTimersCounter] = SWTimer_vfnAllocateChannel(LIGHTTIMER_APP_INITIAL_TIMER,LightTimerApp_vfnapTimerCallback[bCreateTimersCounter]);
		/* make sure to turn off all outputs first */
		pdwOutputPort = LightTimer_baOutputPort[bCreateTimersCounter];
		dwOutputMask = LightTimer_dwaOutputPin[bCreateTimersCounter];
		#if LIGHTTIMER_OUTPUT_ACTIVE_HIGH == 0
			*pdwOutputPort |= dwOutputMask;
		#elif LIGHTTIMER_OUTPUT_ACTIVE_HIGH == 1
			*pdwOutputPort &= ~dwOutputMask;
		#endif
		
		bCreateTimersCounter++;
	}
	
	
}

void LightTimer_vfnProtocolCallback(LightTimerCommand_t * ptCommandToExecute)
{
	LightTimerApp_vfnapCommands[ptCommandToExecute->bCommand](ptCommandToExecute->bOutput,ptCommandToExecute->dwTimer);
}

void LightTimerApp_vfnOnCommand(uint8_t bOutput, uint32_t dwTimer)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[bOutput];
	dwOutputMask = LightTimer_dwaOutputPin[bOutput];
#if LIGHTTIMER_OUTPUT_ACTIVE_HIGH == 1
	*pdwOutputPort |= dwOutputMask;
#elif LIGHTTIMER_OUTPUT_ACTIVE_HIGH == 0
	*pdwOutputPort &= ~dwOutputMask;
#endif
	
	(void)dwTimer;
}

void LightTimerApp_vfnOffCommand(uint8_t bOutput, uint32_t dwTimer)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[bOutput];
	dwOutputMask = LightTimer_dwaOutputPin[bOutput];
	
#if LIGHTTIMER_OUTPUT_ACTIVE_HIGH == 0
	*pdwOutputPort |= dwOutputMask;
#elif LIGHTTIMER_OUTPUT_ACTIVE_HIGH == 1
	*pdwOutputPort &= ~dwOutputMask;
#endif
	
	(void)dwTimer;
}

void LightTimerApp_vfnSetTimerCommand(uint8_t bOutput, uint32_t dwTimer)
{
	SWTimer_vfnUpdateCounter(LightTimer_baTimersHandlers[bOutput],dwTimer/SWTIMER_BASE_TIME);
	
	SWTimer_vfnEnableTimer(LightTimer_baTimersHandlers[bOutput]);
}

void LightTimerApp_vfnCancelTimerCommand(uint8_t bOutput, uint32_t dwTimer)
{
	SWTimer_vfnDisableTimer(LightTimer_baTimersHandlers[bOutput]);
	(void)dwTimer;
}


#if LIGHTTIMER_MAX_OUTPUTS > 0
static void LightTimerApp_vfnOutput1Handler(void)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[0];
	dwOutputMask = LightTimer_dwaOutputPin[0];
	
	*pdwOutputPort ^= dwOutputMask;
	
	SWTimer_vfnDisableTimer(LightTimer_baTimersHandlers[0]);
}
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 1
static void LightTimerApp_vfnOutput2Handler(void)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[1];
	dwOutputMask = LightTimer_dwaOutputPin[1];
	
	*pdwOutputPort ^= dwOutputMask;
	
	SWTimer_vfnDisableTimer(LightTimer_baTimersHandlers[1]);
}
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 2		
static void LightTimerApp_vfnOutput3Handler(void)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[2];
	dwOutputMask = LightTimer_dwaOutputPin[2];
	
	*pdwOutputPort ^= dwOutputMask;
	
	SWTimer_vfnDisableTimer(LightTimer_baTimersHandlers[2]);
}
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 3		
static void LightTimerApp_vfnOutput4Handler(void)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[3];
	dwOutputMask = LightTimer_dwaOutputPin[3];
	
	*pdwOutputPort ^= dwOutputMask;
	
	SWTimer_vfnDisableTimer(LightTimer_baTimersHandlers[3]);	
}
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 4		
static void LightTimerApp_vfnOutput5Handler(void)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[4];
	dwOutputMask = LightTimer_dwaOutputPin[4];
	
	*pdwOutputPort ^= dwOutputMask;
	
	SWTimer_vfnDisableTimer(LightTimer_baTimersHandlers[4]);	
}
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 5		
static void LightTimerApp_vfnOutput6Handler(void)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[5];
	dwOutputMask = LightTimer_dwaOutputPin[5];
	
	*pdwOutputPort ^= dwOutputMask;
	
	SWTimer_vfnDisableTimer(LightTimer_baTimersHandlers[5]);	
}
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 6		
static void LightTimerApp_vfnOutput7Handler(void)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[6];
	dwOutputMask = LightTimer_dwaOutputPin[6];
	
	*pdwOutputPort ^= dwOutputMask;
	
	SWTimer_vfnDisableTimer(LightTimer_baTimersHandlers[6]);	
}
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 7		
static void LightTimerApp_vfnOutput8Handler(void)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[7];
	dwOutputMask = LightTimer_dwaOutputPin[7];
	
	*pdwOutputPort ^= dwOutputMask;
	
	SWTimer_vfnDisableTimer(LightTimer_baTimersHandlers[7]);	
}
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 8		
static void LightTimerApp_vfnOutput9Handler(void)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[8];
	dwOutputMask = LightTimer_dwaOutputPin[8];
	
	*pdwOutputPort ^= dwOutputMask;
	
	SWTimer_vfnDisableTimer(LightTimer_baTimersHandlers[8]);	
}
#endif		
#if LIGHTTIMER_MAX_OUTPUTS > 9		
static void LightTimerApp_vfnOutput10Handler(void)
{
	volatile uint32_t * pdwOutputPort;
	uint32_t dwOutputMask;
	
	pdwOutputPort = LightTimer_baOutputPort[9];
	dwOutputMask = LightTimer_dwaOutputPin[9];
	
	*pdwOutputPort ^= dwOutputMask;
	
	SWTimer_vfnDisableTimer(LightTimer_baTimersHandlers[9]);	
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
