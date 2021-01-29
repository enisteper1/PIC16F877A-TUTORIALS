LIST P=16F877A
    INCLUDE "P16F877A.INC"
 __CONFIG _WDTE_OFF & _PWRTE_OFF & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _WRT_OFF & _CP_OFF & _HS_OSC & _WDT_OFF
 
 ORG	H'00'
 BANKSEL    TRISE
 MOVLW	    0x07
 MOVWF	    ADCON1; PORTE 0. pini giris yapt?m
 CLRF	    TRISD; D yi cikis yapt?m
 BANKSEL    PORTD
 MOVLW	    H'FF'
 MOVWF	    PORTD
 
CBLOCK 0xA0; Counter definitons 1 2 3 for delay and 4 for display
    COUNTER_1
    COUNTER_2
    COUNTER_3
    DISP_COUNTER
ENDC

MOVLW	H'00'
MOVWF	DISP_COUNTER
CALL SET_COUNTERS
    
MAIN_LOOP
    BTFSC   PORTE,0
    GOTO    INIT_DISP
    MOVLW   H'FF'
    MOVWF   PORTD
    GOTO    MAIN_LOOP
    
INIT_DISP
    MOVLW   H'00'
    MOVWF   DISP_COUNTER
COUNT_LOOP
    MOVF    DISP_COUNTER, W  
    CALL    DISPLAY_TABLE ; Get the matched row
    MOVWF   PORTD ; Set PORTD 
    CALL    DELAY 
    INCF    DISP_COUNTER 
    BTFSS   DISP_COUNTER,3; if 3rd bit is 1 skip the "GOTO COUNT_LOOP"
    GOTO    COUNT_LOOP;
    BTFSS   DISP_COUNTER,1; if 3rd bit and 1st bit is 1 skip "GOTO COUNT_LOOP"
    GOTO    COUNT_LOOP
    CLRF    DISP_COUNTER;clear counter 
    GOTO MAIN_LOOP 


DISPLAY_TABLE ;7-SEG-DISP-Common Anode
    
    ADDWF  PCL,f  	
    RETLW  0xC0	;0 
    RETLW  0xF9 ;1 
    RETLW  0xA4 ;2 
    RETLW  0xB0 ;3 
    RETLW  0x99 ;4 
    RETLW  0x92 ;5 
    RETLW  0x82 ;6 
    RETLW  0xF8 ;7 
    RETLW  0x80 ;8 
    RETLW  0x90 ;9 

 DELAY;
    DECFSZ  COUNTER_1,F ; skip  "GOTO DELAY" if COUNTER_1 is 0 
    GOTO    DELAY 
    DECFSZ  COUNTER_2,F ; skip  "GOTO DELAY" if COUNTER_1 and COUNTER_2 is 0 
    GOTO    DELAY 
    DECFSZ  COUNTER_3,F 
    GOTO    DELAY 
    CALL    SET_COUNTERS ; Define counters again for next time
RETURN 
    
SET_COUNTERS; 500ms delay definitions
    MOVLW   0x0F
    MOVWF   COUNTER_3	
    MOVLW   0xE7
    MOVWF   COUNTER_2
    MOVLW   0x2D
    MOVWF   COUNTER_1
RETURN

END



