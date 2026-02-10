#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "initial_configs.h"

static const int WIDTH = 1200;
static const int HEIGHT = 700;
static const char *TITLE = "Conway's Game of Life";
static const int CELL_SIZE = 20;
static const int NUM_ROWS = (HEIGHT / CELL_SIZE);
static const int NUM_COLS = (WIDTH / CELL_SIZE);
static sf::Color CELL_COLOR = sf::Color::White;
static sf::Color BACKGROUND_COLOR = sf::Color::Black;
static const int GENERATION_DELAY_MS = 500;
static int dxs[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                        {0, 1},   {1, -1}, {1, 0},  {1, 1}};
static int generation = 0;
static const sf::Font jetbrainsMonoNerd("JetBrainsMonoNerdFont-Light.ttf");

#define INBOUNDS(i, j) (i >= 0 and i < NUM_ROWS and j >= 0 and j < NUM_COLS)
#define COLOR_GRAY sf::Color(179, 169, 168)

static sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), TITLE, sf::Style::Titlebar | sf::Style::Close);

enum class CellState { ALIVE = 0, DEAD };

static std::array<std::array<CellState, NUM_COLS>, NUM_ROWS> board;

static void placeCell(size_t i, size_t j) {
  assert(INBOUNDS(i, j));

  board[i][j] = CellState::ALIVE;
}
static void eraseCell(size_t i, size_t j) {
  assert(INBOUNDS(i, j));

  board[i][j] = CellState::DEAD;
}

static void initBoard() {
  for (size_t i = 0; i < NUM_ROWS; i++) {
    for (size_t j = 0; j < NUM_COLS; j++) {
      board[i][j] = CellState::DEAD;
    }
  }

  // Decide initial configuration
  int i = rand() % NUM_ROWS;
  int j = rand() % NUM_COLS;

  auto pos = applyOffsets((int *)offsets::f_pentonimo,
                          sizeof(offsets::f_pentonimo) / sizeof(int), i, j);

  for (const auto &[_i, _j] : pos) {
    if (!INBOUNDS(_i, _j)) {
      initBoard();
      break;
    }
    board[_i][_j] = CellState::ALIVE;
  }
}

static void drawBoard() {
  for (size_t i = 0; i < NUM_ROWS; i++) {
    for (size_t j = 0; j < NUM_COLS; j++) {
      CellState state = board[i][j];

      if (state == CellState::ALIVE) {
        sf::RectangleShape rect({CELL_SIZE, CELL_SIZE});
        rect.setPosition({i * CELL_SIZE * 1.0f, j * CELL_SIZE * 1.0f});
        rect.setFillColor(CELL_COLOR);

        window.draw(rect);
      }
    }
  }
}

static int getAliveNeighbours(size_t i, size_t j) {
  assert(INBOUNDS(i, j));

  int ans = 0;
  for (const auto dx : dxs) {
    size_t new_i = i + dx[0];
    size_t new_j = j + dx[1];

    if (new_i >= 0 and new_i < NUM_ROWS && new_j >= 0 && new_j < NUM_COLS &&
        board[new_i][new_j] == CellState::ALIVE) {
      ans++;
    }
  }
  return ans;
}

static void drawGenerationLabel() {
	generation++;
	std::string label = "generation: " + std::to_string(generation);
	sf::Text generationLabel(jetbrainsMonoNerd, label);
	generationLabel.setCharacterSize(16);
	generationLabel.setPosition({20.0f, 20.0f});
	window.draw(generationLabel);
}

// Processes 1 generation
static void generate() {
  // decide which cells need change i.e. flipped
  std::vector<std::pair<int, int>> changedCells;
  for (size_t i = 0; i < NUM_ROWS; i++) {
    for (size_t j = 0; j < NUM_COLS; j++) {
      int aliveNeighbours = getAliveNeighbours(i, j);

      if (aliveNeighbours < 2 or aliveNeighbours > 3) {
        if (board[i][j] != CellState::DEAD)
          changedCells.push_back({i, j});
      } else {
        if (aliveNeighbours == 3) {
          if (board[i][j] != CellState::ALIVE)
            changedCells.push_back({i, j});
        }
      }
    }
  }

  // update the board
  for (const auto &[i, j] : changedCells) {
    if (board[i][j] == CellState::ALIVE)
      board[i][j] = CellState::DEAD;
    else
      board[i][j] = CellState::ALIVE;
  }
}

static void drawGrids() {
	// row lines
	for (int i = 0; i < NUM_ROWS; i++) {
		sf::RectangleShape rect({WIDTH, 1});
		rect.setPosition({0, i * CELL_SIZE * 1.0f});
		rect.setFillColor(COLOR_GRAY);
		window.draw(rect);
	}
	// column lines
	for (int i = 0; i < NUM_COLS; i++) {
		sf::RectangleShape rect({1, HEIGHT});
		rect.setPosition({i * CELL_SIZE * 1.0f, 0});
		rect.setFillColor(COLOR_GRAY);
		window.draw(rect);
	}
}

int main() {
  srand(time(0));
  initBoard();

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    window.clear(BACKGROUND_COLOR);

    generate();
		drawGrids();
		drawGenerationLabel();
    drawBoard();

    window.display();

    std::this_thread::sleep_for(std::chrono::milliseconds(GENERATION_DELAY_MS));
  }
}
