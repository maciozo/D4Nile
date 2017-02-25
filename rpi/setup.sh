echo "enable_uart=1" >> /boot/config.txt
echo "gpu_mem=16" >> /boot/config.txt

echo "dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline fsck.repair=yes rootwait" > /boot/cmdline.txt

systemctl stop serial-getty@ttyS0.service
systemctl disable serial-getty@ttyS0.service

reboot
