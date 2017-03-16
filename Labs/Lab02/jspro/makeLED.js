var myArgs = process.argv.slice(2)
var LED3_PATH = "/sys/class/leds/beaglebone:green:usr3"

function writeLED(filename, value, path) {
    var fs = require('fs');
    // this call must be synchronous! Otherwise the timer will not work as there are
    // three calls to write that happen at the same time for the flash call
    
    try {
        fs.writeFileSync(path+filename, value);
    } catch (err) {
        console.log("The Write Fialed to the File: " + path+filename);
    }
}

function removeTrigger() {
    writeLED("/trigger", "none", LED3_PATH);
}

console.log("Starting the LED Node.js Program");
if(myArgs[0]==null) {
    console.log("There is an incorrect number of arguments.");
    console.log("   Usage is: nodejs makeLED.js command");
    console.log("   where command is one of: on, off, flash, or status.");
    process.exit(2); // exits with the error code 2 (incorrect usage)
}

switch(myArgs[0]) {
    case 'on':
        console.log("Turning the LED On");
        removeTrigger();
        writeLED("/brightness","1",LED3_PATH);
        break;
    case 'off':
        console.log("Turning the LED Off");
        removeTrigger();
        writeLED("/brightness","0",LED3_PATH);
        break;
    case 'flash':
        console.log("Flashing the LED");
        removeTrigger();
        writeLED("/trigger","timer",LED3_PATH);
        writeLED("/delay_on","50",LED3_PATH);
        writeLED("/delay_off","50",LED3_PATH);
        break;
    case 'status':
        console.log("Showing trigger status");
        fs = require('fs');
        fs.readFile(LED3_PATH+"/trigger", 'utf8', function(err,data) {
            if(err) {
                return console.log(err);
            }
            console.log(data);
        });
        break;
    default:
        console.log("Invalid Command");
}
console.log("End of Node.js Script");