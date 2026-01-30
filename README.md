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

## Requirements

- C++17 compatible compiler
- Qt6 (Core, Gui, Widgets)
- CMake 3.16 or higher
- OpenMP (for parallel processing)

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

| Option | Short | Long | Description | Default |
|--------|-------|------|-------------|---------|
| Board Size | `-s` | `--size` | Set board size ranging from [1, 36] | 6 |
| Player 1 Type | `-p1` | `--player1` | Set type of player 1 | robot |
| Player 2 Type | `-p2` | `--player2` | Set type of player 2 | robot |

#### Player Type Values

| Value | Description |
|-------|-------------|
| `human` | Human player |
| `robot:easy` | GreedyRobot |
| `robot:medium` | ImproveGreedyRobot |
| `robot:hard` | SimulationRobot |
| `robot:expert` | MonteCarloRobot |
| `robot:master` | ParallelSearchRobot |
| `robot` | Equivalent to `robot:master` |

#### Example Usage

```bash
# Start with default settings
./Dots_and_Boxes

# Start with a 10x10 board
./Dots_and_Boxes --size 10

# Start with human player 1 and expert robot player 2
./Dots_and_Boxes --player1 human --player2 robot:expert

# Start with medium board size and both players as robots
./Dots_and_Boxes -s 8 -p1 robot:medium -p2 robot:hard
```

## Project Structure

```
Dots-and-Boxes/
├── .github/            # GitHub workflows
│   └── workflows/
│       └── macos-build.yml
├── frontend/           # GUI-related files
│   ├── BoxCanvas.hpp   # Canvas for drawing boxes
│   ├── CommandParser.hpp # Command line argument parser
│   ├── Common.hpp      # Common frontend utilities
│   ├── DotCanvas.hpp   # Canvas for drawing dots
│   ├── EdgeCanvas.hpp  # Canvas for drawing edges
│   ├── MainWindow.hpp  # Main window implementation
│   └── MainWindowCreator.hpp # Main window creator
├── include/            # Header files
│   ├── Board.hpp
│   ├── Common.hpp
│   ├── Frontend.hpp
│   ├── Model.hpp
│   └── Robot.hpp
├── src/                # Core game logic
│   ├── board/          # Board implementations
│   │   ├── BasicBoard.hpp
│   │   ├── EdgeCountableBoard.hpp
│   │   ├── ScoreCountableBoard.hpp
│   │   └── ScoreableEdgeBoard.hpp
│   ├── common/         # Utility classes
│   │   ├── Array.hpp
│   │   ├── Iterable.hpp
│   │   ├── List.hpp
│   │   ├── Queue.hpp
│   │   ├── Random.hpp
│   │   └── Span.hpp
│   ├── model/          # Game model
│   │   ├── Edge.hpp
│   │   ├── EdgeCounter.hpp
│   │   ├── GameScoreMap.hpp
│   │   ├── Iota.hpp
│   │   ├── NearBoxes.hpp
│   │   ├── NearEdges.hpp
│   │   ├── SearchScoreMap.hpp
│   │   ├── Square.hpp
│   │   ├── Step.hpp
│   │   ├── Turn.hpp
│   │   └── Types.hpp
│   └── robot/          # AI implementations
│       ├── GreedyRobot.hpp
│       ├── ImproveGreedyRobot.hpp
│       ├── MonteCarloRobot.hpp
│       ├── ParallelSearchRobot.hpp
│       ├── PlayerType.hpp
│       ├── Robot.hpp
│       └── SimulationRobot.hpp
├── main.cpp            # Entry point
├── CMakeLists.txt      # Build configuration
├── LICENSE             # License file
├── README.md           # This file
└── demo.png            # Game demo screenshot
```

## AI Strategies

1. **GreedyRobot**: Follows basic game rules without advanced planning
2. **ImproveGreedyRobot**: Implements a basic search algorithm to evaluate moves
3. **SimulationRobot**: Enhanced version with better move evaluation
4. **MonteCarloRobot**: Uses Monte Carlo simulation for move selection
5. **ParallelSearchRobot**: Leverages multi-core processing for faster search

## Technical Details

- **Language**: C++17
- **GUI Framework**: Qt6
- **Parallel Processing**: OpenMP
- **Build System**: CMake

## License

This project is licensed under the [MIT License](LICENSE).

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgments

- Qt6 for the graphical user interface
- OpenMP for parallel processing capabilities
