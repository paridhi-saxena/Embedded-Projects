#include "i2c-dev.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

__s32 i2c_read_byte_util(__u8 reg) {
	int file;
	char filename[20];
	snprintf(filename, 19, "/dev/i2c-0");
	file = open(filename, O_RDWR);
	if (file < 0) {
		printf("Error in file open");
		exit(1);
	}
	int addr = 0x39;
	if(ioctl(file, I2C_SLAVE, addr) < 0) {
		printf("Error 2");
		exit(1);
	}
	__s32 res;
	printf("%d\n", reg);
	res = i2c_smbus_read_byte_data(file, reg);
	printf("%d\n", res);
	return res;
}

__s32 i2c_read_write_util(__u8 reg) {
	int file;
	char filename[20];
	snprintf(filename, 19, "/dev/i2c-0");
	file = open(filename, O_RDWR);
	if(file < 0) {
	}
	int addr = 0x39;
	if(ioctl(file, I2C_SLAVE, addr) < 0 {
		printf("Error in file while write\n");
		exit(1);
	}
	__s32 res = i2c_smbus_write_byte_data(file, reg);
	return 1;
}

int main1() {
	i2c_read_byte_util(0x92);
	return 1;
}
