# Build Instructions - Traffic Simulator

Complete compilation guide for Windows using MinGW and Visual Studio.

---

## Prerequisites

### 1. Install MSYS2 (for SDL build)

Download from: https://www.msys2.org/

After installation, open **MSYS2 MinGW 64-bit** terminal and run:

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-SDL2
pacman -S make
```

### 2. Visual Studio (Optional - for console build)

Download Visual Studio Community Edition with C++ support.

---

## Method 1: Build SDL Simulator (MinGW)

### Step 1: Open MSYS2 MinGW 64-bit Terminal

Navigate to project directory:
```bash
cd /c/path/to/Trafficsim
```

### Step 2: Compile the Generator

```bash
g++ src/generator_main.cpp src/TrafficGenerator.cpp \
    -o generator.exe \
    -std=c++11
```

**Expected Output:**
```
(No output means success)
```

### Step 3: Compile the SDL Simulator

```bash
g++ src/simulator_sdl.cpp \
    src/TrafficManager.cpp \
    src/TrafficLight.cpp \
    src/Vehicle.cpp \
    src/FileReader.cpp \
    src/SDLRenderer.cpp \
    -o simulator_sdl.exe \
    -lmingw32 -lSDL2main -lSDL2 \
    -I/mingw64/include/SDL2 \
    -L/mingw64/lib \
    -std=c++11
```

**Expected Output:**
```
(No output means success)
```

### Step 4: Run

**Terminal 1:**
```bash
./generator.exe
```

**Terminal 2:**
```bash
./simulator_sdl.exe
```

---

## Method 2: Build Console Simulator (Visual Studio or MinGW)

### Using MinGW:

```bash
g++ src/main.cpp \
    src/TrafficManager.cpp \
    src/TrafficLight.cpp \
    src/Vehicle.cpp \
    src/FileReader.cpp \
    -o simulator.exe \
    -std=c++11
```

### Using Visual Studio:

1. Open Visual Studio
2. Create new C++ Console project
3. Add all `.cpp` files from `src/` except `simulator_sdl.cpp` and `SDLRenderer.cpp`
4. Build → Build Solution (F7)
5. Run without debugging (Ctrl+F5)

---

## Method 3: Using Makefile (Advanced)

Create a file named `Makefile`:

```makefile
CXX = g++
CXXFLAGS = -std=c++11 -Wall
SDL_FLAGS = -lmingw32 -lSDL2main -lSDL2 -I/mingw64/include/SDL2 -L/mingw64/lib

SRC_DIR = src
BUILD_DIR = build

# Source files
GENERATOR_SRC = $(SRC_DIR)/generator_main.cpp $(SRC_DIR)/TrafficGenerator.cpp
CONSOLE_SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/TrafficManager.cpp \
              $(SRC_DIR)/TrafficLight.cpp $(SRC_DIR)/Vehicle.cpp \
              $(SRC_DIR)/FileReader.cpp
SDL_SRC = $(SRC_DIR)/simulator_sdl.cpp $(SRC_DIR)/TrafficManager.cpp \
          $(SRC_DIR)/TrafficLight.cpp $(SRC_DIR)/Vehicle.cpp \
          $(SRC_DIR)/FileReader.cpp $(SRC_DIR)/SDLRenderer.cpp

# Targets
all: generator console sdl

generator:
	$(CXX) $(GENERATOR_SRC) -o generator.exe $(CXXFLAGS)

console:
	$(CXX) $(CONSOLE_SRC) -o simulator.exe $(CXXFLAGS)

sdl:
	$(CXX) $(SDL_SRC) -o simulator_sdl.exe $(CXXFLAGS) $(SDL_FLAGS)

clean:
	rm -f *.exe *.o lane_*.txt

run_generator:
	./generator.exe

run_console:
	./simulator.exe

run_sdl:
	./simulator_sdl.exe

.PHONY: all clean run_generator run_console run_sdl
```

**Usage:**
```bash
make all              # Build everything
make generator        # Build only generator
make sdl              # Build only SDL simulator
make clean            # Clean build files
make run_generator    # Run generator
make run_sdl          # Run SDL simulator
```

---

## Troubleshooting

### Error: "SDL2/SDL.h: No such file or directory"

**Solution:**
```bash
# In MSYS2 MinGW terminal
pacman -S mingw-w64-x86_64-SDL2
```

### Error: "undefined reference to SDL_main"

**Solution:** Make sure you include `-lmingw32 -lSDL2main -lSDL2` in that exact order.

### Error: "The procedure entry point could not be located"

**Solution:** Copy SDL2.dll to the same directory as simulator_sdl.exe:
```bash
cp /mingw64/bin/SDL2.dll .
```

### Generator creates files but simulator doesn't read them

**Solution:** Make sure both programs run in the same directory where lane files are created.

### Window appears but immediately closes

**Solution:** Check SDL initialization. The window should stay open. Make sure you're not clicking the close button.

---

## Verification

After successful build, you should have:

```
Trafficsim/
├── generator.exe          ✓ Traffic generator
├── simulator.exe          ✓ Console simulator (optional)
├── simulator_sdl.exe      ✓ SDL simulator
└── SDL2.dll              ✓ (if needed)
```

**Test Generator:**
```bash
./generator.exe
```
Expected output:
```
Starting continuous generation...

Generated: BA1PA2345 in lane A
[1] Vehicle BA1PA2345 arrived at Lane A (wait: 1.2s)
[2] Vehicle LU3KA6789 arrived at Lane B (wait: 0.8s)
...
```

**Test Simulator:**
```bash
./simulator_sdl.exe
```
Expected: SDL window opens showing roads and traffic lights.

---

## Build Tips

1. **Always compile in MSYS2 MinGW 64-bit** for SDL projects
2. **Don't mix** MinGW32 and MinGW64 libraries
3. **Run both programs** from the same directory
4. **Check for typos** in file paths (especially in Windows)
5. **Use forward slashes** (/) not backslashes (\) in paths

---

## Quick Build Commands (Copy-Paste)

**For SDL Graphical Version:**
```bash
# Compile generator
g++ src/generator_main.cpp src/TrafficGenerator.cpp -o generator.exe -std=c++11

# Compile SDL simulator
g++ src/simulator_sdl.cpp src/TrafficManager.cpp src/TrafficLight.cpp src/Vehicle.cpp src/FileReader.cpp src/SDLRenderer.cpp -o simulator_sdl.exe -lmingw32 -lSDL2main -lSDL2 -I/mingw64/include/SDL2 -L/mingw64/lib -std=c++11

# Run (in 2 separate terminals)
./generator.exe
./simulator_sdl.exe
```

**For Console Version:**
```bash
# Compile generator
g++ src/generator_main.cpp src/TrafficGenerator.cpp -o generator.exe -std=c++11

# Compile console simulator
g++ src/main.cpp src/TrafficManager.cpp src/TrafficLight.cpp src/Vehicle.cpp src/FileReader.cpp -o simulator.exe -std=c++11

# Run (in 2 separate terminals)
./generator.exe
./simulator.exe
```

---

**Last Updated:** December 26, 2024