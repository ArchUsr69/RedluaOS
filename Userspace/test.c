#include <redlib.h>

#include <Kernel/console.h>
#include <Kernel/uart.h>

void redConsole();
void new_line();
void clear();

/*
 * Pong Game for RedluaOS
 * Features:
 * - Two-player game with directional input control
 * - Input is only enabled for the paddle the ball is moving towards
 * - Left paddle (ball moving left): W (up), S (down)
 * - Right paddle (ball moving right): O (up), K (down)
 * - 45-degree ball ricochet (always moves diagonally)
 * - Ball is a white space character
 * - Paddle height: 18 characters
 * - Score tracking (points per side wall touch)
 * - Tick-based movement (same mechanism as snake)
 */

typedef struct {
    uint16 x;
    uint16 y;
} Position;

typedef struct {
    Position pos;
    int dirX;
    int dirY;
} Ball;

typedef struct {
    Position pos;
    size_t height;
} Paddle;

static Ball ball;
static Ball lastBallPos;
static Paddle leftPaddle;
static Paddle rightPaddle;
static size_t leftScore = 0;
static size_t rightScore = 0;
static size_t pongTickCounter = 0;
static const size_t PONG_TICK_RATE = 8000;
static const size_t PADDLE_HEIGHT = 18;
static char activeInput = 0;

static uint32 pongSeed = 12345;

static uint32 pongRandom() {
    pongSeed = (pongSeed * 1103515245 + 12345) & 0x7fffffff;
    return pongSeed;
}

static uint16 pongRandomRange(uint16 min, uint16 max) {
    if (min >= max) return min;
    return min + (pongRandom() % (max - min));
}

static void pongInit() {
    ball.pos.x = Console.columns / 2;
    ball.pos.y = Console.rows / 2;
    
    uint32 rand = pongRandom();
    ball.dirX = (rand & 1) ? 1 : -1;
    ball.dirY = ((rand >> 1) & 1) ? 1 : -1;
    
    lastBallPos = ball;

    leftPaddle.pos.x = 2;
    leftPaddle.pos.y = (Console.rows - PADDLE_HEIGHT) / 2;
    leftPaddle.height = PADDLE_HEIGHT;

    rightPaddle.pos.x = Console.columns - 3;
    rightPaddle.pos.y = (Console.rows - PADDLE_HEIGHT) / 2;
    rightPaddle.height = PADDLE_HEIGHT;

    leftScore = 0;
    rightScore = 0;
    pongTickCounter = 0;
    activeInput = 0;

    pongSeed = (uint32)&ball;
}

static void pongDrawBorders() {
    for (uint16 x = 0; x < Console.columns; x++) {
        consoleWriteCharXY(Background, White, ' ', x, 0);
        consoleWriteCharXY(Background, White, ' ', x, Console.rows - 1);
    }
    for (uint16 y = 1; y < Console.rows - 1; y++) {
        consoleWriteCharXY(Background, White, ' ', 0, y);
        consoleWriteCharXY(Background, White, ' ', Console.columns - 1, y);
    }
}

static void pongClearScreen() {
    for (uint16 x = 1; x < Console.columns - 1; x++) {
        for (uint16 y = 1; y < Console.rows - 1; y++) {
            consoleWriteCharXY(Background, Background, ' ', x, y);
        }
    }
}

static void pongDrawPaddle(Paddle* paddle) {
    for (size_t i = 0; i < paddle->height; i++) {
        uint16 y = paddle->pos.y + i;
        if (y > 0 && y < Console.rows - 1) {
            consoleWriteCharXY(Background, White, ' ', paddle->pos.x, y);
        }
    }
}

static void pongErasePaddle(Paddle* paddle) {
    for (size_t i = 0; i < paddle->height; i++) {
        uint16 y = paddle->pos.y + i;
        if (y > 0 && y < Console.rows - 1) {
            consoleWriteCharXY(Background, Background, ' ', paddle->pos.x, y);
        }
    }
}

static void pongDrawBall() {
    if (ball.pos.x > 0 && ball.pos.x < Console.columns - 1 &&
        ball.pos.y > 0 && ball.pos.y < Console.rows - 1) {
        consoleWriteCharXY(Background, White, ' ', ball.pos.x, ball.pos.y);
    }
}

static void pongEraseBall() {
    if (lastBallPos.x > 0 && lastBallPos.x < Console.columns - 1 &&
        lastBallPos.y > 0 && lastBallPos.y < Console.rows - 1) {
        consoleWriteCharXY(Background, Background, ' ', lastBallPos.x, lastBallPos.y);
    }
}

