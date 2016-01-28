/*
 * uart.h
 *
 *  Created on: Nov 16, 2015
 *      Author: schaefer
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx_hal.h"

/** @brief USART driver for USART3 */
class uart
{
public:
  uart ( void);
  void puts( const char * data); //!< output string
  void wait_4_character( void);  //!< wait for receive
  char receive( void);		 //!< get received character
  static UART_HandleTypeDef huart;
  static uint8_t received_char;
};

#endif /* UART_H_ */
