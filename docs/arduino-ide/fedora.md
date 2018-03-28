Installation instructions for Fedora
=====================================

- Install the latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/Main/Software). `$ sudo dnf -y install arduino` will most likely install an older release.
- Open Terminal and execute the following command (copy->paste and hit enter):

  ```bash
  sudo usermod -a -G dialout $USER && \
  sudo dnf install git python3-pip python3-pyserial && \
  cd ~/Arduino/hardware/ && \
  git clone https://github.com/FluoTechnology/FLUO_IOT.git ./aFluoTechnology && \
  cd aFluoTechnology/esp32/tools && \
  python2 get.py
  ```
- Restart Arduino IDE
