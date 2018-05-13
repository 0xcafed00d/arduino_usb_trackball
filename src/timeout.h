#ifndef ARDUINO_RFID_MUSICPLAYER_UTILS_H
#define ARDUINO_RFID_MUSICPLAYER_UTILS_H

#include "Arduino.h"

class TimeOut {
	uint32_t interval;
	uint32_t start;

   public:
	TimeOut(uint32_t timeoutms = 0) : interval(timeoutms) {
		start = millis();
	}

	bool hasTimedOut() const {
		return (millis() - start) > interval;
	}
};

#endif  // ARDUINO_RFID_MUSICPLAYER_UTILS_
