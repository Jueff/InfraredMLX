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

class IRReceiverNEC : public IRReceiver
{
	public:

	IRReceiverNEC (uint16_t receivePin, uint16_t address)
  : IRReceiver(receivePin, address) 
  {
	}
    
	IRReceiverNEC (uint16_t receivePin)
  : IRReceiver(receivePin, addressOffset) 
  {
	}
  
	virtual void decode(MobaLedLib_C& mobaLedLib) 
  {
    if (IrReceiver.decodedIRData.protocol==decode_type_t::NEC)
    {
      bool repeated = false;
      if (IrReceiver.decodedIRData.flags & (IRDATA_FLAGS_IS_AUTO_REPEAT | IRDATA_FLAGS_IS_REPEAT))
      {
        int ticks = IrReceiver.irparams.rawbuf[0] * MICROS_PER_TICK;
        repeated = ticks < 250000;    // repeat within the last 250ms
      }
        
      //if (IrReceiver.decodedIRData.address==0xEF00)	
      if (IrReceiver.decodedIRData.address==0x00)	
      {
#if !defined(ARDUINO_RASPBERRY_PI_PICO)             // RP2040 doesn't support get/set brightness
        if (IrReceiver.decodedIRData.command==0)	// brightness down
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
        else if (IrReceiver.decodedIRData.command==1)	// brightness up
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
#endif            
        {
          doSetOutput(addressOffset+IrReceiver.decodedIRData.command, 1);
        }
      }	
    }
#ifdef MLL_IRRECEIVE_DEBUG
    else
    {
      Serial.println("data skipped, expecting NEC");
    }
#endif
  }	
};
