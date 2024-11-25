# Description

Driver creates block input device for every mtd device.
Writing to block device sends endcoded data to mtd device.

Triggered timer callback verifies integrity of data by reading every block and writing it back if errors were detected. 
