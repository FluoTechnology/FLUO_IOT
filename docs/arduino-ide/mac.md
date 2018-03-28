Installation instructions for Mac OS
=====================================

- Install latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/Main/Software)
- Open Terminal and execute the following command (copy->paste and hit enter):

  ```bash
  cd ~/Arduino/hardware/ && \
  git clone https://github.com/FluoTechnology/FLUO_IOT.git ./aFluoTechnology && \
  cd aFluoTechnology/esp32/tools && \
  python2 get.py
  ```
- If you get the error below. Install the command line dev tools with xcode-select --install and try the command above again:
  
```xcrun: error: invalid active developer path (/Library/Developer/CommandLineTools), missing xcrun at: /Library/Developer/CommandLineTools/usr/bin/xcrun```

```xcode-select --install```

- Restart Arduino IDE

