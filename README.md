<h1 align="center">Jarvis: Desk Buddy</h1>
<p align="center">
An open-source desk buddy that blinks, tells the time, and notices when you're near.
</p>
<p align="center">
    <img src="https://img.shields.io/badge/Status-In%20development-green" alt="Status">
    <img src="https://img.shields.io/badge/Software-PlatformIO-orange" alt="PlatformIO">
    <img src="https://img.shields.io/badge/Firmware-C%2FC%2B%2B%20(Arduino)-blue" alt="Firmware">
    <br>
    <img src="https://img.shields.io/badge/Licence-MIT-red" alt="MIT">
</p>

---

### Overview

Jarvis is an open-source desktop companion built on an ESP32. It shows a pair of animated eyes on an OLED display, keeps the time synced over WiFi, and uses an ultrasonic sensor to notice when you get close, looking up and reacting when you're near.

Timekeeping is handled over the network: the ESP32 connects to WiFi and syncs to an NTP server on boot, with automatic daylight-saving handling for the local timezone, so there's no dedicated RTC to keep set. The eyes are driven by the FluxGarage RoboEyes library, and a button lets you switch between moods and views.

The project is designed to be:

- Expressive, with animated eyes that blink, glance around, and react to presence.
- Easy to read, with a crisp OLED display and a WiFi-synced clock.
- Simple to build, on a single USB-C-powered board.
- Hackable and well-documented, as a learning reference for embedded projects.

### Pinout

<table>
  <thead>
    <tr>
      <th>Component</th>
      <th>Component Pin</th>
      <th>ESP32 GPIO</th>
      <th>Notes</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td rowspan="4"><a href="https://www.lcsc.com/product-detail/C5248080.html">OLED Display (SSD1306, I²C)</a></td>
      <td>VCC</td>
      <td>3V3</td>
      <td>Do not use 5V / VIN</td>
    </tr>
    <tr>
      <td>GND</td>
      <td>GND</td>
      <td>Common ground</td>
    </tr>
    <tr>
      <td>SDA</td>
      <td>GPIO21</td>
      <td>I²C data</td>
    </tr>
    <tr>
      <td>SCL</td>
      <td>GPIO22</td>
      <td>I²C clock</td>
    </tr>
    <tr>
      <td rowspan="2"><a href="https://www.lcsc.com/product-detail/C52750873.html">Next Screen Button</a></td>
      <td>Pin 1</td>
      <td>GPIO32</td>
      <td>INPUT_PULLUP; pressed = LOW</td>
    </tr>
    <tr>
      <td>Pin 2</td>
      <td>GND</td>
      <td>Other leg to ground</td>
    </tr>
    <tr>
      <td rowspan="2"><a href="https://www.lcsc.com/product-detail/C52750873.html">Previous Screen Button</a></td>
      <td>Pin 1</td>
      <td>GPIO27</td>
      <td>INPUT_PULLUP; pressed = LOW</td>
    </tr>
    <tr>
      <td>Pin 2</td>
      <td>GND</td>
      <td>Other leg to ground</td>
    </tr>
  </tbody>
</table>

---

<h3 align="center">License</h3>
<pre align="center">Copyright © 2026 Carmen<br><br>Jarvis is open-source and released under the MIT License.<br><br>See <a href="./LICENSE.md">LICENSE.md</a> for the full details.</pre>

<h3 align="center">Credits &amp; Team</h3>
<p align="center">Built and maintained by Carmen :)</p>
<p align="center">
    <a href="https://github.com/carmoruda/Chronos-Ion/graphs/contributors">
        <img src="https://contrib.rocks/image?repo=carmoruda/Jarvis" width="50"/>
    </a>
</p>
