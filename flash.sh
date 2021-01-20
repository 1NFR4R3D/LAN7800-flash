#!/bin/bash

echo "Reading EEPROM Byte 0x0..."

dump=$(ethtool -e $1 raw on offset 0 length 1)
echo $dump | tr -d \\n | xxd -p

if [[ "$(echo $dump | tr -d \\n | xxd -p)" == "a5" ]]
then
    read -r -p "Are you sure you want to overwrite EEPROM?[y/N] " response
elif [[ "$(echo $dump | tr -d \\n | xxd -p)" == "00" ]]
then
    response=y
else
    echo "Verify the selected device."
    read -r -p "Are you sure you want to continue?[y/N] " response
fi

response=${response,,}
if [[ "$response" =~ ^(yes|y) ]]
then
    echo "Flashing EEPROM..."
    ethtool -E $1 magic 0x78A5 offset 0 length 512 < $2
    echo "Done!"
else
    echo "Aborting..."
fi

function test {
    size=$(stat -c%s "$2")
    file=$(<$2)
    flashed=$(ethtool -e $1 raw on offset 0 length "$size")
    #ToDo - Compare $file and $flashed
    if [[ "$file" == "$flashed" ]]
    then
        echo "Verified! EEPROM Read matches $1!"
    else
        echo "Error! EEPROM Read did not match $1!"
    fi
}
