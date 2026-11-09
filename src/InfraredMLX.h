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

#if !defined(AVR) && !defined(ESP32) && !defined(ARDUINO_RASPBERRY_PI_PICO)
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
#include <IRReceiverNEC.h>
#include <IRReceiverRC5.h>
