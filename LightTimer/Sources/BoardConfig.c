
/*HEADER******************************************************************************************
*
* Comments:
*
*
**END********************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <MKL25Z4.h>
#include "BoardConfig.h"
#include "GPIO_Mux.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section                 
///////////////////////////////////////////////////////////////////////////////////////////////////



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




///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section                       
///////////////////////////////////////////////////////////////////////////////////////////////////

void BoardConfig_vfnInit(void)
{
#if TARGET_CLK == (8000000)
	/* SIM_SOPT2: PLLFLLSEL=1 */
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; /* Select PLL as a clock source for various peripherals */
	/* SIM_SOPT1: OSC32KSEL=3 */
	SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0x03); /* LPO 1kHz oscillator drives 32 kHz clock for various peripherals */

	/* Switch to FBE Mode */
	/* MCG_C2: LOCRE0=0,??=0,RANGE0=2,HGO0=0,EREFS0=1,LP=0,IRCS=0 */
	MCG_C2 = (MCG_C2_RANGE0(0x02) | MCG_C2_EREFS0_MASK);
	/* OSC0_CR: ERCLKEN=1,??=0,EREFSTEN=0,??=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
	OSC0_CR = OSC_CR_ERCLKEN_MASK;
	/* MCG_C1: CLKS=2,FRDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
	MCG_C1 = (MCG_C1_CLKS(0x02) | MCG_C1_FRDIV(0x03) | MCG_C1_IRCLKEN_MASK);
	/* MCG_C4: DMX32=0,DRST_DRS=0 */
	MCG_C4 &= (uint8_t)~(uint8_t)((MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x03)));
	/* MCG_C5: ??=0,PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=3 */
	MCG_C5 = MCG_C5_PRDIV0(0x03);
	/* MCG_C6: LOLIE0=0,PLLS=0,CME0=0,VDIV0=0 */
	MCG_C6 = MCG_C6_VDIV0(0x00);
	while((MCG_S & MCG_S_IREFST_MASK) != 0x00U) { /* Check that the source of the FLL reference clock is the external reference clock. */
	}
	while((MCG_S & 0x0CU) != 0x08U) {    /* Wait until external reference clock is selected as MCG output */
	}
	/* Switch to PBE Mode */
	/* MCG_C6: LOLIE0=0,PLLS=1,CME0=0,VDIV0=0 */
	MCG_C6 = (MCG_C6_PLLS_MASK | MCG_C6_VDIV0(0x00));
	while((MCG_S & 0x0CU) != 0x08U) {    /* Wait until external reference clock is selected as MCG output */
	}
	while((MCG_S & MCG_S_LOCK0_MASK) == 0x00U) { /* Wait until locked */
	}
