/***************************************************************************//**
 * @file
 * @brief Quick IoT Advertising Service
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

#include "qiot_temperature_service_config.h"
#include "qiot_temperature_service.h"

#include "sl_board_control_config.h"
#include "sl_board_control.h"
#include "em_gpio.h"

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#ifdef SL_CATALOG_PRINTF_PRESENT
#include "stdio.h"
#endif // SL_CATALOG_PRINTF_PRESENT
#endif // SL_COMPONENT_CATALOG_PRESENT

#include "em_common.h"
#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "sl_sensor_rht.h"

static sl_sleeptimer_timer_handle_t     sampling_timer_handle_g;
static uint32_t                         relative_humidity_g;
static int32_t                          temperature_g;

static void sampling_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

// -----------------------------------------------------------------------------
// Private function definitions

static void sampling_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  sl_status_t sc;
  (void)(handle);
  (void)(data);

  sc = sl_sensor_rht_get(&relative_humidity_g, &temperature_g);
  if(sc != SL_STATUS_OK){
#ifdef SL_CATALOG_PRINTF_PRESENT
    printf("[QIOT_TEMP] INFO : sensor data get failed with code 0x%2lX\n", sc);
#endif //SL_CATALOG_PRINTF_PRESENT
  }
  qiot_temperature_event(relative_humidity_g, temperature_g);
}

// -----------------------------------------------------------------------------
// Public function definitions

void qiot_temperature_service_init( void )
{
  sl_status_t sc;
  uint32_t timer_timeout_ms     = TEMPERATURE_SAMPLING_RATE_MS;
  uint32_t timer_timeout_ticks  = 0;

  relative_humidity_g = 0;
  temperature_g = - 200;

#ifndef SL_BOARD_ENABLE_SENSOR_RHT
#ifdef SL_CATALOG_PRINTF_PRESENT
  printf("[QIOT_TEMP] WARNING : No Sensor Enable Pin on this board, expect errors 0x31 on i2cspm");
#endif //SL_CATALOG_PRINTF_PRESENT
  #warning("[QIOT_TEMP] WARNING : No Sensor Enable Pin on this board, expect errors 0x31 on i2cspm");
#else 
#if (SL_BOARD_ENABLE_SENSOR_RHT == 0)
  sc = sl_board_enable_sensor(SL_BOARD_SENSOR_RHT);
  printf("[QIOT_TEMP] INFO : Enabling Environmental Sensor supply on behalf of board control : 0x%2lX \n", sc);
#endif // (SL_BOARD_ENABLE_SENSOR_RHT == 0)
#endif // SL_BOARD_ENABLE_SENSOR_RHT

  sampling_timer_callback(NULL, NULL);//Call it once but might not be right moment in app boot
#ifdef SL_CATALOG_PRINTF_PRESENT
  printf("[QIOT_TEMP] DEBUG : Sensor data initialized to Temp = %ld, RHumidity = %lu.\n", temperature_g, relative_humidity_g);
#endif //SL_CATALOG_PRINTF_PRESENT

  if (timer_timeout_ms > 0)
  {
    sl_sleeptimer_ms32_to_tick ( timer_timeout_ms, &timer_timeout_ticks );
    sc = sl_sleeptimer_start_periodic_timer( &sampling_timer_handle_g,
                                        timer_timeout_ticks,
                                        sampling_timer_callback,
                                        (void *)NULL,
                                        0,
                                        0);
    if(sc != SL_STATUS_OK){
#ifdef SL_CATALOG_PRINTF_PRESENT
  printf("[QIOT_TEMP] DEBUG : Periodic timer init returned code 0x%2lX \n", sc);
#endif //SL_CATALOG_PRINTF_PRESENT
    }
#ifdef SL_CATALOG_PRINTF_PRESENT
  printf("[QIOT_TEMP] DEBUG : Periodic sampling enabled. Period =  %lu ms\n", timer_timeout_ms);
#endif //SL_CATALOG_PRINTF_PRESENT
  }
}

void qiot_temperature_service_get_temperature( int32_t *t )
{
  (*t) = temperature_g;
}

void qiot_temperature_service_get_relative_humidity( uint32_t *rh )
{
  (*rh) = relative_humidity_g;
}

SL_WEAK void qiot_temperature_event(uint32_t rh, int32_t t)
{
#ifdef SL_CATALOG_PRINTF_PRESENT
  printf("[QIOT_TEMP] DEBUG : Sensor data event, Temp = %ld, RHumidity = %lu.\n", t, rh);
#else
  (void)(rh);
  (void)(t);
#endif //SL_CATALOG_PRINTF_PRESENT
}