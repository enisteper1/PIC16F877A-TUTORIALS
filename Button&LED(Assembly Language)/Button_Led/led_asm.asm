
    LIST P=16F877A
    #INCLUDE "P16F877A.INC"

    ORG H'00'
    BANKSEL TRISB
    MOVLW   H'FF'
    MOVWF   TRISB ; INPUT
    CLRF    TRISD ; OUTPUT
    BANKSEL PORTD
    CLRF    PORTD
    
    
    MAIN_LOOP
	BTFSC	PORTB,0 ; Check if button is clicked
	GOTO	OPEN_LED
	BCF	PORTD,0; Close the led if button is not clicked
	GOTO	MAIN_LOOP
    OPEN_LED 
	BSF	PORTD,0 ; Open the led
	GOTO	MAIN_LOOP
    END


