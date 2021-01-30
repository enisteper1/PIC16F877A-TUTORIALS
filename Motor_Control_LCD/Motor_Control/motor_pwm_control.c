#include <motor_pwm_control.h>
#include "lcd.c"
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


unsigned int pot_value_new = 128;
unsigned int pot_value_old = 128;

unsigned int working_pwm = 128;
void Start();

void main()
{
   // Timer setup
   setup_timer_2(T2_DIV_BY_16,255,1);
   // CCP1 -> RC2 pin pwm output
   setup_CCP1(CCP_PWM);
   // Clock division 20Mhz / 16
   setup_adc(adc_clock_div_16);
   // AN0 pin analog  input
   setup_adc_ports(AN0);
   
   // A0 pini giris oldu
   set_tris_a(0x01);
   
   // B portu RB1 RB2 RB3 pins are defined as input
   set_tris_b(00001110);
   output_b(0x00);
   
   // C2 C3 C4 pinleri cikis oldu
   set_tris_c(00011100);
   output_c(0x00);
   
   // D port is output
   set_tris_d(0x00);
   output_d(0x00);
   
   // LCD INIT
   lcd_init();
   // A0 pin is listening
   set_adc_channel(0);
   delay_us(20);
   // 0 as Initialization
   set_pwm1_duty(0);
   
   while(TRUE)
   {
     // Check if start button is clicked
     if(input(START_STOP_PIN))
     {
      delay_ms(250);
      Start();
     }
     printf(lcd_putc,"\f\tMotor Control");
     // Set pwm to 0
     set_pwm1_duty(0);
     // Set 2 pin to low in order to prevent motor to rotate 
     output_low(MOTOR_PIN_1);
     output_low(MOTOR_PIN_2);
     delay_ms(20);
   }

}

void Start()
{ 
   // Initialization
   // %50 duty cycle
   pot_value_new = 128;
   pot_value_old = 128;
   working_pwm = 128;
   output_high(MOTOR_PIN_1);
   output_low(MOTOR_PIN_2);
   // %50 duty cycle olarak 128 degerinde pwm basiyorum
   set_pwm1_duty(working_pwm);
   
   while(TRUE)
   {
      // Check if Stop button is clicked
      if(input(START_STOP_PIN))
      {
         delay_ms(250);
         break;
      }
      // Increase Speed 
      else if (input(INCREASE_PIN))
      {
         delay_ms(100);
         if (working_pwm < 230)
               working_pwm  += 25;
         else
            working_pwm = 255;
      }
      // Decrease Speed
      else if (input(DECREASE_PIN))
      {
         delay_ms(100);
         // In order to keep working_pwm between 0-255 
         if (working_pwm > 50)
            working_pwm -= 25;
         else
            working_pwm = 50;
      }
      //Read value from potentiometer
      pot_value_new = read_adc();
      // The reason of +10 or -10 is to prevent differences in pot_value because of noise
      if (pot_value_new >= pot_value_old + 10 || pot_value_new <= pot_value_old - 10)
      {  
         // set old value as new value
         pot_value_old = pot_value_new;
         working_pwm = pot_value_new;
         if (working_pwm > 50)
            working_pwm -= 25;
         else
            working_pwm = 50;
      }
      
      set_pwm1_duty(working_pwm);
     // Show the pwm at LCD  %u -> for unsigned integer
      printf(lcd_putc,"\fPWM: %u", working_pwm);
      delay_ms(100);
   }

}
