# Small Scanner with ADNS2610 and Arduino nano (atmega328)

This project is based on the sourcecode from "Mauskamera" by Daniel Bachfeld,
[Link](https://maker-faire.de/wp-content/uploads/2020/03/Alte-Maus-als-MiniKamera.pdf) which was available in
c't 4/2014 . The image signal is read out and processed with simple thresholding,
because the SSD1306 Oled Display isnt capable of gray values. Pixels are doubled to enhance visibility.

<img src="https://github.com/pgreendale/arduino_mouse_adns2610/blob/main/Whole.jpeg" alt="Whole Thing" width="400"/>


**Attention!** The USB cord is reused and wired directly to the sensor, thus not compatible with USB! Connecting it directly to your computer may harm sensor and computer!   

### Hardware:
* 0.96" Oled Display [Amazon](https://www.amazon.de/AZDelivery-Display-Arduino-Raspberry-gratis/dp/B074N9VLZX/ref=sr_1_1_sspa?__mk_de_DE=ÅMÅŽÕÑ&crid=3LXRAYAXIVZ2I&keywords=SSD1306&qid=1659463826&sprefix=ssd1306%2Caps%2C163&sr=8-1-spons&psc=1&smid=A1X7QLRQH87QA3&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyMDFCUlhaQlVaWDJZJmVuY3J5cHRlZElkPUEwNjcwODQwRlVBODgwSFhUMThNJmVuY3J5cHRlZEFkSWQ9QTAyODcwOTQxWVNNNUE4MEYwUklBJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==)
* Arduino Nano Clone [Amazon](https://www.amazon.de/AZDelivery-Nano-verlötete-Version-Kompatibel/dp/B01MS7DUEM/ref=sr_1_1_sspa?__mk_de_DE=ÅMÅŽÕÑ&crid=21OV1PM3HO43K&keywords=arduino%2Bnano&qid=1659463886&sprefix=arduino%2Bnano%2Caps%2C105&sr=8-1-spons&smid=A1X7QLRQH87QA3&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUExRFU4QjdDM1M2NkFPJmVuY3J5cHRlZElkPUEwNzEzMDg2MlkyT0ZYUTBDRFdFMyZlbmNyeXB0ZWRBZElkPUEwODA2NzA2NlBEQzZSVTk5TEpMJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ&th=1)
* Logitech M-BT96a optical Mouse with ADNS2610

<img src="https://github.com/pgreendale/arduino_mouse_adns2610/blob/main/Circuit.jpg" alt="Circuit" width="400"/>

### Method
* Rewire the sensor, either by cutting the signal lines from the MCU in the mouse or (as I did) resoldering it on breadboard. The current of the LED is set by an matching resistor, I would recommend reusing the parts from the mouse
* The LED draws some current, for stability are extra capacitors recommended. I use 100n and 10µF in parallel
* Resolder USB Cable and use an matching USB-Type A port, you can also directly wire
* RE-Use the optics, the field-of-view is quite small, text with size 4-5pt matches it.
* Compile and Upload software with Arduino Framework, installation of Adafruits SSD1306 Library is needed [SSD1306](https://github.com/adafruit/Adafruit_SSD1306)

Have Fun!

Tips:
* Play with the threshold settings, if contrast it too low or too high
* If the sensor is reset with power glitches, it would turn on power saving of the led, the image will disapper after a few seconds
* Power consumption is quite high, arduino gets a little warm

ToDo:
* Replace per-pixel drawing with bitmap function from Adafruits lib
* automatic thresholding (sensor has sum parameter, based on that are possibilities)
