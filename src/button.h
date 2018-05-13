#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
   public:
	Button(uint8_t pin) : m_pin(pin) {
	}
	void update() {
		m_justPressed = false;

		bool newstate = (digitalRead(m_pin) == 0);
		if (!m_state && newstate) {
			m_justPressed = true;
		}
		m_state = newstate;
	}

	bool isPressed() {
		return m_state;
	}

	bool justPressed() {
		return m_justPressed;
	}

   private:
	uint8_t m_pin;
	bool m_state = false;
	bool m_justPressed = false;
};

#endif /* BUTTON_H */
