#include "i2c-dev.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

float getTemp() {
	int file;
  	int adapter_nr = 0; /* probably dynamically determined */
  	char filename[20];
	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
  	file = open(filename, O_RDWR);
  	if (file < 0) {
		printf("error 1");
    		exit(1);
  	}
	int addr = 0x48; /* The I2C address */

  	if (ioctl(file, I2C_SLAVE, addr) < 0) {
    		/* ERROR HANDLING; you can check errno to see what went wrong */
		printf("error 2");
    		exit(1);
  	}
	__u8 reg = 0x92; /* Device register to access */
 	__s32 res = 0x0000;
 	char buf[10];

  	/* Using I2C Read, equivalent of i2c_smbus_read_byte(file) */
	__u8 MSB = i2c_smbus_read_byte(file);
	__u8 LSB = i2c_smbus_read_byte(file);

	int temparature = ((MSB << 8) | LSB) >> 4;
	//printf("%d %d\n", MSB, LSB);
	float celsius = temparature*0.0625;
	printf("Temp in C %f\n", celsius);
	return celsius;	
}

