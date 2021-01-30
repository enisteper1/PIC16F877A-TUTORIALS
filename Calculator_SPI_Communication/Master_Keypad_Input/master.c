#include <master.h>
#FUSES  NOPUT, NOPROTECT, HS, NOWRT, NOBROWNOUT, NOCPD, NOLVP, NOWDT
//ROWS
#define ROW_1 pin_d0
#define ROW_2 pin_d1
#define ROW_3 pin_d2
#define ROW_4 pin_d3
//COLUMNS
#define COL_1 pin_d4
#define COL_2 pin_d5
#define COL_3 pin_d6
#define COL_4 pin_d7

// Buton Checking Function
void Buton_Sorgula();
// Calculation Function
void Calculation();
// Command Sending Function
void Send_Command( char );
// For the input from the keypad
double inp = 0;
// For the total digit
double out = 0;
char operation = '/';
// Working or not variable
int start = 0;
// It is used to make lcd to print 0 at start
int init = 0;


void main()
{
   setup_adc_ports( 0x00 );
   set_adc_channel(0x00);
   // Interrupt will occur from Low to High
   setup_spi(SPI_MASTER| SPI_L_TO_H| SPI_CLK_DIV_16);
   delay_ms( 200 );
   //First 4 is output and last 4 is input
   set_tris_d(0b11110000);
   output_d(0x00);
  // Interrupt for B0 from Low to High
  ext_int_edge(L_TO_H);
  // Enable B0 and global interrupt
  enable_interrupts( INT_EXT );
  enable_interrupts( GLOBAL );
   
   while(TRUE)
   {  
      if ( start )
      {  
         // Ilk it is first time
         if ( init )
         {
            Send_Command( '0' );
            init  = 0;
         }
         // Check buttons
         Check_Buttons();
      }
      else
         // Stop Command
         Send_Command( 'A' );
   }
}

void Check_Buttons()
{

      //ROW 1 -> 7, 8, 9, /
      output_high( ROW_1 );
      if( input( COL_1 ) )// 7
      {
         if( inp == 0 && out == 0)Send_Command( '\f' );
         inp *=10;inp += 7;Send_Command( '7' );delay_ms(250);
      }
      else if( input( COL_2 ) )// 8
      {
         if( inp == 0 && out == 0) Send_Command( '\f' );
         inp *=10;inp += 8;Send_Command( '8' );delay_ms(250);
      }
      else if( input( COL_3 ) )// 9
      {
         if( inp == 0 && out == 0)Send_Command( '\f' );
         inp *=10;inp += 9;Send_Command( '9' );delay_ms(250);
      }
      else if( input( COL_4 ) )// /
      {
         operation = '/';
         if ( out == 0 ){out = inp; inp = 0;}
         Send_Command( '/' );delay_ms(250);
      }
      output_low( ROW_1 );

      //ROW 2 -> 4, 5, 6, x
      output_high( ROW_2 );
      // 4
      if( input( COL_1 ) )
      {
         if( inp == 0 && out == 0) Send_Command( '\f' );  
         inp *=10;inp += 4; Send_Command( '4' ); delay_ms(250);
      }
      else if( input( COL_2 ) )// 5
      {
         if( inp == 0 && out == 0) Send_Command( '\f' );
         inp *=10;inp += 5; Send_Command( '5' );delay_ms(250);
      }
      else if( input( COL_3 ) )  // 6
      {
         if( inp == 0 && out == 0) Send_Command( '\f' );   
         inp *=10;inp += 6;Send_Command( '6' );delay_ms(250);
      }
      
      else if( input( COL_4 ) )// x
      {
         operation = '*';
         if ( out == 0 ){out = inp; inp = 0;}
         Send_Command( '*' );delay_ms(250);
      }
      output_low( ROW_2 );

      //Row 3 -> 1, 2, 3, -
      output_high( ROW_3 );
      if( input( COL_1 ) )// 1
      {
         if( inp == 0 && out == 0)Send_Command( '\f' );   
         inp *=10;inp += 1;Send_Command( '1' );delay_ms(250);
      }
      else if( input( COL_2 ) ) // 2
      {
         if( inp == 0 && out == 0) Send_Command( '\f' );  
         inp *=10;inp += 2;Send_Command( '2' );delay_ms(250);
      }
      else if( input( COL_3 ) ) // 3
      {
         if( inp == 0 && out == 0)Send_Command( '\f' );  
         inp *=10;inp += 3; Send_Command( '3' );delay_ms(250);
      }
      else if( input( COL_4 ) )// -
      {
         operation = '-';
         if ( out == 0 ){out = inp;inp = 0;}
         Send_Command( '-' );delay_ms(250);
      }
      output_low( ROW_3 );

      //Row 4  -> ON/C, 0, =, +
      output_high( ROW_4 );
      if( input( COL_1 ) )// ON/C
      { // Input ve Output degerlerini sifirladim
         inp = 0;out = 0;
         Send_Command( '\f' );Send_Command( '0' );delay_ms(250);
      }
      if( input( COL_2 ) )// 0
      {
         if( inp == 0 && out == 0)Send_Command( '\f' );   
         inp *=10;Send_Command( '0' );delay_ms(250);
      }
      else if( input( COL_3 ) )// =
      {
         Send_Command( '\f' );
         Calculation();delay_ms(250);
      }
      else if( input( COL_4 ) )// +
      {
         operation = '+';
         if ( out == 0 ){out = inp;inp = 0;}
         inp = 0;Send_Command( '+' );delay_ms(250);
      }
      output_low( ROW_4 );
}

void Calculation()
{
   switch(operation)
   {
      case '+': out += inp; break;
      case '-': out -= inp; break;
      case '*': out *= inp; break;
      case '/': out /= inp; break;
      default: break;
   }
   inp = 0;
   int digit = 0;
   int multiplier = 1;
   // Find multiplier of number
   while( multiplier * 10 <= out)
      multiplier *= 10;
   // Temporary variable that will decrease
   double temp = out;
   // From left to left getting the digits 
   while( multiplier >= 1 )
   {
      digit = temp / multiplier;
      temp -= multiplier * digit;
      // The purpose of " + '0' "  to send digital number as char 'number' 
      Send_Command( digit + '0' );
      
      multiplier /= 10;
   }
   // if Operation is division
   if ( operation == '/' )
   {
      // Send comma
      Send_Command( ',' );
      // In order to get 2 number after the comma multiplied with 100
      temp *= 100;
      // Divided to 10 to get 1st digit
      digit = temp / 10;
      temp -= 10 * digit;
      Send_Command( digit + '0' );
      // Remaining one is second digit
      digit = temp / 1;
      Send_Command( digit + '0' );
   }
}
// SPI haberlesmesi
void Send_Command(char command)
{ 
      // Kesme haberinin gonderilmesi
      output_low( pin_e0 );
      // Komutun gonderilmesi
      spi_write(command);
      delay_us(10);
      // Kesmenin bitirilmesi
      output_high( pin_e0 );
      delay_ms( 10 );
}

#int_ext
void rb0_kesmesi()
{
   // If the circuit is working it will stop if it is not it wll start
   start = !start;
   // Cleared input and output
   out = 0;
   inp = 0;f
   // Made 1 to make circuit show 0 again at start 
   init = 1;
}
