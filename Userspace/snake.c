#include <redlib.h>

#include <Kernel/console.h>
#include <Kernel/uart.h>

/*
 * Simple Snake Game for RedluaOS
 * Uses the entire screen with white space characters
 * Features:
 * - Tick-based movement and input handling
 * - Random apple spawning
 * - Collision detection (walls and self)
 * - Score tracking
 * - Optimized rendering (only updates changed tiles)
 */

#define SNAKE_MAX_LENGTH 1024

typedef struct {
    uint16 x;
    uint16 y;
} Position;

typedef struct {
    Position body[SNAKE_MAX_LENGTH];
    size_t length;
    int dirX;
    int dirY;
    int nextDirX;
    int nextDirY;
} Snake;

typedef struct {
    Position pos;
    char spawned;
} Apple;

static Snake snake;
static Apple apple;
static Apple lastApple;
static size_t score = 0;
static size_t tickCounter = 0;
static const size_t TICK_RATE = 5000;  // Cycles per movement

/*
 * Simple pseudo-random number generator
 * Uses a basic LCG algorithm
 */
static uint32 seed = 12345;

uint32 random() {
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return seed;
}

uint16 randomRange(uint16 min, uint16 max) {
    if (min >= max) return min;
    return min + (random() % (max - min));
}

/*
 * Initialize the snake game
 */
void snakeInit() {
    // Initialize snake in the middle
    snake.length = 3;
    snake.body[0].x = Console.columns / 2;
    snake.body[0].y = Console.rows / 2;
    snake.body[1].x = snake.body[0].x - 1;
    snake.body[1].y = snake.body[0].y;
    snake.body[2].x = snake.body[0].x - 2;
    snake.body[2].y = snake.body[0].y;

    snake.dirX = 1;
    snake.dirY = 0;
    snake.nextDirX = 1;
    snake.nextDirY = 0;

    apple.spawned = 0;
    lastApple.spawned = 0;
    score = 0;
    tickCounter = 0;

    // Seed the random generator with a somewhat unique value
    seed = (uint32)&snake;
}

/*
 * Draw borders once at game start
 */
void drawBorders() {
    for (uint16 x = 0; x < Console.columns; x++) {
        consoleWriteCharXY(Background, White, ' ', x, 0);
        consoleWriteCharXY(Background, White, ' ', x, Console.rows - 1);
    }
    for (uint16 y = 1; y < Console.rows - 1; y++) {
        consoleWriteCharXY(Background, White, ' ', 0, y);
        consoleWriteCharXY(Background, White, ' ', Console.columns - 1, y);
    }
}

/*
 * Clear the screen (fill with spaces)
 */
void clearScreen() {
    for (uint16 y = 1; y < Console.rows - 1; y++) {
        for (uint16 x = 1; x < Console.columns - 1; x++) {
            consoleWriteCharXY(Background, Background, ' ', x, y);
        }
    }
}

/*
 * Spawn a new apple at a random location
 */
void spawnApple() {
    Position newPos;
    char collision;

    do {
        collision = 0;
        newPos.x = randomRange(1, Console.columns - 1);
        newPos.y = randomRange(1, Console.rows - 1);

        // Check if apple spawns on snake
        for (size_t i = 0; i < snake.length; i++) {
            if (snake.body[i].x == newPos.x && snake.body[i].y == newPos.y) {
                collision = 1;
                break;
            }
        }
    } while (collision);

    apple.pos = newPos;
    apple.spawned = 1;
}

/*
 * Draw only the apple (called when apple changes)
 */
void updateApple() {
    // Erase old apple if it existed
    if (lastApple.spawned) {
        consoleWriteCharXY(Background, Background, ' ', lastApple.pos.x, lastApple.pos.y);
    }

    // Draw new apple
    if (apple.spawned) {
        consoleWriteCharXY(Background, Green, ' ', apple.pos.x, apple.pos.y);
        lastApple = apple;
    }
}

/*
 * Handle input and update direction
 */
void handleInput() {
    char input = Uart.readByte();

    if (input != 0) {
        // Arrow keys or WASD controls
        if (input == 'w' || input == 'W') {
            if (snake.dirY != 1) {
                snake.nextDirX = 0;
                snake.nextDirY = -1;
            }
        } else if (input == 's' || input == 'S') {
            if (snake.dirY != -1) {
                snake.nextDirX = 0;
                snake.nextDirY = 1;
            }
        } else if (input == 'a' || input == 'A') {
            if (snake.dirX != 1) {
                snake.nextDirX = -1;
                snake.nextDirY = 0;
            }
        } else if (input == 'd' || input == 'D') {
            if (snake.dirX != -1) {
                snake.nextDirX = 1;
                snake.nextDirY = 0;
            }
        }
    }
}

/*
 * Move the snake and handle collisions
 */
void moveSnake() {
    // Update direction
    snake.dirX = snake.nextDirX;
    snake.dirY = snake.nextDirY;

    // Calculate new head position
    Position newHead;
    newHead.x = snake.body[0].x + snake.dirX;
    newHead.y = snake.body[0].y + snake.dirY;

    // Check wall collision
    if (newHead.x <= 0 || newHead.x >= Console.columns - 1 ||
        newHead.y <= 0 || newHead.y >= Console.rows - 1) {
        // Clear old snake before reset
        for (size_t i = 0; i < snake.length; i++) {
            consoleWriteCharXY(Background, Background, ' ', snake.body[i].x, snake.body[i].y);
        }
        snakeInit();
        clearScreen();
        return;
    }

    // Check self collision
    for (size_t i = 0; i < snake.length; i++) {
        if (snake.body[i].x == newHead.x && snake.body[i].y == newHead.y) {
            // Clear old snake before reset
            for (size_t j = 0; j < snake.length; j++) {
                consoleWriteCharXY(Background, Background, ' ', snake.body[j].x, snake.body[j].y);
            }
            snakeInit();
            clearScreen();
            return;
        }
    }

    // Get the old tail position before we move
    Position oldTail;
    oldTail.x = snake.body[snake.length - 1].x;
    oldTail.y = snake.body[snake.length - 1].y;

    // Check apple collision
    char appleEaten = 0;
    if (apple.spawned && apple.pos.x == newHead.x && apple.pos.y == newHead.y) {
        score += 10;
        apple.spawned = 0;
        appleEaten = 1;
        // Grow snake (don't remove tail)
        if (snake.length < SNAKE_MAX_LENGTH) {
            snake.length++;
        }
    }

    // Shift body segments (move from tail to head)
    for (size_t i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }

    // Add new head
    snake.body[0] = newHead;

    // Draw new head
    consoleWriteCharXY(Background, White, ' ', snake.body[0].x, snake.body[0].y);

    // Erase old tail (only if snake didn't eat apple)
    if (!appleEaten) {
        consoleWriteCharXY(Background, Background, ' ', oldTail.x, oldTail.y);
    }

    // Spawn new apple if needed
    if (!apple.spawned) {
        spawnApple();
        updateApple();
    }
}

/*
 * Main snake game function
 */
void snake() {
    snakeInit();
    clearScreen();
    drawBorders();
    spawnApple();
    updateApple();

    // Draw initial snake
    for (size_t i = 0; i < snake.length; i++) {
        consoleWriteCharXY(Background, White, ' ', snake.body[i].x, snake.body[i].y);
    }

    while (true) {
        // Non-blocking input check every cycle
        handleInput();

        // Tick-based movement
        tickCounter++;
        if (tickCounter >= TICK_RATE) {
            tickCounter = 0;
            moveSnake();
        }
    }
}