static void pongDrawScore() {
    uint16 scoreX = Console.columns / 2 - 5;
    uint16 scoreY = 0;

    consoleWriteCharXY(Background, White, 'L', scoreX, scoreY);
    consoleWriteCharXY(Background, White, ':', scoreX + 1, scoreY);

    char leftDigit = '0' + (leftScore % 10);
    consoleWriteCharXY(Background, White, leftDigit, scoreX + 2, scoreY);

    consoleWriteCharXY(Background, White, 'R', scoreX + 4, scoreY);
    consoleWriteCharXY(Background, White, ':', scoreX + 5, scoreY);

    char rightDigit = '0' + (rightScore % 10);
    consoleWriteCharXY(Background, White, rightDigit, scoreX + 6, scoreY);
}

static void pongHandleInput() {
    char input = Uart.readByte();

    if (input != 0) {
        if (ball.dirX == -1) {
            if (input == 'w' || input == 'W') {
                activeInput = 1;
            } else if (input == 's' || input == 'S') {
                activeInput = 2;
            }
        } else if (ball.dirX == 1) {
            if (input == 'o' || input == 'O') {
                activeInput = 1;
            } else if (input == 'k' || input == 'K') {
                activeInput = 2;
            }
        }
    }
}

static void pongUpdatePaddles() {
    if (ball.dirX == -1) {
        if (activeInput == 1) {
            if (leftPaddle.pos.y > 1) {
                pongErasePaddle(&leftPaddle);
                leftPaddle.pos.y--;
                pongDrawPaddle(&leftPaddle);
            }
        } else if (activeInput == 2) {
            if (leftPaddle.pos.y + leftPaddle.height < Console.rows - 1) {
                pongErasePaddle(&leftPaddle);
                leftPaddle.pos.y++;
                pongDrawPaddle(&leftPaddle);
            }
        }
    } else if (ball.dirX == 1) {
        if (activeInput == 1) {
            if (rightPaddle.pos.y > 1) {
                pongErasePaddle(&rightPaddle);
                rightPaddle.pos.y--;
                pongDrawPaddle(&rightPaddle);
            }
        } else if (activeInput == 2) {
            if (rightPaddle.pos.y + rightPaddle.height < Console.rows - 1) {
                pongErasePaddle(&rightPaddle);
                rightPaddle.pos.y++;
                pongDrawPaddle(&rightPaddle);
            }
        }
    }

    activeInput = 0;
}

static void pongMoveBall() {
    pongEraseBall();
    lastBallPos = ball;

    uint16 newX = ball.pos.x + ball.dirX;
    uint16 newY = ball.pos.y + ball.dirY;

    if (newY <= 1) {
        newY = 2;
        ball.dirY = 1;
    } else if (newY >= Console.rows - 2) {
        newY = Console.rows - 3;
        ball.dirY = -1;
    }

    if (newX == leftPaddle.pos.x) {
        if (newY >= leftPaddle.pos.y &&
            newY < leftPaddle.pos.y + leftPaddle.height) {
            newX = leftPaddle.pos.x + 1;
            ball.dirX = 1;
        }
    } else if (newX == rightPaddle.pos.x) {
        if (newY >= rightPaddle.pos.y &&
            newY < rightPaddle.pos.y + rightPaddle.height) {
            newX = rightPaddle.pos.x - 1;
            ball.dirX = -1;
        }
    }

    if (newX <= 0) {
        rightScore++;
        pongInit();
        pongClearScreen();
        pongDrawBorders();
        pongDrawPaddle(&leftPaddle);
        pongDrawPaddle(&rightPaddle);
        pongDrawScore();
        pongDrawBall();
        return;
    }

    if (newX >= Console.columns - 1) {
        leftScore++;
        pongInit();
        pongClearScreen();
        pongDrawBorders();
        pongDrawPaddle(&leftPaddle);
        pongDrawPaddle(&rightPaddle);
        pongDrawScore();
        pongDrawBall();
        return;
    }

    ball.pos.x = newX;
    ball.pos.y = newY;

    pongDrawBall();
}

void test() {
    pongInit();
    pongClearScreen();
    pongDrawBorders();

    pongDrawPaddle(&leftPaddle);
    pongDrawPaddle(&rightPaddle);

    pongDrawBall();
    pongDrawScore();

    while (true) {
        pongHandleInput();
        pongUpdatePaddles();

        pongTickCounter++;
        if (pongTickCounter >= PONG_TICK_RATE) {
            pongTickCounter = 0;
            pongMoveBall();
            pongDrawScore();
        }
    }
}
