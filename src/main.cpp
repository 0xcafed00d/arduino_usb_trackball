#include <Arduino.h>
#include <Mouse.h>

static const int PIN_X1 = 2;
static const int PIN_X2 = 8;
static const int PIN_Y1 = 3;
static const int PIN_Y2 = 9;

void xAxisISR();
void yAxisISR();

volatile int32_t xcount = 0;
volatile int32_t ycount = 0;

void setup() {
	while (!Serial) {
	}
	Mouse.begin();

	Serial.begin(115200);

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
		Mouse.move(x / 10000, y / 10000);
}

void xAxisISR() {
	static uint32_t lastMS = 0;
	uint32_t now = micros();
	uint32_t val = now - lastMS;
	lastMS = now;

	if (val < 100000) {
		val = 100000 - val;
		if (digitalRead(PIN_X2))
			xcount -= val;
		else
			xcount += val;
	}
}

void yAxisISR() {
	static uint32_t lastMS = 0;
	uint32_t now = micros();
	uint32_t val = now - lastMS;
	lastMS = now;

	if (val < 100000) {
		val = 100000 - val;

		if (digitalRead(PIN_Y2))
			ycount -= val;
		else
			ycount += val;
	}
}
