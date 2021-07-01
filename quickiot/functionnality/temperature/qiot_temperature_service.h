/***************************************************************************//**
 * @file
 * @brief Relative Humidity and Temperature GATT Service header
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef QIOT_TEMPERATURE_SERVICE_H_
#define QIOT_TEMPERATURE_SERVICE_H_

#include "stdint.h"

/**************************************************************************//**
 * Init function
 *****************************************************************************/
void qiot_temperature_service_init( void );

/**************************************************************************//**
 * Getter for Temperature sensor measurement data.
 * @param[out] t Temperature (in 0.001 Celsius)..
 *****************************************************************************/
void qiot_temperature_service_get_temperature( int32_t *t );

/**************************************************************************//**
 * Getter for Relative Humidity sensor measurement data.
 * @param[out] rh Relative Humidity in 0.001 percent).
 *****************************************************************************/
void qiot_temperature_service_get_relative_humidity( uint32_t *rh );

/**************************************************************************//**
 * Callback Functions to pass 
 * @param[out] rh Relative Humidity.
 * @param[out] t Temperature.
 *****************************************************************************/
void qiot_temperature_event(uint32_t rh, int32_t t);

#endif // QIOT_ADV_SERVICE_H
