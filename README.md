# PDP-11-Control-Panel
Replacement front panel found on DEC PDP-11 computers.

![ControlPanel](https://github.com/user-attachments/assets/9112db66-7bfd-4a50-b062-37b2f3442094)


Building a working minimal PDP-11 system involves an amount of complexity.

It is not sufficient to just add a DC power supply to the PDP-11 backplane, as there are some control signals that are required by the PDP-11 Processor (BDCOK H, BPOK H, BEVNT L and BHALT L) and which must be supplied externally.

In original DEC equipment, these signals would typically be provided by circuitry in the DEC power supply in combination with a 3-switch and 2-LED front panel. If you do not have the original DEC power supply and front panel, these control signals need to be synthesised.

This project will enable a QBUS PDP-11 backplane (such as a H9270, H9273, H9278 or H9281) when populated with an appropriate collection of PDP-11 boards, to function as a minimal PDP-11 system.

# The control panel has 3 buttons

Reset - Press to reset the computer the running led will briefly go out while the reset process is underway.

Halt -  Press to halt the CPU, press again to run the CPU. Pressing the Halt button will drop the console in to the monitor.

Aux  -  Programmed to Start/Stop the LTC clock.

The control panel requires 5v DC to operate, plug the 10 pin connectector into your backplain or used the seperate connectors to directly wire the control panel the the backplain.

# Bill of Materials

3 x 5mm LED's

3 x button6x6x8.5

J1  2 pole terminal block

U1  PIC16F628A     18 pin dip

U2  10 pin connector 2.54 mm

U3  Crystal 32.7680khz 6pf smd


R1               10K 0805 smd

R2,R3,R4,R7      220 ohm 0805 smd

R5,R6,R8         1K 0805 smd


C1,C2            100pF 0805 smd

C3,C4,C5         100 nF 0805 smd


# PIC16F628A

Program with powplus.hex, the c code is provided in powplus.c

