# 🧩 C++ Tetris Game

![Tetris Game Banner](https://images.ctfassets.net/u4t0f0gy4xt5/3kmer2KOGBKyLNuqnncZQA/7a7f2befc358116e633ee017277660d8/Tetris_banner.jpg?w=1200&h=630&q=50&fit=fill&f=top)

## Overview

A modern implementation of the classic Tetris game built in C++ using SDL2 for rendering. This project aims to recreate the authentic Tetris experience with accurate physics, scoring mechanics, and gameplay features.

## 🎮 Features

- 📊 Classic Tetris gameplay with authentic mechanics
- 🧱 Seven tetromino shapes (I, J, L, O, S, T, Z)
- 🔢 Scoring system based on the original Tetris rules
- ⏫ Speed increases with level progression
- 👀 Next piece preview (up to 4 pieces)
- 🔄 Piece rotation with wall kick support
- ⏸️ Pause functionality
- 🏁 Game over detection

<!-- ## 🖼️ Screenshots -->

<!-- Nothing there yet!! -->

<!-- ![Gameplay Screenshot](https://i.imgur.com/pBGHNMQ.png) -->

## 🛠️ Building and Running

### Prerequisites

- C++ compiler with C++17 support
- SDL2 library
- CMake (for build generation)

### Installation

#### Ubuntu/Debian

```bash
# Install the required dependencies
sudo apt-get update
sudo apt-get install -y build-essential cmake libsdl2-dev

# Clone the repository
git clone https://github.com/lucaspujol/cpp-tetris.git
cd cpp-tetris

# Build the project
mkdir build && cd build
cmake ..
make

# Run the game
./tetris
```

## 🎮 Controls

- **← →** - Move piece left/right
- **↑** - Rotate piece
- **↓** - Soft drop
- **Space** - Hard drop
- **C** - Hold piece
- **P** - Pause game
- **Esc** - Quit game

## 🏆 Scoring System

| Action | Points |
|--------|--------|
| 1 line | 40 × (level + 1) |
| 2 lines | 100 × (level + 1) |
| 3 lines | 300 × (level + 1) |
| 4 lines | 1200 × (level + 1) |

## 🚀 Level Progression

The game speed increases as you clear lines and advance through levels:

| **Level**   | **Frames per cell** |
|:----------:|:------------------:|
|     0      |        48          |
|     1      |        43          |
|     2      |        38          |
|   ...      |       ...          |
|     9      |         6          |
|  10–12     |         5          |
|  13–15     |         4          |
|  16–18     |         3          |
|  19–28     |         2          |
|   29+      |         1          |

## 📚 Project Structure

- `src/` - Source code
  - `main.cpp` - Entry point
  - `Game.cpp` - Game logic
  - `Board.cpp` - Board management
  - `Piece.cpp` - Tetromino definitions and rotations
  - `Renderer.cpp` - SDL2 rendering
- `assets/` - Game assets (fonts, sounds)

## 🧠 Technical Implementation

This project uses:
- **Object-Oriented Programming**: Game logic is organized into classes
- **SDL2**: For window creation, rendering, and input handling
- **Modern C++**: Utilizes C++17 features

## 👏 Acknowledgments

- Original Tetris game created by Alexey Pajitnov
- The [Tetris Wiki](https://tetris.fandom.com/wiki/Tetris_Wiki) for game mechanics reference

---

Made with ❤️ by Lucas Pujol