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


#if !defined(ESP32) && !defined(ARDUINO_RASPBERRY_PI_PICO)
#error Platform is not supported
#endif

class IRReceiverRC5 : public IRReceiver
{
	public:
	
	IRReceiverRC5 (uint16_t receivePin)
  : IRReceiver(receivePin) 
  {
	}
  
  virtual void decode(MobaLedLib_C& mobaLedLib) 
  {
    if (IrReceiver.decodedIRData.protocol==decode_type_t::RC5)
    {
      doSetOutput(3000+IrReceiver.decodedIRData.command, 1);
    }
#ifdef MLL_IRRECEIVE_DEBUG
    else
    {
      Serial.println("data skipped, expecting RC5");
    }
#endif
  }	
};
