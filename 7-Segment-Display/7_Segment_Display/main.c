#include <main.h>

void Lookup_Table(int8); // Common Anode Lookup Table

void main()
{
   // Analog ports are defined as digital input
   setup_adc_ports(0xFF);
   set_adc_channel(0x00);
   // D port as output
   set_tris_d(0x00);
   output_d(0xFF);

   // Main Loop
   while(TRUE)
   {
      // if E0 pin is clicked
      if (input(pin_e0))
      {
         delay_us(20);
         // 0-9 counter
         for(int8 i = 0; i<10; i++)
         {
            Lookup_Table(i);
            delay_ms(500);
            
         }
          
      }
      output_d(0xFF);
   }

}


// 7 SEGMENT DISPLAY COMMON ANODE LOOKUP TABLE
void Lookup_Table(int8 counter)
{
   switch(counter)
   {
      case 0: output_d(0xC0); break;
      case 1: output_d(0xF9); break;
      case 2: output_d(0xA4); break;
      case 3: output_d(0xB0); break;
      case 4: output_d(0x99); break;
      case 5: output_d(0x92); break;
      case 6: output_d(0x82); break;
      case 7: output_d(0xF8); break;
      case 8: output_d(0x80); break;
      case 9: output_d(0x90); break;
      default: output_d(0xFF); break;
   }
   
}
