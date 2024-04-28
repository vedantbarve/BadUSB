# BadUSB script

This is the script that runs when we insert the USB in the victim's machine. Before being able to inject keyboard strokes, we need to burn the bootloader onto the attiny85.

## Steps to burn micronucleus bootloader on the attiny85 using Arduino Uno
1. Configure Arduino Uno as ISP by uploading the script given in examples -> ArduinoISP onto it.
![bootloader](https://github.com/vedantbarve/BadUSB/blob/master/assets/bootloader.png)
2. Make appropriate connections as given above.
3. Edit the COM number to which the Arduino Uno has been connected in the Burn_AT85_bootloader.bat file, for example, COM5.
4. Move both the files ATtiny85.hex and Burn_AT85_bootloader.bat to the root folder of Arduino IDE (selected while installing Arduino IDE).
5. Run Burn_AT85_bootloader.bat as admin.
6. After a few seconds, a message AVRdude done will appear, indicating success.

After following the above steps, make a PCB according to the schematic given in the [pcb](https://github.com/vedantbarve/BadUSB/tree/master/pcb) folder.

## Steps to setup Arduino IDE for Attiny85
1. Add the following url to the Additional Borad Manager, in Files > Preferences : http://digistump.com/package_digistump_index.json.
2. Download Digistump AVR from Board manager.
3. Select the board as Digispark micronucleus bootloader and flash the desired script.
