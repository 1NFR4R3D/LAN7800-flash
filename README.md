# Microchip LAN7800 Flashing Script

## WARNING
This script DOES NOT verify that you have selected the correct device, or the correct firmware. Please exercise caution.

### Usage
> $ sudo ./flash.sh *device* *file*


### Notes - 
*ethtool* commands - 
* EEPROM
  * Read - `ethtool -e eth0 offset 0 length 512`
  * Write - `ethtool -E eth0 magic 0x78A5 offset 0 length 512 < eeprom.bin`
* OTP
  * Write - `ethtool -E eth0 magic 0x78F3 offset 0 length 512 < otp.bin`
