# cdfr2020CarteCerveauProg

Code for the robotronik club participation to the french robotics cup (WIP).
This code will implement the brain of the robot: trajectory planning, strategy and actuator control

Reminder: To clone the submodule in the same time, use `git clone --recurse-submodules`, then don't forget to build libopencm3. You need to execute `make install_udev` one time to add the permission to flash.

To compile and flash you need gcc-arm-none-eabi-bin, st-linkand  openocd

To build: `mainTest.elf`
To flash: `mainTest.flash`
To clean: `make clean`

## Software documentation
You can access the documentation generated from the code with doxygen (see the doxygen paragraph to generate it) in the doxygen/html or latex.

## Hardware documentation
Microcontroller used: STM32F401RE on a Nulceo-64 board, main doc:
* [Reference Manual STM32F401 line, doc RM0368, 847 pages](https://www.st.com/resource/en/reference_manual/dm00096844-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
* [STM32F401xD/xE datasheet, 135 pages](https://www.st.com/resource/en/datasheet/stm32f401re.pdf)
    * Alternate function mapping, p45, table 9
* [Reference STM Nuleo-64 Board, doc UM1724, 69 pages](https://www.st.com/resource/en/user_manual/dm00105823-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf)

## Doxygen

Generating the documentation with doxygen:
--> install doxygen on your system

--> run `doxygen doxygenConf` from the project directory

--> the documentation can then be read from doxygen/html/index.html in a browser

--> to generate the pdf for the github (or for yourself !) go into the latex directory `cd doxygen/latex` and run `make`.

Note: You must have a latex distribution on your computer that has `pdflatex` command.

## Coding style

* Tabs are spaces = 4

* Brackets :
    ```
    def peripheral_action_subjectofaction(params){
        code...
        some more code...
    }
    ```
* Function naming
    * _ is the separator
    * peripheral_action_subjectofaction
    * function starting with _ are private and should not be called in high level code
  
* Variables
    * my_var

* Documentation using doxygen
    * comment the function interface in the .h files (javadoc like)
    * detail the function in .c files

* We envision three levels for the code :
    * lowlevel fuctions that must be as general as possible to setup the hardware config (ex: timer fuctions)
    * lowlevel modules with the functions called by the user (ex: motor module, with setup and speed/dir fuctions)
    * rolling unit level (ex: control engineering)
