#include <Arduino.h>
#include <Mouse.h>

// ==================================================================
//   Config settings
// ==================================================================

static const int PIN_X1 = 2;       // white
static const int PIN_X2 = 4;       // yellow
static const int PIN_Y1 = 3;       // purple
static const int PIN_Y2 = 5;       // brown
static const int PIN_BUTTON1 = 9;  // orange
static const int PIN_BUTTON2 = 8;  // green

static const int32_t xScale = 8000;
static const int32_t yScale = 8000;

static const int xInverted = 0;
static const int yInverted = 0;

static const int updateRate = 50;

// ==================================================================

void xAxisISR();
void yAxisISR();

volatile int32_t xcount = 0;
volatile int32_t ycount = 0;

void setup() {
	Mouse.begin();

	attachInterrupt(digitalPinToInterrupt(PIN_X1), xAxisISR, RISING);
	attachInterrupt(digitalPinToInterrupt(PIN_Y1), yAxisISR, RISING);
	pinMode(PIN_X2, INPUT);
	pinMode(PIN_Y2, INPUT);
	pinMode(PIN_BUTTON1, INPUT_PULLUP);
	pinMode(PIN_BUTTON2, INPUT_PULLUP);
}

void checkMouseButton(int pin, int button) {
	if (digitalRead(pin) == 0) {
		if (!Mouse.isPressed(button)) {
			Mouse.press(button);
		}
	} else {
		if (Mouse.isPressed(button)) {
			Mouse.release(button);
		}
	}
}

void loop() {
	delay(1000 / updateRate);

	noInterrupts();
	int32_t x = xcount;
	int32_t y = ycount;
	xcount = 0;
	ycount = 0;
	interrupts();

	if (x != 0 || y != 0)
		Mouse.move(x / xScale, y / yScale);

	checkMouseButton(PIN_BUTTON1, MOUSE_LEFT);
	checkMouseButton(PIN_BUTTON2, MOUSE_RIGHT);
}

void updateAxis(uint32_t& last, volatile int32_t& count, int pin, int inverted) {
	uint32_t now = micros();
	uint32_t val = now - last;
	last = now;

	if (val < 100000) {
		val = 100000 - val;

		if (digitalRead(pin) == inverted)
			count += val;
		else
			count -= val;
	}
}

void xAxisISR() {
	static uint32_t last = 0;
	updateAxis(last, xcount, PIN_X2, xInverted);
}

void yAxisISR() {
	static uint32_t last = 0;
	updateAxis(last, ycount, PIN_Y2, yInverted);
}
