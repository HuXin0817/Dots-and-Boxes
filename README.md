# Dots and Boxes

A sophisticated implementation of the classic Dots and Boxes game with multiple AI strategies and a Qt-based graphical user interface.

## Features

- Interactive graphical interface using Qt6
- Multiple AI opponents with different strategies:
  - GreedyRobot
  - ImproveGreedyRobot
  - SimulationRobot
  - MonteCarloRobot
  - ParallelSearchRobot
- Configurable game settings
- Score tracking and display
- Visual representation of the game board
- Background mode for automated gameplay
- LRU cache optimization for AI move evaluation

## Requirements

- C++23 compatible compiler
- Qt6 (Core, Gui, Widgets)
- CMake 3.16 or higher
- Intel TBB (Threading Building Blocks) for parallel processing

## Build Instructions

### macOS

```bash
# Clone the repository
git clone <repository-url>
cd Dots-and-Boxes

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# Run the game
./Dots_and_Boxes
```

### Other Platforms

The project can be built on other platforms with Qt6 support. Follow similar steps for your specific platform.

## Usage

### Basic Usage

1. Launch the game application
2. Use the mouse to click on edges to claim them
3. Complete boxes by claiming all four edges to score points
4. The player with the most boxes at the end wins

### Command Line Arguments

The game supports the following command line arguments:

| Option          | Short | Long           | Description            | Default |
| --------------- | ----- | -------------- | ---------------------- | ------- |
| Board Size      | `-s`  | `--boardsize`  | Set board size (1-36)  | 6       |
| Player 1 Type   | `-p1` | `--player1`    | Set type of player 1   | robot   |
| Player 2 Type   | `-p2` | `--player2`    | Set type of player 2   | robot   |
| Background Mode | `-b`  | `--background` | Run in background mode | false   |

### Environment Variables

The following environment variables can be used to set default values:

| Variable     | Description               |
| ------------ | ------------------------- |
| `BOARD_SIZE` | Default board size (1-36) |
| `PLAYER1`    | Default type for player 1 |
| `PLAYER2`    | Default type for player 2 |

Environment variables take precedence over command line defaults but can be overridden by explicit command line arguments.

#### Player Type Values

| Value          | Description                  |
| -------------- | ---------------------------- |
| `human`        | Human player                 |
| `robot:easy`   | GreedyRobot                  |
| `robot:medium` | ImproveGreedyRobot           |
| `robot:hard`   | SimulationRobot              |
| `robot:expert` | MonteCarloRobot              |
| `robot:master` | ParallelSearchRobot          |
| `robot`        | Equivalent to `robot:master` |

#### Example Usage

```bash
# Start with default settings
./Dots_and_Boxes

# Start with a 10x10 board
./Dots_and_Boxes --boardsize 10

# Start with human player 1 and expert robot player 2
./Dots_and_Boxes --player1 human --player2 robot:expert

# Start with medium board size and both players as robots
./Dots_and_Boxes -s 8 -p1 robot:medium -p2 robot:hard

# Start in background mode
./Dots_and_Boxes --background
```

## Project Structure

```
Dots-and-Boxes/
├── .github/            # GitHub workflows
│   └── workflows/
│       └── macos-build.yml
├── include/            # Header files
│   └── Dab/
│       ├── Board.h
│       ├── BoardSize.h
│       ├── Common.h
│       ├── Frontend.h
│       ├── Model.h
│       ├── PlayerType.h
│       ├── Robot.h
│       ├── Tools.h
│       ├── common/         # Common utility classes
│       │   ├── Array.h
│       │   ├── Iterable.h
│       │   ├── LRUCache.h
│       │   ├── List.h
│       │   ├── Queue.h
│       │   ├── Random.h
│       │   ├── Span.h
│       │   └── Vector.h
│       └── robot/          # AI implementations
│           ├── CachedRobot.h
│           ├── GreedyRobot.h
│           ├── ImproveGreedyRobot.h
│           ├── MonteCarloRobot.h
│           ├── ParallelSearchRobot.h
│           └── SimulationRobot.h
├── frontend/           # GUI-related files
│   ├── BaseCanvas.cpp
│   ├── BaseCanvas.h
│   ├── BoxCanvas.cpp
│   ├── BoxCanvas.h
│   ├── CMakeLists.txt
│   ├── DotCanvas.cpp
│   ├── DotCanvas.h
│   ├── EdgeCanvas.cpp
│   ├── EdgeCanvas.h
│   ├── Frontend.cpp
│   ├── MainWindow.cpp
│   └── MainWindow.h
├── .clang-format       # Code formatting configuration
├── .gitignore          # Git ignore file
├── CMakeLists.txt      # Build configuration
├── LICENSE             # License file
├── README.md           # This file
├── VERSION             # Version information
├── demo.png            # Game demo screenshot
└── main.cpp            # Entry point
```

## AI Strategies

1. **GreedyRobot**: Follows basic game rules without advanced planning
2. **ImproveGreedyRobot**: Implements a basic search algorithm to evaluate moves
3. **SimulationRobot**: Enhanced version with better move evaluation
4. **MonteCarloRobot**: Uses Monte Carlo simulation for move selection
5. **ParallelSearchRobot**: Leverages multi-core processing for faster search

All robot strategies use **CachedRobot** for performance optimization, which caches move evaluations using an LRU cache to reduce redundant computations.

## Technical Details

- **Language**: C++23
- **GUI Framework**: Qt6
- **Parallel Processing**: Intel TBB (Threading Building Blocks)
- **Build System**: CMake

## License

This project is licensed under the [MIT License](LICENSE).

Copyright (c) 2025 Xin Hu <202219120810@stu.cdut.edu.cn>

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgments

- Qt6 for the graphical user interface
- Intel TBB for parallel processing capabilities
