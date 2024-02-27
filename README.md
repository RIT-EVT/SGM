# REV3-SGM

## Introduction
The Strain Gauge Module (SGM) is a PCB capable of sensing strain on REV3 in multiple locations. It 
supports multiple strain gauges, applying a voltage over the strain gauge and measuring the drop in voltage.
As the strain gauge faces stress/strain, its resistance changes, which results in a change to the voltage 
drop. The SGM amplifies the measured voltage before reading, though it should remain in the range of 0V to 3V3.
The SGM performs processing (filtering) on the raw voltage data before broadcasting the data on the CAN network.
