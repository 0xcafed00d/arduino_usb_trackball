#include <Arduino.h>
#include <Mouse.h>

static const int PIN_X1 = 2;
static const int PIN_X2 = 8;
static const int PIN_Y1 = 3;
static const int PIN_Y2 = 9;

static const int32_t xScale = 10000;
static const int32_t yScale = 10000;

static const int xInverted = 0;
static const int yInverted = 0;

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
}

void loop() {
	delay(20);

	noInterrupts();
	int32_t x = xcount;
	int32_t y = ycount;
	xcount = 0;
	ycount = 0;
	interrupts();

	if (x != 0 || y != 0)
		Mouse.move(x / xScale, y / yScale);
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
