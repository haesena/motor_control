/*
 * SimpleMotor.cpp
 *
 *  Created on: 27.02.2018
 *      Author: haesen
 */

#include "Motor.h"

Motor::Motor(byte step, byte dir, byte enable, byte limit) :
step_pin{step}, dir_pin{dir}, enable_pin{enable}, limit_pin{limit} {
	pinMode(step_pin, OUTPUT);
	pinMode(dir_pin, OUTPUT);
	pinMode(enable_pin, OUTPUT);
	pinMode(limit_pin, INPUT_PULLUP);

	delay = 0.03;

	// clockwise direction
	digitalWrite(dir_pin, HIGH);
}

void Motor::setEnable(bool e) {
	enabled = e;
	digitalWrite(enable_pin, e ? LOW : HIGH);
}

void Motor::setDirection(byte d) {
	if(d != CW && d != CCW) {
		return;
	}

	digitalWrite(dir_pin, d == CW ? HIGH : LOW);
	direction = d;
}

void Motor::setDelay(double d) {
	delay = d;
}

double Motor::getDelay() {
	return delay;
}

bool Motor::limitEngaged() {
	return digitalRead(limit_pin) == 0;
}

void Motor::singleStep() {
	digitalWrite(step_pin, HIGH);
	digitalWrite(step_pin, LOW);
}

/**
 * Function to home a single motor axis. The motor is first driver clockwise until the limit
 * switch is no longer engaged, and then brought back to the limit
 */
void Motor::home() {
	// first we turn clockwise in case the position is negative (limit switch engaged)
	this->setDirection(CW);

	while(this->limitEngaged()) {
		// while the limit switch is engaged, the position is negative so turn a whole rotation clockwise
		for(int i = 0; i < 6400; i++) {
			this->singleStep();
			delayMicroseconds(delay * 1000);
		}
	}

	// now we can turn counter-clockwise till the limit switch is engaged again
	this->setDirection(CCW);

	while(!this->limitEngaged()) {
		this->singleStep();
		delayMicroseconds(delay * 1000);
	}
}

/**
 * Static function to home two motor-axes simultaneously
 */
void Motor::home_2d(Motor* motor_1, Motor* motor_2) {
	// direction for homing is counter-clockwise
	motor_1->setDirection(CCW);
	motor_2->setDirection(CCW);

	// both motors are homed simultaneously, so we need to keep track of the homed axes
	bool homed1 = false;
	bool homed2 = false;

	while(!homed1 || !homed2) {
		if(motor_1->limitEngaged()) {
			// if limit switch for x is engaged, x is homed
			homed1 = true;
		} else {
			// else we step the motor one step further to home
			motor_1->singleStep();
		}

		if(motor_2->limitEngaged()) {
			// if limit switch for y is engaged, y is homed
			homed2 = true;
		} else {
			// else we step the motor one step further to home
			motor_2->singleStep();
		}

		// delay for stepping
		delayMicroseconds(motor_1->getDelay() * 1000);
	}
}

void Motor::step(float steps) {
	for(float i = 0; i < steps; i++) {
		this->singleStep();
		delayMicroseconds(delay * 1000);
	}
}

void Motor::step_2d(Motor* motor_1, Motor* motor_2, float steps1, float steps2) {
	float max_steps, min_steps, s1, s2, y_delta, y_cc;
	Motor *m1, *m2;
	double d;
	y_cc = s1 = s2 = 0;

	if(steps1 > steps2) {
		max_steps = steps1;
		min_steps = steps2;
		m1 = motor_1;
		m2 = motor_2;
		d = m1->getDelay();
	} else {
		max_steps = steps2;
		min_steps = steps1;
		m1 = motor_2;
		m2 = motor_1;
		d = m2->getDelay();
	}

	y_delta = (floor)(max_steps/min_steps);

	for(float i = 0; i < max_steps; i++) {
		s1++;
		y_cc++;

		if(y_cc >= y_delta) {
			y_cc = 0;
			if(s2 < min_steps) {
				s2++;
				m2->singleStep();

			}
		}

		m1->singleStep();

		delayMicroseconds(d * 1000);
	}

	for(float i = 0; i < (min_steps - s2); i++) {
		m2->singleStep();
		s2++;
		delayMicroseconds(d * 1000);
	}
}
