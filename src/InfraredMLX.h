#pragma once

/*
 MobaLedLib Infrared Extensions
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

#define EXCLUDE_EXOTIC_PROTOCOLS
#define EXCLUDE_UNIVERSAL_PROTOCOLS
#define DISABLE_LED_FEEDBACK_FOR_RECEIVE
#define USE_STANDARD_DECODE
#define NO_LED_FEEDBACK_CODE
#define DECODE_NEC
#define DECODE_RC5
  
#include "IRremote.hpp"

#include <MLLExtension.h>

#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO)
  #include "CommInterface.h"
#else
  void Update_InputCh_if_Addr_exists(uint16_t ReceivedAddr, uint8_t Direction, uint8_t OutputPower);
#endif

class IRReceiver : public MLLExtension
{
	private:
  uint16_t lastOutputAddress;
  unsigned long lastMillis;

  protected:
  uint16_t receivePin;

	public:
	
	IRReceiver(uint16_t receivePin) {
    this->receivePin = receivePin;
    lastOutputAddress = 0;
	}
	
	virtual void setup(MobaLedLib_C& mobaLedLib) 
  {
    IrReceiver.begin(receivePin, false); // Start the receiver
	}

  virtual void decode(MobaLedLib_C& mobaLedLib)
  {    
  }
  
 	virtual void loop(MobaLedLib_C& mobaLedLib) 
  {
    if (lastOutputAddress!=0 && (millis()-lastMillis)>=250)
    {
      doSetOutput(lastOutputAddress, 0);
      lastOutputAddress = 0;
    }
		if (IrReceiver.decode())
    {
#ifdef MLL_IRRECEIVE_DEBUG
      IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
#endif        
      decode(mobaLedLib);
      IrReceiver.resume(); // Enable receiving of the next value
    }
	}
  
  protected: 
  void doSetOutput(uint16_t address, uint8_t value)
  {
    if (lastOutputAddress!=0 && lastOutputAddress!=address)
    {
      doSetOutput(lastOutputAddress, 0);
    } 
    lastOutputAddress = address;
    lastMillis = millis();
    
  #ifdef MLL_IRRECEIVE_DEBUG
    Serial.print("Set output ");
    Serial.print(address);
    Serial.print(" to ");
    Serial.println(value);
  #endif
              
  #if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO)
    char s[20];
    sprintf(s, "@%4i %02X %02X\n", address, 0, value);
    CommInterface::addToSendBuffer(s);			
  #else            
    #ifdef USE_EXT_ADDR
      Update_InputCh_if_Addr_exists(address, 0, value);			
    #endif
  #endif
  }  
};
          
#include <IRReceiverNEC.h>
#include <IRReceiverRC5.h>

