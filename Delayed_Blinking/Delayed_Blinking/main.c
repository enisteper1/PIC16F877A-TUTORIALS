#include <main.h>

////////////////////////////////////////////////////////
///////////START-PAUSE-STOP BUTONLU DENEY///////////////
//START -> SIRAYLA LED YAKMA
//PAUSE -> SONSUZ DONGU
//STOP  -> 3 KERE YAKIP MAINE GERI DONME
void Blink_Sequential(); // START -> SIRAYLA LED YAK
void Pause(); // PAUSE_BUTTON -> SONSUZ DONGUYE SOK
void Stop(); // STOP_BUTTON -> 3 KERE YAK ve Maine don
void Check_Pause_Stop(); // Check PAUSE_BUTTON AND STOP_BUTTON 
void Close_All_Leds(); // Close all leds 
int8 condition = 1; // Stopa basildiginda led yakmayi durdurmasi icin kosul

void main()
{
   set_tris_b(0xFF);
   output_b(0x00);
   set_tris_d(0x00);
   output_d(0x00);

   while(TRUE)
   {
      if (input(pin_b1)) // Eger START butonuna basilmissa
      {
         delay_us(20); // Hata onlemek icin 20 us delay
         Blink_Sequential(); // Sirayla ledler yanmaya baslar
      }
      
      else if (input(pin_b7)) // Eger STOP  butonuna basilmissa
      {
         delay_us(20); // Hata onlemek icin 20 us delay
         Stop();
      }
      
      Close_All_Leds();
      condition = 1;
   }

}

void Blink_Sequential()
{  
   while(TRUE)
   {  // LOW D3 HIGH D0 CHECK BUTTONS
      output_low(pin_d3);
      output_high(pin_d0);
      delay_ms(250);
      Check_Pause_Stop();
      delay_ms(250);
      Check_Pause_Stop();
      if (condition != 1)
         break;
      
      // LOW D0 HIGH D1 CHECK BUTTONS
      output_low(pin_d0);
      output_high(pin_d1);
      delay_ms(250);
      Check_Pause_Stop();
      delay_ms(250);
      Check_Pause_Stop();
      if (condition != 1)
         break;
         
      // LOW D1 HIGH D2 CHECK BUTTONS
      output_low(pin_d1);
      output_high(pin_d2);
      delay_ms(250);
      Check_Pause_Stop();
      delay_ms(250);
      Check_Pause_Stop();
      if (condition != 1)
         break;
      
      // LOW D2 HIGH D3 CHECK BUTTONS
      output_low(pin_d2);
      output_high(pin_d3);
      delay_ms(250);
      Check_Pause_Stop();
      delay_ms(250);
      Check_Pause_Stop();
      if (condition != 1)
         break;
   }
}
// Check Pause and Stop button
void Check_Pause_Stop()
{
   if (input(pin_b4))
   {
      delay_us(20);
      Pause();
   }
   else if (input(pin_b7))
   {  
      delay_us(20);
      Stop();
   }
}

void Pause()
{
   while(TRUE)
   {
      if (input(pin_b1)) // Pause sorgula
         break;
         
      else if (input(pin_b7)) // Stop sorgula
      {
         Stop();
         break;
      }
      
   }
}


void Stop()
{
   for(int8 i = 0; i < 3; i++)
   {  
      // Tum ledleri yak
      output_high(pin_d0);
      output_high(pin_d1);
      output_high(pin_d2);
      output_high(pin_d3);
      
      delay_ms(250);
      Close_All_Leds();
      delay_ms(250);
      
      
   }
   // Ledlerin yanmasini onlemek icin kosul
   condition = 0; 
   
}

void Close_All_Leds()

{
   output_low(pin_d0);
   output_low(pin_d1);
   output_low(pin_d2);
   output_low(pin_d3);

}
