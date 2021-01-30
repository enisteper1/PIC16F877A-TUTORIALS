#include <ldr_motor_contol.h>
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOCPD                    //No EE protection
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NODEBUG

#define START_STOP_PIN PIN_B3
#define INCREASE_PIN PIN_B1
#define DECREASE_PIN PIN_B2
#define MOTOR_PIN_1 PIN_C3
#define MOTOR_PIN_2 PIN_C4


unsigned int ldr_value_new = 128;
unsigned int ldr_value_old = 128;

unsigned int working_pwm = 128;
unsigned int duty = 0;
// 7 SEG DISPLAY COMMON ANODE
char lookup_table[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0xFF};
void Start();

void main()
{
   // Timer setup
   setup_timer_2(T2_DIV_BY_16,255,1);
   // CCP1 -> RC2 pini analog output
   setup_CCP1(CCP_PWM);
   // Dinlenilen clock kesmesi 20Mhz / 16
   setup_adc(adc_clock_div_16);
   // AN0 pini analog  oldu
   setup_adc_ports(AN0);
   
   // A0 pini giris oldu
   set_tris_a(0x01);
   
   // B portu RB1 RB2 RB3 pinleri giris oldu
   set_tris_b(00001110);
   output_b(0x00);
   
   // C2 C3 C4 pinleri cikis oldu
   set_tris_c(00011100);
   output_c(0x00);
   
   // D portu cikis oldu
   set_tris_d(0x00);
   output_d(0xFF);
   

   // A0 pini dinlenmeye baslandi
   set_adc_channel(0);
   delay_us(20);
   // en basta 0 pwm bastirdim default olarak
   set_pwm1_duty(0);
   while(TRUE)
   {
     // Start butonu sorgulamasi
     if(input(START_STOP_PIN))
     {
      delay_ms(250);
      Start();
     }
     // PWM 0 bassin
     output_d(0xFF);
     set_pwm1_duty(0);
     // 2 tarafi da low yaparak calismamasini motorun garantiledim
     output_low(MOTOR_PIN_1);   
     output_low(MOTOR_PIN_2);
     delay_ms(20);
   }

}

void Start()
{ 
   // Her baslangicta default degerleri ayarliyorum
   // %50 duty cycle
   ldr_value_new = 128;
   ldr_value_old = 128;
   working_pwm = 128;
   //Surucunun bir bacagina low digerine high vererek calistirmaya hazirliyorum
   output_high(MOTOR_PIN_1);
   output_low(MOTOR_PIN_2);
   // %50 duty cycle 
   set_pwm1_duty(working_pwm);
   delay_ms(200);
   
   while(TRUE)
   {
      // Stop button check
      if(input(START_STOP_PIN))
      {
         delay_ms(250);
         break;
      }
      // Hiz arttir 
      else if (input(INCREASE_PIN))
      {
         delay_ms(100);
         if (working_pwm < 230)
               working_pwm  += 25;
         else
            working_pwm = 255;
      }
      // Decrease speed
      else if (input(DECREASE_PIN))
      {
         delay_ms(100);
         // In order to keep working_pwm between 0-255 
         if (working_pwm > 40)
            working_pwm -= 25;
         //min speed
         else
            working_pwm = 20;
      }
      //Read value from potentiometer
      ldr_value_new = read_adc();
      // The reason of +20 or -20 is to prevent differences in pot_value because of noise
      if (ldr_value_new >= ldr_value_old + 20 || ldr_value_new <= ldr_value_old - 20)
      {  
		 // set old value as new value
         ldr_value_old = ldr_value_new;
         working_pwm = ldr_value_new;
         // Min PWM
         if (working_pwm < 20)
            working_pwm = 20;
      }
      
      set_pwm1_duty(working_pwm);
      if(working_pwm > 25)
         duty = working_pwm / 25 - 1;
      else
         duty = 0;
      // Show the value at 7 segment display 
      output_d(lookup_table[duty]);
      
      
      delay_ms(100);
   }

}

