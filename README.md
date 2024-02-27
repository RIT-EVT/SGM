[EVT-core Documentation](https://evt-core.readthedocs.io/)

5) Import the project into Read the Docs, following the steps on 
[the wiki](https://wiki.rit.edu/display/EVT/Documentation+and+Organization+Standards)
   7) `docs/source/index.rst:6` // Update with board documentation
   8) `docs/source/api/index.rst:4,12,15`   // Update this one too

8) Everything in this README from this final step up should be deleted, leaving only the content 
below. When finished, all the changes should be committed and pushed to the setup branch, and a PR
should be created to merge into main.

# REV3-SGM

## Introduction

The Strain Gauge Module (SGM) is a PCB capable of sensing strain on REV3 in multiple locations. It 
supports multiple strain gauges, applying a voltage over the strain gauge and measuring the drop in voltage.
As the strain gauge faces stress/strain, its resistance changes, which results in a change to the voltage 
drop. The SGM amplifies the measured voltage before reading, though it should remain in the range of 0V to 3V3.
The SGM performs processing (filtering) on the raw voltage data before broadcasting the data on the CAN network.



The Strain Gauge Module, SGM, 

What it does: 
    Measures stress on bike
    Operates based on changes in voltage (signal is amplified)
    Gauges will be placed in multiple places with one main board monitoring them
    Voltage should (in theory) range between 0V to 3V3

Possible features to incorporate:
    Measure baseline voltage on startup?
        Changes from this baseline would be our strain
        Makes sense if humidity + temperature affect resistance
    Filter the data
        Process out random spikes/dips
        If there's a sudden but maintained spike: it's significant
            Maybe check if difference is sustained? Or x% different?

Protocols:
    CAN - send out data over CAN
        Won't need to store much data, mostly shipped off
        Possibly sent to VCU, but talk to Carson about this
    3 status LED's
        Available for errors or steady processing
        3 short, 3 long, 3 short, pause. SOS in morse code
        GPIO LED's (PV5, PV6, PV7 a.k.a. 57, 58, 59)
    JTAG and UART on board for debugging
    ADC handles reading the voltages