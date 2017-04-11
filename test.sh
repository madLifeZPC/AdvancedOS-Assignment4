#!/bin/sh

sudo make uninstall
rm -f /dev/fourmb_device

sudo mknod /dev/fourmb_device c 61 0
make
sudo make install

gcc test_llseek.c -o test_llseek
sudo ./test_llseek

dmesg | tail -10

echo abc > /dev/fourmb_device 
cat /dev/fourmb_device 
echo defg > /dev/fourmb_device 
cat /dev/fourmb_device

gcc test_write.c -o test_write
sudo ./test_write