#elif TARGET_CLK == (48000000)
	/* SIM_CLKDIV1: OUTDIV1=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,OUTDIV4=3,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
	  SIM_CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0x00) | SIM_CLKDIV1_OUTDIV4(0x01)); /* Set the system prescalers to safe value */
	  /* SIM_SCGC5: PORTA=1 */
	  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;   /* Enable clock gate for ports to enable pin routing */
	  if ((PMC_REGSC & PMC_REGSC_ACKISO_MASK) != 0x0U) {
	    /* PMC_REGSC: ACKISO=1 */
	    PMC_REGSC |= PMC_REGSC_ACKISO_MASK; /* Release IO pads after wakeup from VLLS mode. */
	  }
	  /* SIM_CLKDIV1: OUTDIV1=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,OUTDIV4=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
	  SIM_CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0x00) | SIM_CLKDIV1_OUTDIV4(0x01)); /* Update system prescalers */
	  /* SIM_SOPT2: PLLFLLSEL=1 */
	  SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; /* Select PLL as a clock source for various peripherals */
	  /* SIM_SOPT1: OSC32KSEL=3 */
	  SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0x03); /* LPO 1kHz oscillator drives 32 kHz clock for various peripherals */
	  /* SIM_SOPT2: TPMSRC=1 */
	  SIM_SOPT2 = (uint32_t)((SIM_SOPT2 & (uint32_t)~(uint32_t)(
	               SIM_SOPT2_TPMSRC(0x02)
	              )) | (uint32_t)(
	               SIM_SOPT2_TPMSRC(0x01)
	              ));                      /* Set the TPM clock */
	  /* PORTA_PCR18: ISF=0,MUX=0 */
	  PORTA_PCR18 &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
	  /* PORTA_PCR19: ISF=0,MUX=0 */
	  PORTA_PCR19 &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
	  /* Switch to FBE Mode */
	  /* MCG_C2: LOCRE0=0,??=0,RANGE0=2,HGO0=0,EREFS0=1,LP=0,IRCS=0 */
	  MCG_C2 = (MCG_C2_RANGE0(0x02) | MCG_C2_EREFS0_MASK);
	  /* OSC0_CR: ERCLKEN=1,??=0,EREFSTEN=0,??=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
	  OSC0_CR = OSC_CR_ERCLKEN_MASK;
	  /* MCG_C1: CLKS=2,FRDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
	  MCG_C1 = (MCG_C1_CLKS(0x02) | MCG_C1_FRDIV(0x03) | MCG_C1_IRCLKEN_MASK);
	  /* MCG_C4: DMX32=0,DRST_DRS=0 */
	  MCG_C4 &= (uint8_t)~(uint8_t)((MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x03)));
	  /* MCG_C5: ??=0,PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=3 */
	  MCG_C5 = MCG_C5_PRDIV0(0x03);
	  /* MCG_C6: LOLIE0=0,PLLS=0,CME0=0,VDIV0=0 */
	  MCG_C6 = MCG_C6_VDIV0(0x00);
	  while((MCG_S & MCG_S_IREFST_MASK) != 0x00U) { /* Check that the source of the FLL reference clock is the external reference clock. */
	  }
	  while((MCG_S & 0x0CU) != 0x08U) {    /* Wait until external reference clock is selected as MCG output */
	  }
	  /* Switch to PBE Mode */
	  /* MCG_C6: LOLIE0=0,PLLS=1,CME0=0,VDIV0=0 */
	  MCG_C6 = (MCG_C6_PLLS_MASK | MCG_C6_VDIV0(0x00));
	  while((MCG_S & 0x0CU) != 0x08U) {    /* Wait until external reference clock is selected as MCG output */
	  }
	  while((MCG_S & MCG_S_LOCK0_MASK) == 0x00U) { /* Wait until locked */
	  }
	  /* Switch to PEE Mode */
	  /* MCG_C1: CLKS=0,FRDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
	  MCG_C1 = (MCG_C1_CLKS(0x00) | MCG_C1_FRDIV(0x03) | MCG_C1_IRCLKEN_MASK);
	  while((MCG_S & 0x0CU) != 0x0CU) {    /* Wait until output of the PLL is selected */
	  }
	
#endif
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK;
	
	/* RGB Pins (Using TPMxCHx)*/
	/* RED	*/
	GPIO_vfnPinMux(GPIO_PORT_B,18,GPIO_MUX_ALT_1);
	/* GREEN */
	GPIO_vfnPinMux(GPIO_PORT_B,19,GPIO_MUX_ALT_1);
	/*	BLUE */
	GPIO_vfnPinMux(GPIO_PORT_D,1,GPIO_MUX_ALT_1);
	
	GPIOB_PDDR |= (1<<18)|(1<<19);
	
	GPIOD_PDDR |= (1<<1);
	
	/* I2C Pins set up */
	GPIO_vfnPinMux(GPIO_PORT_E,24,GPIO_MUX_ALT_5);
		
	GPIO_vfnPinMux(GPIO_PORT_E,25,GPIO_MUX_ALT_5);
	
	/* UART pins */
	/* RX */
	GPIO_vfnPinMux(GPIO_PORT_A,1,GPIO_MUX_ALT_2);
	/* TX */
	GPIO_vfnPinMux(GPIO_PORT_A,2,GPIO_MUX_ALT_2);
	
	GPIO_vfnPinMux(GPIO_PORT_B,1,GPIO_MUX_ALT_1);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////

