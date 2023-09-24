// Title: UART_MCC
// Purpose: In this project we used MCC to configure UART1 module. 
// The main.c include several simple calls in which we can send characters or
// receive characters. 
// Compiler: MPLAB X IDE XC8 v6.05
// Author: Oscar Avendano
// Input: Characters: t,f
// Output: Tx RC7,Rx RC6, RD0,RD1
// PIC18F46K42 Settings % Definitions 

#include "mcc_generated_files/system/system.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "esp8266.h"
#include <xc.h> 
#define LED  PORTDbits.RD0
#define LED1  PORTDbits.RD1
#define _XTAL_FREQ 4000000      // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4     // system clock is 1 usec                                                        


// Used to write a char to terminal.
void UART1_Write_Char(char txData) 
{
    while (U1TXIF == 0); // Wait until transmit buffer is empty
    U1TXB = txData;
}

// Used to write a string to terminal.
void UART1_Write_String(char txData[]) 
{
    // Keep adding to txData until the full string is sent.
    for (int i = 0; i < strlen(txData); i++) 
    {    
        UART1_Write_Char(txData[i]);
    }

}

// Sends AT command and expects to receive a OK.
bool esp8266_startup()
{
    //** Sending Characters to UART **//    
    char uart_rd[2];
    char data[50];
    
    // Send AT command and wait until the ESP sends back "OK"
    UART1_Write_String("AT\r\n");
    
        // Clear the uart_rd array.
        strcpy(uart_rd, "");
        strcpy(data, "");
    
        // Wait for first char input to be received.
        while(!UART1_IsRxReady());
        
        uart_rd[0] = UART1_Read();
        
        // Wait for second char input to be received.
        while(!UART1_IsRxReady());
        
        uart_rd[1] = UART1_Read();
        
        // NOTES // changing the array uart_rd to only 2 elements removes the
        // annoying lagging values that were also received
        // Now I wonder why?
        
        
        if(strcmp(uart_rd, "OK") == 0)
        {
            // Prints the character that was received.
                sprintf(data, "You Entered %s \r\n", uart_rd);
                UART1_Write_String(data);
            
            UART1_Write_String("ESP is connected!\r\n");
            __delay_ms(500);
            
            return true;
        }
        else
        {
            // Prints the character that was received.
            sprintf(data, "You Entered %s \r\n", uart_rd);
            
             
            UART1_Write_String(data);
                
            UART1_Write_String("ESP connection failed!\r\n");
            __delay_ms(500);
            
            return false;
        }
        
    
}

// Send data to ThingSpeak using ESP8266 module
void SendDataToThingSpeak(int Value) 
{
    // Initialize UART1 with a baud rate of 9600 
    //UART1_Init(9600);
    char command[100];
    char command1[100];
    
    // Set ESP8266 to Station mode
    UART1_Write_String("AT+CWMODE=1\r\n");
    __delay_ms(1000);

    // Replace with your Wi-Fi credentials
    UART1_Write_String("AT+CWJAP=\"eero11\",\"sierra1124\"\r\n");
    __delay_ms(8000); // Adjust the delay as needed

    // Construct the HTTP GET request with your API key and data
    //char command[100];
    sprintf(command,"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    UART1_Write_String(command);
    __delay_ms(2000);


//    sprintf(command1, "GET /update?api_key=WQ55SIPE56XX5EE1&field1=%d\r\n", Value);
//    sprintf(command, "AT+CIPSEND=%d\r\n", (int)strlen(command1)); // THIS IS GIVING ME A DOT
//    UART1_Write_String(command);
//    __delay_ms(1000);
    
    sprintf(command1, "GET /update?api_key=WQ55SIPE56XX5EE1&field1=%d\r\n", Value);
    
    sprintf(command, "AT+CIPSEND=%d\r\n", (int)strlen(command1));
    UART1_Write_String(command);
    __delay_ms(1000);
//
//    sprintf(command1, "GET /update?api_key=WQ55SIPE56XX5EE1&field1=%d\r\n", Value);
    UART1_Write_String(command1);
    __delay_ms(1000);

    UART1_Write_String("AT+CIPCLOSE\r\n");
}

///***INITIALIZE***///
int main(void){

    //**Calling Functions**//
    SYSTEM_Initialize();
    
    // Variable declarations.

    SendDataToThingSpeak(5);
    SendDataToThingSpeak(6);
    SendDataToThingSpeak(7);
//    SendDataToThingSpeak(50);
//    SendDataToThingSpeak(500);
    while(1);
    
//    while(1)
//    {
//        // Check to see if the ESP22 is functioning.
//        // Once the "OK" is returned we are good to go.
//       
//            SendDataToThingSpeak(5);
//            SendDataToThingSpeak(50);
//            SendDataToThingSpeak(500);
//        
//    }
    
    return 0;
}
