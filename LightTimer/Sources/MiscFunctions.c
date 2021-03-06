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
#include "MiscFunctions.h"
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

uint8_t bfnStringCompare(const uint8_t * bpStringBase, const uint8_t * bpStringToCompare, uint16_t wAmountOfCharacters)
{
	uint8_t bStatus = STRING_OK;
	uint8_t bStringOffset = 0;
	
	while(wAmountOfCharacters--)
	{
		/* Compare each character, early exit just after a character mismatch */
		if(bpStringBase[bStringOffset] != bpStringToCompare[bStringOffset])
		{
			bStatus = STRING_ERROR;
			wAmountOfCharacters = 0;
		}
		bStringOffset++;
	}
	
	return (bStatus);
}

uint8_t bfnAsciiToHex(uint8_t bAsciiData)
{
	uint8_t bHexData = 0;
	
	if ((bAsciiData >= '0') && (bAsciiData <= '9'))
	{
		bHexData = (bAsciiData - '0');
	}
	else
	{
		if((bAsciiData >= 'A') && (bAsciiData <= 'F'))
		{
			bAsciiData &= ~0x20;
			bHexData=  (bAsciiData - 'A' + 10);
		}
		else
		{
			if((bAsciiData >= 'a') && (bAsciiData <= 'f'))
			{
				bAsciiData &= ~0x20;
				bHexData=  (bAsciiData - 'A' + 10);
			}
		}
	}
	
	return(bHexData);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
