/*
 * SimpleMotor.h
 *
 *  Created on: 27.02.2018
 *      Author: haesen
 */

#include "Arduino.h"

#ifndef MOTOR_H_
#define MOTOR_H_

// Motor directions
#define CW                 0
#define CCW                1

class Motor {
public:
	Motor(byte step, byte dir, byte enable, byte limit);
	// getters & setters
	void setEnable(bool e);
	void setDirection(byte dir);
	void setDelay(double d);
	double getDelay();

	// stepping functions
	void step(float steps);
	static void step_2d(Motor* motor_1, Motor* motor_2, float steps1, float steps2);

	// homing functions
	void home();
	static void home_2d(Motor* motor_1, Motor* motor_2);

private:
	byte step_pin;
	byte dir_pin;
	byte enable_pin;
	byte limit_pin;

	bool enabled = false;
	byte direction = 0;

	double delay;

	void singleStep();
	bool limitEngaged();
};

#endif /* MOTOR_H_ */
