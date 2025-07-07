
# Description

  - A sleek and responsive switch control panel built using **ESP32** and **LVGL**, designed in **SquareLine Studio**, and wirelessly controls a **relay board (ESP-12F)** via **ESP-NOW**.

---

## Features

- UI designed in **SquareLine Studio**
- Controls 4-channel **relay board (ESP-12F)** via **ESP-NOW**
- Save/restore button states using **SPIFFS**
- Display powered by **LVGL** (with SPI TFT screen)
- Info tab displays:
  - Uptime  
  - MAC Address  
  - Board ID  
  - Firmware Version  

---

## Folder Structure 

```sh
├── src-vi # Vietnamese MOD sources
├── src-eng # English sources
├── get-mac # Script to retrieve MAC address board ESP
├── images # Screenshots and demo assets 
```

---

## Screenshots

<table align="center">
  <tr>
    <td style="padding:10px">
      <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/cyd-pic0.png" alt="Image 1" width="300"/>
    </td>
    <td style="padding:10px">
      <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/cyd-pic1.png" alt="Image 2" width="300"/>
    </td>
  </tr>
  <tr>
    <td style="padding:10px">
      <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/cyd-pic2.png" alt="Image 3" width="300"/>
    </td>
    <td style="padding:10px">
      <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/cyd-pic3.png" alt="Image 4" width="300"/>
    </td>
  </tr>
</table>

---

## Hardware List

- **You can buy the tested compatible parts by click images below**  
<table align="center">
  <tr>
    <td align="center" style="padding: 10px;">
      <a href="https://s.click.aliexpress.com/e/_oBfo3DO" target="_blank">
        <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/esp32cyd.png"
             alt="ESP32 CYD"
             width="300"
             style="border: 1px solid #ccc; border-radius: 8px; padding: 4px;" />
      </a>
      <div><strong>ESP32 CYD (Cheap Yellow Display)</strong></div>
    </td>
    <td align="center" style="padding: 10px;">
      <a href="https://s.click.aliexpress.com/e/_oC5nlQY" target="_blank">
        <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/relayesp12.png"
             alt="ESP-12F Relay"
             width="300"
             style="border: 1px solid #ccc; border-radius: 8px; padding: 4px;" />
      </a>
      <div><strong>ESP-12F Relay Module</strong></div>
    </td>
  </tr>
</table>

---  

## Demo Video

https://github.com/user-attachments/assets/dc04637b-13f2-4d57-9798-e326a3d3a328  

**Watch full tutorial on YouTube**  

[![Watch the video](https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/yt-cover.png)](https://youtu.be/DaC8soiB9zQ)

---
