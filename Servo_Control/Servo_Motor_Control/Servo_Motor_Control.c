#include <proje.h>
#define LCD_RS_PIN PIN_B0
#define LCD_RW_PIN PIN_B1
#define LCD_ENABLE_PIN PIN_B2
#define LCD_DATA4 PIN_B3
#define LCD_DATA5 PIN_B4
#define LCD_DATA6 PIN_B5
#define LCD_DATA7 PIN_B6
#include <lcd.c>

#FUSES NOPUT, NOPROTECT, HS, NOWRT, NOBROWNOUT, NOCPD, NOLVP, NOWDT

#define SERVO_PIN PIN_D7

#define GREEN_LED PIN_D0
#define RED_LED PIN_D1

#define SENSOR_1 PIN_C1
#define SENSOR_2 PIN_C4



void Servo_High()
{
   output_high(SERVO_PIN);
   delay_us(550 + 2000);
   output_low(SERVO_PIN);
   delay_us(19450 - 2000);
}


void Servo_Low()
{
   output_high(SERVO_PIN);
   delay_us(550 + 1000);
   output_low(SERVO_PIN);
   delay_us(19450 - 1000);
}



void main()
{  
   set_tris_b(0b00000000);
   output_b(0x00);
   
   set_tris_d(0b10000000);
   output_d(0x00);
   
   set_tris_c(0b000010010);
   output_c(0x00);
   
   lcd_init();
   lcd_putc( "Closed!" );
   output_high( RED_LED );
   Servo_Low();
 
 while(TRUE)
 {
 
  if( input( SENSOR_1 ) == 0 )
  {
   delay_us(20);
   servo_high();
   output_high( GREEN_LED );
   output_low( RED_LED );
   lcd_putc( "\fOpen!" );
  }
  else if ( input( SENSOR_2 ) == 0 )
  {
   delay_us(20);
   Servo_Low();
   output_low( GREEN_LED );
   output_high( RED_LED );
   lcd_putc( "\fClosed!" );
  }

 } 
}
