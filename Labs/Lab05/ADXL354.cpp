#include<iostream>
#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<iomanip>
#include<unistd.h>
#include<math.h>
using namespace std;

// Small macro to display value in hexadecimal with 2 places
#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)

// The ADXL345 Registers required for this example
#define DEVID           0x00
#define POWER_CTL       0x2D
#define DATA_FORMAT     0x31
#define DATAX0          0x32
#define DATAX1          0x33
#define DATAY0          0x34
#define DATAY1          0x35
#define DATAZ0          0x36
#define DATAZ1          0x37
#define BUFFER_SIZE     0x40
unsigned char dataBuffer[BUFFER_SIZE];

// Write a single value to a single register
int writeRegister(int file, unsigned char address, char value) {
    unsigned char buffer[3];
    buffer[0] = address;
    buffer[1] = value;
    if (write(file, buffer, 2)!=2) {
        cout << "Failed write to the device" << endl;
        return 1;
    }
    return 0;
}

// Read the entire 40 registers into the buffer (10 reserved)
int readRegisters(int file) {
    // Write a 0x00 to the device sets the address back to
    // 0x00 for the coming block read
    writeRegister(file, 0x00, 0x00);
    if(read(file, dataBuffer, BUFFER_SIZE)!=BUFFER_SIZE) {
        cout << "Failed to read in the full buffer." << endl;
        return 1;
    }
    if(dataBuffer[DEVID]!=0xE5) {
        cout << "Probelm detected! Device ID is wrong" << endl;
        return 1;
    }
    return 0;
}

// Short is 16-bits in size on BBGW
short combineValues(unsigned char msb, unsigned char lsb) {
    // shift the msb right by 8 bits and OR with lsb
    return ((short)msb<<8)|(short)lsb;
}

int main() {
    
    int file;
    cout << "Starting the ADXL345 sensor application" << endl;
    if((file=open("/dev/i2c-2", O_RDWR)) < 0) {
        cout << "failed to open the bus " << endl;
        return 1;
    }
    if(ioctl(file, I2C_SLAVE, 0x53) < 0) {
        cout << "failed to connect to the sensor" << endl;
        return 1;
    }
    writeRegister(file, POWER_CTL, 0x08);
    // Setting mode to 00000000=0x00 for +/-2g 10-bit
    // Setting mode to 00001011=0x0B for +/-16g 13-bit
    writeRegister(file, DATA_FORMAT, 0x00);
    readRegisters(file);
    cout << "The Device ID is: " << HEX(dataBuffer[DEVID]) << endl;
    cout << "The POWER_CTL mode is: " << HEX(dataBuffer[POWER_CTL]) << endl;
    cout << "The DATA_FORMAT is: " << HEX(dataBuffer[DATA_FORMAT]) << endl;
    cout << dec << endl;    // reset back to decimal
    
    // now loop and display the x, y, z, accelerometer for 60 seconds
    float COUNTS_PER_G = 4096.0;
    float roll = 0.0;
    float pitch = 0.0;
    int count = 0;
    while (count < 60) {
        short x = combineValues(dataBuffer[DATAX1], dataBuffer[DATAX0]);
        short y = combineValues(dataBuffer[DATAY1], dataBuffer[DATAY0]);
        short z = combineValues(dataBuffer[DATAZ1], dataBuffer[DATAZ0]);
        //use \r and flush to write the output on the same line
        float ax = x/COUNTS_PER_G;
		float ay = y/COUNTS_PER_G;
		float az = z/COUNTS_PER_G;
	
	    roll = atan2(ay, az)*180/M_PI;
	    pitch = atan2(ax, sqrt(ay*ay + az*az))*180/M_PI;
        
        cout << "X="<<x<<" Y="<<y<<" Z="<<z<<" sample="<<count<< " Roll: " << roll << " " << " Pitch: " << pitch << "        \r"<<flush;
	    
	    //cout << " Roll: " << roll << " " << " Pitch: " << pitch << "        \r"<<flush;
        usleep(1000000);
        readRegisters(file);    // read the sensor again
        count++;
    }
    close(file);
    return 0;
    
}