#include <redlib.h>

#include <Kernel/console.h>
#include <Kernel/uart.h>

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
static Paddle rightPaddle;
static Paddle leftPaddle;
static size_t leftScore = 0;
static size_t rightScore = 0;
static size_t pongTickCounter = 0;
static const size_t PONG_STICK_RATE = 8000;
static const size_t PADDLE_HEIGHT = 18;
static char activeInput = 0;

static uint32 pongSeed = 545463;

static uint32 pongRandom() {
    pongSeed = (pongSeed * 1103515245 + 545463) & 0x7fffffff;
    return pongSeed;
}

void test() {
    pongRandom();
    return;
}
