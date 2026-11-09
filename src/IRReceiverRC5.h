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


#if !defined(AVR) && !defined(ESP32) && !defined(ARDUINO_RASPBERRY_PI_PICO)
#error Platform is not supported
#endif

#include <MLLExtension.h>

#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO)
  #include "CommInterface.h"
#else
  void Update_InputCh_if_Addr_exists(uint16_t ReceivedAddr, uint8_t Direction, uint8_t OutputPower);
#endif

class IRReceiverRC5 : public MLLExtension
{
	private:

	public:
	
	IRReceiverRC5 (uint16_t receivePin) {
    IrReceiver.begin(receivePin, false); // Start the receiver
	}
	
	void setup(MobaLedLib_C& mobaLedLib) {
	}

	void loop(MobaLedLib_C& mobaLedLib) 
  {
    decodeRC5();
  }
    
  void decodeRC5()
  {
		if (IrReceiver.decode()) 
		{
#ifdef MLL_IRRECEIVE_DEBUG
      IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
#endif        
			if (IrReceiver.decodedIRData.protocol==decode_type_t::RC5)
			{
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO)
        char s[20];
        sprintf(s, "@%4i %02X %02X\n", 3001+IrReceiver.decodedIRData.command, 0, 1);
        CommInterface::addToSendBuffer(s);			
#else            
  #ifdef USE_EXT_ADDR
				Update_InputCh_if_Addr_exists(3001+IrReceiver.decodedIRData.command, 0, 1);			
  #endif
#endif
			}
      IrReceiver.resume(); // Enable receiving of the next value
		}
  }	
};
