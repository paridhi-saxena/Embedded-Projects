/*
 * File:   PIC and Galileo communication
 * simple Galileo program example
 * for UMass Lowell 16.480/552
 *
 * Author: Hema Routhu, Paridhi Saxena, Kunwar Taha
 * Created on 2014/9/13
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define Strobe     (40) // IO8
#define GP_4       (6) // IO4
#define GP_5	   (0) // IO5
#define GP_6	   (1) // IO6
#define GP_7	   (38) // IO7
#define GPIO_DIRECTION_IN      (1)
#define GPIO_DIRECTION_OUT     (0)
#define BLINK_TIME_SEC         (2)
#define ERROR_G			(-1)

#define MSG_RESET 0x0 /* reset the sensor to initial state */
#define MSG_PING 0x1 /* check if the sensor is working properly */
#define MSG_GET 0x2 /* obtain the most recent ADC result */
#define MSG_ACK 0xE /* acknowledgement to the commands */
#define MSG_NOTHING 0xF /* reserved */

//open GPIO and set the direction
int openGPIO(int gpio, int direction )
{
        //1.set the GPIO
        //2.set the direction
        //3.set the voltage
        char buffer[256];
        int fileHandle;
        int fileMode;

  //Export GPIO
        fileHandle = open("/sys/class/gpio/export", O_WRONLY);
        if(ERROR_G == fileHandle)
        {
               puts("Error: Unable to opening /sys/class/gpio/export");
               return(-1);
        }
        sprintf(buffer, "%d", gpio);
        write(fileHandle, buffer, strlen(buffer));
        close(fileHandle);

   //Direction GPIO
        sprintf(buffer, "/sys/class/gpio/gpio%d/direction", gpio);
        fileHandle = open(buffer, O_WRONLY);
        if(ERROR_G == fileHandle)
        {
               puts("Unable to open file:");
               puts(buffer);
               return(-1);
        }

        if (direction == GPIO_DIRECTION_OUT)
        {
               // Set out direction
               write(fileHandle, "out", 3);
               fileMode = O_WRONLY;
        }
        else
        {
               // Set in direction
               write(fileHandle, "in", 2);
               fileMode = O_RDONLY;
        }
        close(fileHandle);


   //Open GPIO for Read / Write
        sprintf(buffer, "/sys/class/gpio/gpio%d/value", gpio);
        fileHandle = open(buffer, fileMode);
        if(ERROR_G == fileHandle)
        {
               puts("Unable to open file:");
               puts(buffer);
               return(-1);
        }

        return(fileHandle);  //This file handle will be used in read/write and close operations.
}

//write value
int writeGPIO(int fHandle, int val) {
        if(val ==  0) {
               // Set GPIO low status
               write(fHandle, "0", 1);
        } else {
               // Set GPIO high status
               write(fHandle, "1", 1);
        }

        return(0);
}

int readGPIO(int fileHandle) {
        int value;
        read(fileHandle, &value, 1);
        if('0' == value) {
               value = 0;
        } else {
               value = 1;
        }
        return value;
}

