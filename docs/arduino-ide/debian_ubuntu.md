Installation instructions for Debian / Ubuntu OS
=================================================

- Install latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/Main/Software)
- Open Terminal and execute the following command (copy->paste and hit enter):

  ```bash
  sudo usermod -a -G dialout $USER && \
  sudo apt-get install git && \
  wget https://bootstrap.pypa.io/get-pip.py && \
  sudo python get-pip.py && \
  sudo pip install pyserial && \
  cd ~/Arduino/hardware/ && \
  git clone https://github.com/FluoTechnology/FLUO_IOT.git ./aFluoTechnology && \
  cd aFluoTechnology/esp32/tools && \
  python2 get.py
  ```
- Restart Arduino IDE

