#include <Slave_LCD_Output.h>
#include <lcd.c>

char deger;
#int_SSP
void SSP_Interrupt()
{
      deger = spi_read();
      // if the command is 'A' close the lcd
      if ( deger == 'A' )
      {
         lcd_putc( '\f' );
         output_d( 0x00 );
      }
      // if not print the char in LCD
      else
         printf(lcd_putc, "%c", deger);   
}
void main()
{
   // define analog inputs as digital
   setup_adc_ports(0xFF);
   set_adc_channel(0x00);
   // LCD Initialization
   lcd_init();
   // CLK_DIV_DISABLED because of it is slave 
   setup_spi(SPI_SLAVE|SPI_L_TO_H| SPI_SS_DISABLED);
   // Enable interrupt
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   
   while(TRUE);
     
}
