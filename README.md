# ANDRE
We are the Autonomous Navigational Driving Roadster Engineers, and we are creating the Marz Rover, a fully autonomous minature Mercedes Benz designed for path navigation and object avoidance.

# Programming Environment Setup
This code was deveveloped using the PlatformIO IDE. This code should work with the default Ardunio IDE, but libraries dependencies will need to be handled. 

To install the PlatformIO IDE, follow the steps [here](https://platformio.org/get-started/ide?install=vscode).

Then, clone this repository. The default PlatformIO project folder on Windows is in `%UserProfile%/Documents/PlatformIO`

Open the project in VSCode. You can open a directory in its file explorer on the left-hand side.

The main programming logic is within `main.cpp`. You need to specify the port in which to upload the code to in `platformio.ini`. To find out which port this is, on Windows you can search for `Device Manager` and there will be an entry for the Arduino under `Ports (COM & LPT)`.

To run a different line of code logic, rename the current `main.cpp` to another file with a different extension, such as `.txt`. Rename the desired file to execute to `main.cpp`.