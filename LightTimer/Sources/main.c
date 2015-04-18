/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "BoardConfig.h"
#include "SW_Timer.h"
#include "MiscFunctions.h"
#include "Bluetooth.h"
#include "LightTimer_App.h"
#include "LightTimer_Protocol.h"

int main(void)
{
	BoardConfig_vfnInit();
	SWTimer_vfnInit();
	Bluetooth_vfnInitModule();
	LightTimer_vfnInit();
	
	
	for(;;) 
	{	   
		SWTimer_vfnServiceTimers();
		Bluetooth_vfnStateMachine();
		vfnLightTimerProtocol_vfnProtocol();
	}
	
	return 0;
}

