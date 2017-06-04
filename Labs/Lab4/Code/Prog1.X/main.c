/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC16F18857
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.20
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "mcc_generated_files/mcc.h"

/*
                         Main application
 */

void set_receive() {
   /*1.set RA2 as input
   2.set RC2 RC3 RC4 RC5 as input
   3.set them as digital I/O*/
  
}

unsigned char receive_msg() {
    set_receive();
    unsigned char msg;
    /*1.wait strobe high
    2.wait strobe low
    3.read the data
    4.wait strobe high
    5.return the data*/
    while(PORTAbits.RA3) {}
    if(!PORTAbits.RA3) {
        msg = ((((PORTCbits.RC2 << 1 + PORTCbits.RC3) << 1) + PORTCbits.RC4) << 1) + PORTCbits.RC5;
    }
    return msg;
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    
    unsigned char msg;    
    /*while(1)
    {  
    msg=receive_msg();
    if(msg == 0xd)
   	  PORTCbits.RC0 = 1;  //light the LED
    else
        PORTCbits.RC0 = 0;
    }*/
    
    while (1)
    {
       int val = ADCC_GetSingleConversion(channel_ANA2);
        /*if(val > 500) 
        {// Add your application code
            IO_RC6_SetHigh();
        
        } else {
            IO_RC6_SetLow(); 
        }*/
        msg = receive_msg();
        PORTAbits.RA4 = 0;
        switch(msg) {

            case 0x1  :
                PORTAbits.RA4 = 1;
                TRISC = 00000000;
                IO_RC2_SetDigitalOutput();
                IO_RC3_SetDigitalOutput();
                IO_RC4_SetDigitalOutput();
                IO_RC5_SetDigitalOutput();
                PORTCbits.RC2 = 0;
                PORTCbits.RC3 = 1;
                PORTCbits.RC4 = 1;
                PORTCbits.RC5 = 1;
                break; /* optional */
	
            case 0x2 :
                PORTAbits.RA4 = 1;
                TRISC = 00000000;
                IO_RC2_SetDigitalOutput();
                IO_RC3_SetDigitalOutput();
                IO_RC4_SetDigitalOutput();
                IO_RC5_SetDigitalOutput();
                PORTCbits.RC2 = 0;
                PORTCbits.RC3 = 1;
                PORTCbits.RC4 = 1;
                PORTCbits.RC5 = 1;
                break; /* optional */
  
            default : /* Optional */
                PORTAbits.RA4 = 0;
        }
        //SLEEP(10);
        IO_RC2_SetDigitalInput();
        IO_RC3_SetDigitalInput();
        IO_RC4_SetDigitalInput();
        IO_RC5_SetDigitalInput();
        /*if(msg == 0x1) {
            PORTAbits.RA4 = 1;  //light the LED
        } else {
            PORTAbits.RA4 = 0;
        }*/
    }
}

int getFourBits(int adcVal) {
    int sol = adcVal && 0x0000000F;
    sol = sol << 28;
    return sol;
}
/**
 End of File
*/