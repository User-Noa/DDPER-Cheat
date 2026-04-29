
<div align="center">
  
# 🎯 DDPER Cheat - Linux Internal
**internal cheat for DDPER on Linux**

[![English](https://img.shields.io/badge/Language-English-purple?style=for-the-badge)](README.md)
[![Persian](https://img.shields.io/badge/Language-Persian-green?style=for-the-badge)](README_per.md)

  <img src="https://img.shields.io/badge/Platform-Linux-34a0a4?style=for-the-badge&logo=linux" />
  <img src="https://img.shields.io/badge/Status-Undetected-success?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Tested_On-Arch_Linux-1793d1?style=for-the-badge&logo=arch-linux" />
</div>

---

> [!WARNING]  
> 🛑 **Are you on Windows?**   
> Bro, you're in the wrong place! This cheat is strictly for Linux. If you use Windows, go check out this project instead:  
> 👉 **[DPerX for Windows](https://github.com/k-i-o/DPerX)**

---

## 🔥 Features

*   **Aimbot:** Lock onto enemies with multiple modes (Classic, Onkey , Lazer For FNG Mode). Includes prediction!
*   **ESP:** See enemies position with colored lines (Purple for far, Red for close danger).
*   **Triggerbot:** In FNG mode, it automatically switches to laser and fires :D
*   **Spinbot:** Spin around like crazy (perfect for trolling).
*   **ImGui Menu:** Press **INSERT** to toggle.

---

## 🛠️ Prerequisites & Installation

> [!NOTE]  
> This cheat was developed and fully tested on **Arch Linux**, but it should work flawlessly on other distros too.

First, you need to install the required dependencies (compiler, SDL2, etc.). Run the command for your distro:

**Arch Linux / Manjaro:**
```bash
sudo pacman -S base-devel sdl2 mesa pkgconf
```

**Ubuntu / Debian / Mint:**
```bash
sudo apt install build-essential libsdl2-dev libgl1-mesa-dev pkg-config
```

**Fedora:**
```bash
sudo dnf install gcc-c++ SDL2-devel mesa-libGL-devel pkgconf
```

---

## 🚀 Build & Run
**1. Cloning:**
Open a terminal & Clone the repo:
```bash
git clone https://github.com/User-Noa/DDPER-Cheat.git
cd DDPER-Cheat
```

**2. Compiling:**
Run this to build the `cheat.so` file:
```bash
g++ -shared -fPIC main.cpp ./src/*.cpp ./lib/*.cpp \
    -o cheat.so -ldl -lGL -lSDL2 $(pkg-config --cflags sdl2) -pthread \
    -I./src -I./lib
```

**2. Running the game with the cheat:**
**Move** the compiled `cheat.so` file directly into your DDPER game directory. Then, open a terminal in that folder and run:
```bash
sudo LD_PRELOAD=./cheat.so ./DDPER
```

> [!TIP]  
> Once the game opens, hit the **INSERT** key on your keyboard to open the cheat menu. Have fun :D

---

## ⚙️ Important Note About Offsets

> [!IMPORTANT]  
> This cheat and the offsets provided in `offsets.h` are specifically made for **DDPER**.  
> If you want to use this on normal **DDNet**, the logic will work, but you **MUST** update the offsets in `offsets.h` to match your DDNet version, otherwise it will crash!