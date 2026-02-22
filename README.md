# 🛡️ Reborn Easy Login Manager

[![Build and Release](https://github.com/l2reborn/EasyLoginManager/actions/workflows/build.yml/badge.svg)](https://github.com/l2reborn/EasyLoginManager/actions/workflows/build.yml)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20macOS-lightgrey)](https://github.com/l2reborn/EasyLoginManager)
[![Download](https://img.shields.io/badge/download-latest-blue)](https://github.com/421p/RebornEasyLoginManager/releases/)

A lightweight, cross-platform account manager designed for Lineage II players. Securely manage your accounts, passwords, and descriptions with ease.

---

### 📥 Download

You can find the latest releases for Windows and macOS here:
[**Download Latest Version**](https://github.com/421p/RebornEasyLoginManager/releases/)

### ✨ Features

- **Multi-Account Management**: Easily add, edit, and organize multiple accounts.
- **Inline Editing**: Quickly update account descriptions with double-click support.
- **Internationalization**: Full support for English and French languages.
- **Portable & Compact**: 
  - **Windows**: Single-file distribution! All translations are embedded directly into the executable.
  - **macOS**: Native `.app` bundle with DMG installer.
- **Modern UI**: Built with wxWidgets for a native look and feel on all platforms.

### 🚀 Getting Started

#### Prerequisites
- **Windows**: `vcpkg` (for dependency management).
- **macOS**: `Homebrew` (for wxWidgets).

#### Build Instructions

##### Windows (Visual Studio)
```powershell
# Install dependencies via vcpkg
vcpkg install

# Configure and build
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static
cmake --build build --config Release
```

##### macOS
```bash
# Install wxWidgets
brew install wxwidgets dylibbundler

# Configure and build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### 🛠️ Technical Details

- **Language Support**: Uses a custom `LanguageManager` to handle JSON-based translations.
- **Windows Resource Integration**: Translation files are compiled into the Windows executable as `RCDATA` resources, allowing for a single-file distribution without external dependency folders.
- **Static Linking**: On Windows, it uses static linking to ensure the executable runs on systems without requiring additional DLLs.

### 🌍 Translations

We currently support:
- 🇺🇸 English (`en.json`)
- 🇫🇷 French (`fr.json`)

To contribute a new translation, simply add a new `.json` file to the `i18n/` directory and update the `LanguageManager` and UI.

---

*Developed for the Reborn Lineage community.*
