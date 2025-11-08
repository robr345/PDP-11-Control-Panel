# PDP-11-Control-Panel
Replacement for the front panel found on DEC PDP-11 computers

Building a working minimal PDP-11 system involves an amount of complexity. It is not sufficient to just add a DC power supply to the PDP-11 backplane, as there are some control signals that are required by the PDP-11 Processor (BDCOK H, BPOK H, BEVNT L and BHALT L) and which must be supplied externally. In original DEC equipment, these signals would typically be provided by circuitry in the DEC power supply in combination with a 3-switch and 2-LED front panel. If you do not have the original DEC power supply and front panel, these control signals need to be synthesised.

This project will enable a QBUS PDP-11 backplane (such as a H9270, H9273 or H9281) when populated with an appropriate collection of PDP-11 boards, to function as a minimal PDP-11 system.