int closeGPIO(int gpio, int fileHandle) {
        char buffer[256];
        close(fileHandle); //This is the file handle of opened GPIO for Read / Write earlier.
        fileHandle = open("/sys/class/gpio/unexport", O_WRONLY);
        if(ERROR_G == fileHandle) {
               puts("Unable to open file:");
               puts(buffer);
               return(-1);
        }
        sprintf(buffer, "%d", gpio);
        write(fileHandle, buffer, strlen(buffer));
        close(fileHandle);

        return(0);
}
//main
int picCommand(int select) {
        int fileHandleGPIO_4;
        int fileHandleGPIO_5;
        int fileHandleGPIO_6;
        int fileHandleGPIO_7;
        int fileHandleGPIO_S;
	int bit0, bit1, bit2, bit3,i, res, adcres;
        fileHandleGPIO_4 = openGPIO(GP_4, GPIO_DIRECTION_OUT);
        fileHandleGPIO_5 = openGPIO(GP_5, GPIO_DIRECTION_OUT);
        fileHandleGPIO_6 = openGPIO(GP_6, GPIO_DIRECTION_OUT);
        fileHandleGPIO_7 = openGPIO(GP_7, GPIO_DIRECTION_OUT);
        fileHandleGPIO_S = openGPIO(Strobe, GPIO_DIRECTION_OUT);

	switch(select) {

		case 1:
               /*1.Strobe high
               2.write data
               3.Strobe low
	           4.Strobe high*/
	        writeGPIO(fileHandleGPIO_S, 1);

	        writeGPIO(fileHandleGPIO_4, 0);
	        writeGPIO(fileHandleGPIO_5, 0);
	        writeGPIO(fileHandleGPIO_6, 0);
	        writeGPIO(fileHandleGPIO_7, 1);
	           
		sleep(BLINK_TIME_SEC);
	           
		writeGPIO(fileHandleGPIO_S, 0);
		printf("strobe is %d\n", readGPIO(fileHandleGPIO_S));
		sleep(BLINK_TIME_SEC);
	        writeGPIO(fileHandleGPIO_S, 1);
	        sleep(BLINK_TIME_SEC);
		printf("PING sent\n");

	       closeGPIO(GP_4, fileHandleGPIO_4);
               closeGPIO(GP_5, fileHandleGPIO_5);
               closeGPIO(GP_6, fileHandleGPIO_6);
               closeGPIO(GP_7, fileHandleGPIO_7);
               fileHandleGPIO_4 = openGPIO(GP_4, GPIO_DIRECTION_IN);
               fileHandleGPIO_5 = openGPIO(GP_5, GPIO_DIRECTION_IN);
               fileHandleGPIO_6 = openGPIO(GP_6, GPIO_DIRECTION_IN);
               fileHandleGPIO_7 = openGPIO(GP_7, GPIO_DIRECTION_IN);
	       printf("message sent");
	       writeGPIO(fileHandleGPIO_S, 0);
	       sleep(BLINK_TIME_SEC);
	       //printf("s is %d\n", readGPIO(fileHandleGPIO_S));
	       //while(readGPIO(fileHandleGPIO_S) == 0) {
               bit3 = readGPIO(fileHandleGPIO_4);
               bit2 = readGPIO(fileHandleGPIO_5);
               bit1 = readGPIO(fileHandleGPIO_6);
               bit0 = readGPIO(fileHandleGPIO_7);
	       printf("%d %d %d %d\n", bit3, bit2, bit1, bit0);
               res = (((((bit3 << 1) + bit2) << 1) + bit1) << 1) + bit0;
               printf("%d %d %d %d \n", bit3, bit2, bit1, bit0);
               if(res == 0xE) {
                    printf("Got Ack!!!!!");
		    break;
               } else {
                    printf("No Ack\n");
               }
               sleep(BLINK_TIME_SEC);
	       //}
	       writeGPIO(fileHandleGPIO_S, 1);
               fileHandleGPIO_4 = openGPIO(GP_4, GPIO_DIRECTION_OUT);
               fileHandleGPIO_5 = openGPIO(GP_5, GPIO_DIRECTION_OUT);
               fileHandleGPIO_6 = openGPIO(GP_6, GPIO_DIRECTION_OUT);
               fileHandleGPIO_7 = openGPIO(GP_7, GPIO_DIRECTION_OUT);
               sleep(BLINK_TIME_SEC);
	       break;

		case 2:
               //ADC result read
	           writeGPIO(fileHandleGPIO_4, 0);
	           writeGPIO(fileHandleGPIO_5, 0);
	           writeGPIO(fileHandleGPIO_6, 1);
	           writeGPIO(fileHandleGPIO_7, 0);
               sleep(BLINK_TIME_SEC);
	           writeGPIO(fileHandleGPIO_S, 0);
	           sleep(BLINK_TIME_SEC);
	           writeGPIO(fileHandleGPIO_S, 1);
	           sleep(BLINK_TIME_SEC);
	           closeGPIO(GP_4, fileHandleGPIO_4);
               closeGPIO(GP_5, fileHandleGPIO_5);
               closeGPIO(GP_6, fileHandleGPIO_6);
               closeGPIO(GP_7, fileHandleGPIO_7);
               fileHandleGPIO_4 = openGPIO(GP_4, GPIO_DIRECTION_IN);
               fileHandleGPIO_5 = openGPIO(GP_5, GPIO_DIRECTION_IN);
               fileHandleGPIO_6 = openGPIO(GP_6, GPIO_DIRECTION_IN);
               fileHandleGPIO_7 = openGPIO(GP_7, GPIO_DIRECTION_IN);
               bit3 = readGPIO(fileHandleGPIO_4);
               bit2 = readGPIO(fileHandleGPIO_5);
               bit1 = readGPIO(fileHandleGPIO_6);
               bit0 = readGPIO(fileHandleGPIO_7);
               sleep(BLINK_TIME_SEC);
               res = (((((bit3 << 1) + bit2) << 1) + bit1) << 1) + bit0;
               printf("%d %d %d %d \n", bit3, bit2, bit1, bit0);
               adcres = 0;
               for(i = 0; i < 3; i++) {
                    int adcbit3 = readGPIO(fileHandleGPIO_4);
                    int adcbit2 = readGPIO(fileHandleGPIO_5);
                    int adcbit1 = readGPIO(fileHandleGPIO_6);
                    int adcbit0 = readGPIO(fileHandleGPIO_7);
                    adcres = adcres + (((((adcbit3 << 1) + 			adcbit2) << 1) + adcbit1) << 1) + adcbit0;
                    adcres = adcres << 4;
                    printf("%d %d %d %d %d \n", bit3, bit2, bit1, bit0, adcres);
		    }
               if(res == 0xE) {
                    printf("Got Ack!!!!!");
                    sleep(BLINK_TIME_SEC);
               } else {
                    printf("No Ack\n");
               }
               sleep(BLINK_TIME_SEC);
               fileHandleGPIO_4 = openGPIO(GP_4, GPIO_DIRECTION_OUT);
               fileHandleGPIO_5 = openGPIO(GP_5, GPIO_DIRECTION_OUT);
               fileHandleGPIO_6 = openGPIO(GP_6, GPIO_DIRECTION_OUT);
               fileHandleGPIO_7 = openGPIO(GP_7, GPIO_DIRECTION_OUT);
               sleep(BLINK_TIME_SEC);
	       break;
}
}
