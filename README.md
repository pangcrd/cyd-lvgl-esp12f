<div align="center">

<img src="https://img.shields.io/badge/ESP32-CYD-blue?style=for-the-badge&logo=espressif&logoColor=white"/>
<img src="https://img.shields.io/badge/UI-LVGL-4CAF50?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Protocol-ESP--NOW-purple?style=for-the-badge"/>
<img src="https://img.shields.io/badge/License-MIT-orange?style=for-the-badge"/>

# CYD LVGL ESP-12F Relay Controller

**A sleek touchscreen switch panel built with ESP32 + LVGL that wirelessly controls a 4-channel relay board via ESP-NOW.**

[Features](#-features) · [Screenshots](#-screenshots) · [Hardware](#-hardware) · [Folder Structure](#-folder-structure) · [Demo](#-demo)

</div>

---

## Overview

This project turns the **ESP32 CYD (Cheap Yellow Display)** into a wireless relay controller. The UI is designed in **SquareLine Studio** and rendered via **LVGL** on a SPI TFT screen. Switch states are sent wirelessly to an **ESP-12F relay board** using the **ESP-NOW** protocol — no WiFi router required.

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| 🎨 SquareLine Studio UI | Professionally designed touchscreen interface |
| 📡 ESP-NOW Control | Wireless relay control without a router |
| 🔁 State Persistence | Button states saved/restored via SPIFFS |
| 🖥️ LVGL Display | Smooth UI rendered on SPI TFT screen |
| ℹ️ Info Tab | Shows Uptime, MAC Address, Board ID, Firmware Version |
| 🌐 Bilingual | Vietnamese and English source variants included |

---

## 📸 Screenshots

<table align="center">
  <tr>
    <td align="center" style="padding:10px">
      <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/cyd-pic0.png" alt="Screen 1" width="300"/>
    </td>
    <td align="center" style="padding:10px">
      <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/cyd-pic1.png" alt="Screen 2" width="300"/>
    </td>
  </tr>
  <tr>
    <td align="center" style="padding:10px">
      <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/cyd-pic2.png" alt="Screen 3" width="300"/>
    </td>
    <td align="center" style="padding:10px">
      <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/cyd-pic3.png" alt="Screen 4" width="300"/>
    </td>
  </tr>
</table>

---

## 🛒 Hardware

Click the images below to purchase tested compatible parts.

<table align="center">
  <tr>
    <td align="center" style="padding: 10px;">
      <a href="https://s.click.aliexpress.com/e/_oBfo3DO" target="_blank">
        <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/esp32cyd.png"
             alt="ESP32 CYD"
             width="280"
             style="border: 1px solid #ccc; border-radius: 8px; padding: 4px;" />
      </a>
      <br/><strong>ESP32 CYD (Cheap Yellow Display)</strong>
    </td>
    <td align="center" style="padding: 10px;">
      <a href="https://s.click.aliexpress.com/e/_oC5nlQY" target="_blank">
        <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/relayesp12.png"
             alt="ESP-12F Relay"
             width="280"
             style="border: 1px solid #ccc; border-radius: 8px; padding: 4px;" />
      </a>
      <br/><strong>ESP-12F Relay Module</strong>
    </td>
  </tr>
</table>

---

## 📁 Folder Structure

```
├── src-vi/          # Vietnamese MOD sources
├── src-eng/         # English sources
├── esp12f-src/      # ESP-12F relay firmware
├── get-mac/         # Script to retrieve board MAC address
└── images/          # Screenshots and assets
```

---

## 🎬 Demo

https://github.com/user-attachments/assets/dc04637b-13f2-4d57-9798-e326a3d3a328

**Watch the full tutorial on YouTube:**

<a href="https://youtu.be/DaC8soiB9zQ" target="_blank">
  <img src="https://raw.githubusercontent.com/pangcrd/cyd-lvgl-esp12f/main/images/yt-cover.png"
       alt="Watch on YouTube" width="480"/>
</a>

---

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---
>
