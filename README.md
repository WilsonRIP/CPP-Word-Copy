# CPP Word

A Microsoft Word clone built with C++ and Qt 6.

## Features

- Rich text editing
- Text formatting (bold, italic, underline, color)
- Paragraph alignment
- Font selection
- Document saving and loading (supports TXT, HTML, RTF)
- Print support
- Word count

## Requirements

- C++17 compatible compiler
- Qt 6.8.2
- CMake 3.16 or later

## Building

### Install dependencies

#### On Ubuntu/Debian:
```bash
sudo apt update
sudo apt install build-essential cmake
# Install Qt 6.8.2
sudo apt install qt6-base-dev libqt6core6 libqt6gui6 libqt6widgets6 libqt6printsupport6
```

#### On macOS (using Homebrew):
```bash
brew install qt6 cmake
```

#### On Windows:
1. Download and install Qt 6.8.2 from the [Qt website](https://www.qt.io/download)
2. Download and install CMake from the [CMake website](https://cmake.org/download/)

### Build the project

```bash
# Clone the repository
git clone https://your-repository-url/cpp-word.git
cd cpp-word

# Create a build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .
```

## Running

```bash
./CPPWord
```

## License

This project is licensed under the MIT License - see the LICENSE file for details. 