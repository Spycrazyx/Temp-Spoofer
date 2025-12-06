# **Temp HWID Spoofer**

A lightweight, temporary hardware ID (HWID) spoofing utility designed to change system-reported serials and identifiers.
**Driver is not included** – this project only contains the user-mode components and interface logic.

## ⭐ Overview

This tool was originally built as a personal experiment to understand how Windows interacts with hardware identifiers through kernel-level drivers. The spoofer temporarily modifies various HWID-related values for testing, debugging, and research purposes.

This repository **does not** contain:

* Any kernel driver
* Any signed or unsigned binaries capable of modifying protected identifiers
* Anything that provides real spoofing functionality out of the box

Instead, it provides the **framework**, **UI/CLI**, and **logic structure** used to communicate with a driver that performs the spoofing.

## 🔧 What It Does

* Sends spoofing requests to a kernel-level driver
* Supports temporary HWID changes (reset after reboot)
* Includes user-mode logic for binding spoof profiles
* Provides a clean base for anyone implementing their own driver

## 📁 What’s Missing

This project does **not** include:

* The kernel driver responsible for actual serial changes
* Any method of bypassing protections, anti-cheat systems, or EAC/BE-related hooks
* Any exploitation or security circumvention code

If you want full functionality, you must implement or provide **your own driver**.

## 🛠️ Requirements

* Windows 10/11
* Kernel driver (not provided) that exposes IOCTLs for serial/identifier rewriting
* Administrator privileges
* Visual Studio / C++ toolchain

## 📂 Project Structure

```
temp spoofer.sln
temp spoofer/
│
├── Entry.cpp               // Program entry point / initialization
├── html.hpp                // Embedded HTML (UI/text formatting)
├── incluedes.hpp           // Global includes & project-wide headers
│
├── Protection/
│   ├── discord.hpp         // Discord webhook/reporting utils
│   ├── httplib.h           // HTTP client library
│   ├── prot.h              // Protection logic (anti-debug, checks, etc.)
│   ├── screenshot.hpp      // Screenshot capture utilities
│   └── stb_image_write.h   // Image output dependency
│
├── Auth/
│   ├── auth.hpp            // Authentication interface
│   ├── json.hpp            // JSON parser for auth/config
│   ├── skStr.h             // String obfuscation (skCrypt-like)
│   ├── Utils.hpp           // Misc utility functions
│   └── library_x64.lib     // External library required for auth
```

## ⚠️ Disclaimer

This project is provided **strictly for educational and research purposes.**
You are responsible for complying with all local laws and terms of service.
Misuse of this project is **not supported or endorsed**.
