# Prerequisites
* Do **not** use the official Arduino IDE, it sucks
* Visual Studio Code
* Arduino Extension (https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino)
  * When prompted, select "Use bundled arduino-cli" (alternatively, you have to install the official Arduino IDE and set its path in the extension settings)
  * Using the Arduino Board Manager (within the extension; just click on "select board" at the bottom), install the "Arduino AVR Boards" package
  * You'll probably have to restart VS Code after that

## IDE Setup
* Board settings are specified in [arduino.json](/.vscode/arduino.json) (use the control bar at the bottom of VS Code to change settings)
* Board type: Arduino Uno (actually, it's a [clone from AZ Delivery](https://www.az-delivery.de/products/mikrocontroller-board))
* Programmer (i.e. compiler): avrispmkii
* The COM port may need to be adjusted on your PC
  * If you cannot find your board on the Serial Monitor (plug icon at the bottom), you probably have to [install the USB drivers](https://support.arduino.cc/hc/en-us/articles/4411305694610-Install-or-update-FTDI-drivers)

## Programming hints
* The main file's name ([IrrigationSystem.ino](IrrigationSystem.ino)) must be identical to the project name
* Files programmed in C/C++ must be contained in a subdirectory (here: [src](/src/))
* Much of the C/C++ standard library cannot be used

# Overview
The main state machine of the program can be seen in [IrrigationSystem.ino](IrrigationSystem.ino). 

**NOTE:** The project uses [resistive moisture sensors](https://www.az-delivery.de/products/feuchtigkeitssensor-modul?_pos=1&_sid=b3e597e6f&_ss=r), which corrode when current is flowing. Thus, they should only be switched on when actually needed (a measurement time of 1 s every 10 min was chosen arbitrarily). Alternatively, [capacitive moisture sensors](https://www.az-delivery.de/products/bodenfeuchte-sensor-modul-v1-2?_pos=3&_sid=b3e597e6f&_ss=r) can be used that do not corrode but possess only an analog output. In that case, I would have had to implement the comparator logic (using potis and LM393 ICs) myself, which I did not want to bother with 🙃.

## Circuit Diagram
Note that the relay representation is not accurate. In reality, all relays are located on one PCB that is connected to the board's 5V VCC and GND. To switch one of the relays, the respective control pin (D2 - D6) must be pulled to LOW.

![Circuit Diagram](circuit.svg)

## Piping
The pump (located at the bottom of the control cabinet) has an inlet connected to the tank's cap. Its outlet is routed via an 8-wise splitter to five NC valves. These valves are opened via the respective relay to pass water through.

**NOTE 1:** The pump is a simple submersible pump which cannot suck up water through an empty pipe. Thus, the inlet pipe must be filled with water (from the full tank) for the system to work.

**NOTE 2:** The pump and valves are switched independently. When turning on the pump without opening any valve, the system is pressurized which may cause leakage. That's very bad because there is no insulation at all between the piping and wiring...
