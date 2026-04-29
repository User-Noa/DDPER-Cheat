<div align="center">
  
# 🎯 DDPER Cheat - Linux Internal
**یه چیت اینترنال برای DDPER روی لینوکس**

[![English](https://img.shields.io/badge/Language-English-purple?style=for-the-badge)](README.md)
[![Persian](https://img.shields.io/badge/Language-Persian-green?style=for-the-badge)](README_per.md)

  <img src="https://img.shields.io/badge/Platform-Linux-34a0a4?style=for-the-badge&logo=linux" />
  <img src="https://img.shields.io/badge/Status-Undetected-success?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Tested_On-Arch_Linux-1793d1?style=for-the-badge&logo=arch-linux" />
</div>

---

> [!WARNING]  
> 🛑  **ویندوزی هستی؟** 
> داداش مسیر رو اشتباه اومدی! این چیت فقط برای لینوکسه. اگه ویندوز داری، برو سراغ این یکی پروژه:  
> 👈 **[DPerX for Windows](https://github.com/k-i-o/DPerX)**

---

## 🔥 امکانات (Features)

*   **ایم بات:** قفل کردن روی انمی‌ها با حالت‌های مختلف (کلاسیک، روی کلید ، لیزر برای مود FNG). قابلیت پیش‌بینی حرکت (Prediction) هم داره!
*   **وال هک:** دیدن پوزیشن انمی‌ها با خطوط رنگی (بنفش برای دور، قرمز برای نزدیک).
*   **تریگر بات:** تو حالت FNG خودش اتوماتیک لیزر می‌کشه و شلیک می‌کنه D:
*   **اسپین بات:** کاراکترت مثل فرفره می‌چرخه (مخصوص ترول کردن بقیه).
*   **منوی گرافیکی:** با دکمه **INSERT** باز میشه؛ یه منوی دارک پرپل خیلی شیک برای تنظیمات.

---

## 🛠️ پیش‌نیازها و نصب

> [!NOTE]  
> این چیت روی **Arch Linux** تست شده و مثل ساعت کار می‌کنه، ولی روی بقیه توزیع‌ها هم جوابه.

اول باید پکیج‌های مورد نیاز (مثل کامپایلر و SDL2) رو نصب کنی. بر اساس توزیع لینوکست یکی از دستورات زیر رو بزن:

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

## 🚀 کامپایل و اجرا

**۱. کلون کردن روی کامپیوتر شخصی:**
ترمینال رو باز کن و پروژه رو کلون کن روی کامپیوترت:
```bash
git clone https://github.com/User-Noa/DDPER-Cheat.git
cd DDPER-Cheat
```

**۲. کامپایل کردن:**
این دستور رو بزن تا فایل `cheat.so` ساخته بشه:
```bash
g++ -shared -fPIC main.cpp ./src/*.cpp ./lib/*.cpp \
    -o cheat.so -ldl -lGL -lSDL2 $(pkg-config --cflags sdl2) -pthread \
    -I./src -I./lib
```

**۳. اجرای بازی با چیت:**
فایل `cheat.so` که ساخته شد رو **Move** کن (انتقال بده) تو دایرکتوری اصلی بازی DDPER. بعد همونجا تو ترمینال دستور زیر رو بزن تا بازی با چیت اجرا بشه:
```bash
sudo LD_PRELOAD=./cheat.so ./DDPER
```

> [!TIP]  
> وقتی بازی باز شد، دکمه **INSERT** رو بزن تا منوی چیت بیاد بالا و حالشو ببری D:

---

## ⚙️ یه نکته مهم در مورد آفست‌ها (Offsets)

> [!IMPORTANT]  
> این چیت و آفست‌هایی که تو فایل `offsets.h` هستن، مخصوص **DDPER** تنظیم شدن.  
> اگه میخوای این چیت رو روی **DDNet** معمولی اجرا کنی، کار می‌کنه، اما باید بری تو فایل `offsets.h` و آفست‌های مخصوص DDNet رو پیدا کنی و جایگزین کنی تا کرش نده.
