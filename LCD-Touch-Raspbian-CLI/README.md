# LCD TOUCH Raspbian CLI


## Driver 
https://github.com/goodtft/LCD-show


sudo apt update
sudo apt install xorg openbox


## Config 

/boot/config.txt

```
# For more options and information see
# http://rptl.io/configtxt
# Some settings may impact device functionality. See link above for details

# Uncomment some or all of these to enable the optional hardware interfaces
#dtparam=i2c_arm=on
#dtparam=i2s=on
#dtparam=spi=on

# Enable audio (loads snd_bcm2835)
dtparam=audio=on

# Additional overlays and parameters are documented
# /boot/firmware/overlays/README

# Automatically load overlays for detected cameras
camera_auto_detect=1

# Automatically load overlays for detected DSI displays
display_auto_detect=1

# Automatically load initramfs files, if found
auto_initramfs=1

# Enable DRM VC4 V3D driver
#dtoverlay=vc4-kms-v3d
max_framebuffers=2

# Don't have the firmware create an initial video= setting in cmdline.txt.
# Use the kernel's default instead.
disable_fw_kms_setup=1

# Disable compensation for displays with overscan
disable_overscan=1

# Run as fast as firmware / board allows
arm_boost=1

[cm4]
# Enable host mode on the 2711 built-in XHCI USB controller.
# This line should be removed if the legacy DWC2 controller is required
# (e.g. for USB device mode) or if USB support is not required.
otg_mode=1

[all]

hdmi_force_hotplug=1
dtparam=i2c_arm=on
dtparam=spi=on
enable_uart=1
dtoverlay=tft35a:rotate=270
framebuffer_width=480
framebuffer_height=320
hdmi_group=2
hdmi_mode=1
hdmi_mode=87
hdmi_cvt 480 320 60 6 0 0 0
hdmi_drive=2

```



sudo nano /etc/X11/xorg.conf.d/99-fbdev.conf

```
 Section "Device"
    Identifier "FBDEV"
    Driver "fbdev"
    Option "fbdev" "/dev/fb0"
EndSection

Section "Device"
    Identifier "Modesetting"
    Driver "modesetting"
    Option "ShadowFB" "true"
    Option "AccelMethod" "none"
EndSection

Section "Monitor"
    Identifier "Monitor0"
EndSection

Section "Screen"
    Identifier "Screen0"
    Device "FBDEV"
    Monitor "Monitor0"
    DefaultDepth 24
    SubSection "Display"
        Depth 24
        Modes "480x320"
    EndSubSection
EndSection

Section "ServerFlags"
    Option "AutoAddGPU" "false"
EndSection
```


### Se for utilizar um shell kiosk.sh com bashrc


kiosk.sh
```
#!/bin/bash

# Aguarda alguns segundos para garantir que o sistema está carregado
sleep 5

# Desativa o gerenciamento de energia da tela
xset s off          # Desativa o modo de espera de tela
xset -dpms          # Desativa o gerenciamento de energia da tela
xset s noblank      # Desativa o escurecimento da tela
unclutter -idle 0    # Oculta o cursor do mouse imediatamente

xinput set-prop "ADS7846 Touchscreen" "Coordinate Transformation Matrix" 0 -1 1 1 0 0 0 0 1

firefox-esr --kiosk https://rasp.byteslivres.com.br
```

.bashrc
```
if [ -z "$DISPLAY" ] && [ "$(tty)" = "/dev/tty1" ]; then
 sudo startx ~/kiosk.sh
fi
```

### Quando correr o comando startx abrir o navegador no modo kioski

USAR ESTE!!!!


nano ~/.xinitrc
```
#!/bin/sh

xset s off          # Desativa o modo de espera de tela
xset -dpms          # Desativa o gerenciamento de energia da tela
xset s noblank      # Desativa o escurecimento da tela

openbox-session &
firefox-esr --kiosk https://rasp.byteslivres.com.br?token=aaaa

```

### Testar o FB

```
sudo fbi -T 1 -d /dev/fb0 -noverbose /home/pi/img.jpg
```



### Links
https://medium.com/@pspracha/how-to-install-3-5-inch-touch-screen-with-raspberry-pi-and-volumio-version-2-917-57072efbd92f


https://avikdas.com/2018/12/31/setting-up-lcd-screen-on-raspberry-pi.html


https://codepen.io/naikus/pen/BzZoLL