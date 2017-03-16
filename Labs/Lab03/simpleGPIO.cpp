/* A simple GPIO application */

#include<iostream>
#include<unistd.h> // for usleep
#include"gpio.h"
using namespace exploringBB;
using namespace std;

int main() {
    // Set GPIO pins 49(maps to 23) and 115(maps to 27)
    GPIO outGPIO(49), inGPIO(115);
    
    // Basic output - Flash the LED 10 times, once per second
    // use outGPIO object to setDirection
    outGPIO.setDirection(OUTPUT);
    for (int i=0; i<10; i++) {
        // use outGPIO object to set the value to high
        outGPIO.setValue(HIGH);
        usleep(500000); // micro-second sleep .5 seconds
        // use outGPIO object to set the value to low
        outGPIO.setValue(LOW);
        usleep(500000);
    }
    
    // Basic input example
    
    // Use inGPIO to set the direction to input
    inGPIO.setDirection(INPUT);
    
    cout << "The value of the input is: " << inGPIO.getValue() << endl;
    
    // Fast write to GPIO 1million times
    outGPIO.streamOpen();
    for(int i=0; i<1000000; i++) {
        
        // use outGPIO object to access streamWrite method and then  assert a 'high' signal
        outGPIO.streamWrite(HIGH);
        // use outGPIO object to access streamWrite method and then  assert a 'low' signal
        outGPIO.streamWrite(LOW);
    }
    outGPIO.streamClose();
    
    return 0;
}