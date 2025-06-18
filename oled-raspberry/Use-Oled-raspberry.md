# Exemple to use oled in rapsberry 


https://www.instructables.com/Raspberry-Pi-NAS/





```shell
sudo apt-get -y install i2c-tools python3-pip python3-psutil python3-luma.oled



sudo raspi-config  // enable i2c

sudo i2cdetect -y 1

sudo mv oledinfo.py /usr/local/bin/ && sudo chown 0:0 /usr/local/bin/oledinfo.py && sudo chmod 755 /usr/local/bin/oledinfo.py


/usr/local/bin/oledinfo.py



sudo crontab -e

```

ADD in cron

```
@reboot /usr/bin/python3 /usr/local/bin/oledinfo.py
```
