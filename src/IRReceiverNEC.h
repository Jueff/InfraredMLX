#pragma once

/*
 MobaLedLib Infrared Extension for NEC remote controls
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 Copyright (C) 2022  Jürgen Winkler: MobaLedLib@gmx.at

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 -------------------------------------------------------------------------------------------------------------
*/


#if !defined(AVR) && !defined(ESP32)
#error Platform is not supported
#endif

#define EXCLUDE_EXOTIC_PROTOCOLS
#define EXCLUDE_UNIVERSAL_PROTOCOLS
#define DISABLE_LED_FEEDBACK_FOR_RECEIVE

#define DECODE_NEC     																				// turn on only NEC decoder


#include <IRremote.h>
#include <MLLExtension.h>

#ifdef ESP32
  #include "CommInterface.h"
#else
  void Update_InputCh_if_Addr_exists(uint16_t ReceivedAddr, uint8_t Direction, uint8_t OutputPower);
#endif

class IRReceiverNEC : public MLLExtension
{
	private:

	IRrecv* irrecv;
	
	public:
	
	IRReceiverNEC (uint16_t receivePin) {
		irrecv = new IRrecv(receivePin, false);
	}
	
	void setup(MobaLedLib_C& mobaLedLib) {
#ifdef MLL_IRRECEIVE_DEBUG
		Serial.print("IrReceiver created on pin "); Serial.println(irparams.IRReceivePin);	
#endif    
		irrecv->enableIRIn();  // Start the receiver
	}

	void loop(MobaLedLib_C& mobaLedLib) 
  {
    decode();
  }
  
    
  void decode()
  {
		if (irrecv->decode()) 
		{
			if (irrecv->decodedIRData.protocol==decode_type_t::NEC)
			{
#ifdef MLL_IRRECEIVE_DEBUG
        irrecv->printIRResultShort(&Serial);
#endif        

        bool repeated = false;
        if (irrecv->decodedIRData.flags & (IRDATA_FLAGS_IS_AUTO_REPEAT | IRDATA_FLAGS_IS_REPEAT))
        {
          int ticks = irrecv->decodedIRData.rawDataPtr->rawbuf[0]*MICROS_PER_TICK;
          repeated = ticks<250000;    // repeat within the last 250ms
        }
          
				if (irrecv->decodedIRData.address==0xEF00)	//NEC
				{
					if (irrecv->decodedIRData.command==0)	// brightness down
					{
						uint8_t step = repeated ? 5 : 1;			// if repeated command use bigger steps
						uint8_t br = FastLED.getBrightness();
						if (br>=step) 
						{
							FastLED.setBrightness(br-step);
						}
						else
						{
							FastLED.setBrightness(0);
						}
#ifdef MLL_IRRECEIVE_DEBUG
						Serial.print("Brightness set to ");
						Serial.println(FastLED.getBrightness());
#endif            
					}						
					else if (irrecv->decodedIRData.command==1)	// brightness up
					{
						uint8_t step = repeated ? 5 : 1;			// if repeated command use bigger steps
						uint8_t br = FastLED.getBrightness();
						if (br<=(255-step)) 
						{
							FastLED.setBrightness(br+step);
						}
						else
						{
							FastLED.setBrightness(255);
						}
#ifdef MLL_IRRECEIVE_DEBUG
						Serial.print("Brightness set to ");
						Serial.println(FastLED.getBrightness());
#endif            
					}
					else
					{
#ifdef ESP32
 						char s[20];
						sprintf(s, "@%4i %02X %02X\n", 3001+irrecv->decodedIRData.command, 0, 1);
						CommInterface::addToSendBuffer(s);			
#else            
  #ifdef USE_EXT_ADDR
						Update_InputCh_if_Addr_exists(3001+irrecv->decodedIRData.command, 0, 1);			
  #endif
#endif
					}
				}	
			}
      irrecv->resume(); // Enable receiving of the next value
		}
  }	
};
