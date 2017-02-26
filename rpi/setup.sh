# Lock core frequency to 250MHz
echo "enable_uart=1" >> /boot/config.txt

# Save some memory
echo "gpu_mem=16" >> /boot/config.txt

# Remove console=serial0,115200
# Since we want to use it to communicate with mc
echo "dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline fsck.repair=yes rootwait" > /boot/cmdline.txt

# Disable terminal service on serial port
systemctl stop serial-getty@ttyS0.service
systemctl disable serial-getty@ttyS0.service

# Run init script on boot
crontab -l > tempcron
echo "@reboot /home/pi/D4Nile/rpi/init.sh" >> tempcron
crontab tempcron
rm tempcron

reboot
