# ⚙️ IniManager

![Language](https://img.shields.io/badge/Language-C%2B%2B-blue)
![Build](https://img.shields.io/badge/Build-CMake-green)

## Overview

**IniManager** is a robust C++ project designed for parsing, modifying, and managing **INI configuration files**.

It includes a **static library** that can be easily integrated into other applications, and a standalone executable for testing.

It offers a simple and intuitive API to handle sections, keys, and values, with support for comments and case-insensitive keys.

## 🚀 Features

-   **Load & Save**: Read from and write back to INI files.
-   **CRUD Operations**: Create, Read, Update, and Delete sections and keys.
-   **Comments Support**: Preserve and modify comments for sections and keys.
-   **Case Insensitive**: Keys and section names are handled case-insensitively.
-   **No Dependencies**: Standard C++11 implementation.

## 🛠️ Installation & Build

This project uses **CMake**.

```bash
# Create build directory
mkdir build && cd build

# Configure and Build
cmake ..
make
```

### Manual Compilation
If you prefer not to use CMake, you can compile manually using `g++`:

```bash
g++ main.cpp IniFile.cpp -o IniManager -std=c++17
```

## ▶️ Running the Application

After compilation (via CMake or manually), you can run the main executable which performs a series of tests/demonstrations:

```bash
./IniManager
```

## 🧪 Testing

The project includes unit tests using **GoogleTest**.

```bash
# Build tests
make runIniFileTests

# Run tests
./test/runIniFileTests
```

## 💻 Usage Example

```cpp
#include "IniFile.h"
#include <iostream>

int main() {
    // Load an existing file or create a new one
    IniFile config("config.ini");

    // Set values
    config.addSection("Network");
    config.set("Network", "Host", "127.0.0.1");
    config.set("Network", "Port", "8080");
    
    // Add comments
    config.setKeyComment("Network", "Port", " Web Server Port");

    // Get values
    std::string host = config.get("Network", "Host");
    std::cout << "Host: " << host << std::endl;

    // Save changes
    config.save();

    return 0;
}
```

## 📚 API Reference

-   `load(filename)` / `save(filename)`
-   `get(section, key)` / `set(section, key, value)`
-   `addSection(section)` / `deleteSection(section)`
-   `deleteKey(section, key)`
-   `hasSection(section)` / `hasKey(section, key)`
-   `setSectionComment` / `setKeyComment`
