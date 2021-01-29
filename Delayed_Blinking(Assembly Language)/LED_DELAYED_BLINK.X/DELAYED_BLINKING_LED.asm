
LIST P=16F877A
#INCLUDE "P16F877A.INC"
__CONFIG   _CP_OFF  & _LVP_OFF & _BOREN_OFF  & _WDT_OFF & _PWRTE_ON & _HS_OSC; Configuration setup 
    
ORG H'00'
BANKSEL TRISB; BANK 2
MOVLW   H'FF'
MOVWF   TRISB; INPUT 
CLRF    TRISD; OUTPUT
BANKSEL PORTD; BANK 1
CLRF    PORTD

CBLOCK 0xA0; Counter Definitions
    COUNTER_1; 1 2 3 is delay counter
    COUNTER_2
    COUNTER_3
    COUNTER_4; This counter is used for opening and closing the led for 3 times
    A; Variable to check if stop button is clicked
ENDC

CALL    SET_COUNTERS ; Defining Counter Values
MOVLW   D'3'
MOVWF   COUNTER_4
MOVLW   D'0'
MOVWF   A
    
MAIN_LOOP; Ana dongu
    BTFSC   PORTB,1; Start Pin is checked
    GOTO    LED_YAK; if it is clicked start blinking
    CALL    CLOSE_ALL; if not close all leds
    CALL    CHECK_BUTTONS; Stop and Pause button control
    GOTO    MAIN_LOOP
    
LED_YAK
    BCF	    PORTD,3; Close last led 
    BSF	    PORTD,0 ; Open first led
    CALL    DELAY; 500ms delay
    INCF    A 
    DECFSZ  A,F; the purpose of this A continue to blinking if the stop button then pause button is clicked
    GOTO    MAIN_LOOP
    BCF	    PORTD,0; Close first led
    BSF	    PORTD,1; Open second led
    CALL    DELAY
    INCF    A
    DECFSZ  A,F
    GOTO    MAIN_LOOP
    BCF	    PORTD,1
    BSF	    PORTD,2
    CALL    DELAY
    INCF    A
    DECFSZ  A,F
    GOTO    MAIN_LOOP
    BCF	    PORTD,2
    BSF	    PORTD,3
    CALL    DELAY
    INCF    A
    DECFSZ  A,F
    GOTO    MAIN_LOOP
    GOTO    LED_YAK
    
CLOSE_ALL; Close all leds
    BCF	    PORTD,0
    BCF	    PORTD,1
    BCF	    PORTD,2
    BCF	    PORTD,3
    MOVLW   D'0'
    MOVWF   A
RETURN
    
    
CHECK_BUTTONS
    BTFSC   PORTB,4; Check Pause
    CALL    INFINITE_LOOP
    BTFSC   PORTB,7; Check Stop
    CALL    INVOKE
RETURN
 
INVOKE; Function to open and close leds 3 times
    CALL    SET_COUNTERS_2; 250 ms delay counter definition
    BSF	    PORTD,0
    BSF	    PORTD,1
    BSF	    PORTD,2
    BSF	    PORTD,3
    CALL    DELAY_2
    BCF	    PORTD,0
    BCF	    PORTD,1
    BCF	    PORTD,2
    BCF	    PORTD,3
    CALL    DELAY_2
    DECFSZ  COUNTER_4
    GOTO    INVOKE
    MOVLW   D'3'
    MOVWF   COUNTER_4
    MOVLW   D'5'
    MOVWF   A
 RETURN
 
INFINITE_LOOP; Pause
    BTFSC   PORTB,7;Stop button check
    CALL    INVOKE
    CLRF    A; While in the loop if the stop button is clicked and then start button is clicked cleared the A in order to prevent closing
    BTFSS   PORTB,1; Check start button
    GOTO    INFINITE_LOOP
RETURN

 DELAY
    DECFSZ  COUNTER_1,F ; 
    GOTO    DELAY 
    DECFSZ  COUNTER_2,F 
    GOTO    DELAY 
    CALL    CHECK_BUTTONS ; 
    DECFSZ  COUNTER_3,F ;
    GOTO    DELAY ;
    CALL    SET_COUNTERS ; 
RETURN  
    
DELAY_2; In DELAY function because of there is a stop button checking in CHECK_BUTTONS another loop was starting in loop so created 2nd Delay function to avoid it
    BTFSC   PORTB,4
    CALL    INFINITE_LOOP
    DECFSZ  COUNTER_1,F
    GOTO    DELAY_2
    DECFSZ  COUNTER_2,F
    GOTO    DELAY_2
    DECFSZ  COUNTER_3,F
    GOTO    DELAY_2
    CALL    SET_COUNTERS_2
RETURN
    
SET_COUNTERS; 500ms delay 
    MOVLW   0x0B
    MOVWF   COUNTER_3	
    MOVLW   0xE7
    MOVWF   COUNTER_2
    MOVLW   0x2D
    MOVWF   COUNTER_1
RETURN

SET_COUNTERS_2; 250ms delay
    MOVLW   0x06
    MOVWF   COUNTER_3	
    MOVLW   0x74
    MOVWF   COUNTER_2
    MOVLW   0x16
    MOVWF   COUNTER_1
RETURN
    
END