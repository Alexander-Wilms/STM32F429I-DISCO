/**
 @file lcd.h
 @brief LCD module header
 @author: Dr. Klaus Schaefer
 */

#ifndef LCD_H_
#define LCD_H_

void lcd_init( void);
void lcd_write_line( unsigned line, char * text);
//extern SemaphoreHandle_t xSemaphore;
#endif /* LCD_H_ */
