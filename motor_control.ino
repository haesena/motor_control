// Step pins
#define X_STP              2
#define Y_STP              3
#define Z_STP              4

// Direction pins
#define X_DIR              5
#define Y_DIR              6
#define Z_DIR              7

// Enable pin
#define EN                 8

// Limit pins
#define X_LIM              9
#define Y_LIM              10
#define Z_LIM              11


#include "Motor.h"

float one_turn = 200*32;

// current position
float current_pos[3];
Motor* x_motor;
Motor* y_motor;
Motor* z_motor;

/**
 * set up the motors
 */
void motor_setup() {
	x_motor = new Motor(X_STP, X_DIR, EN, X_LIM);
	y_motor = new Motor(Y_STP, Y_DIR, EN, Y_LIM);
	z_motor = new Motor(Z_STP, Z_DIR, EN, Z_LIM);
}

/**
 * set the position
 */
void position(float new_x, float new_y, float new_z) {
	current_pos[0] = new_x;
	current_pos[1] = new_y;
	current_pos[2] = new_z;

	where();
}

void output(String code, float val) {
	Serial.print(code);
	Serial.print(val);
	Serial.print(" ");
}

void where() {
	output("X", current_pos[0]);
	output("Y", current_pos[1]);
	output("Z", current_pos[2]);
	Serial.println("");
}

void home_all() {
	// first we home the z axis, because if it's not centered, we can not move in x- and y-directions
	z_motor->home();
	Serial.println("z-motor homed");

	// now home x and y axes
	Motor::home_2d(x_motor, y_motor);
	Serial.println("x- and y-motors homed");
}

void setup() {

	Serial.begin(115200);

	motor_setup();
}

void loop() {
	Serial.println("");
	Serial.println("---- START ----");
	Serial.println("");

//	Serial.println("X10 Y10");
//
//	x_motor->setDirection(CW);
//	y_motor->setDirection(CW);
//	Motor::step_2d(x_motor, y_motor, (float)10 * (float)one_turn, (float)10 * (float)one_turn);
//
//	Serial.println("Z5");
//	z_motor->setDirection(CCW);
//	z_motor->step((float)5 * (float)one_turn);
//
//	Serial.println("X2 Y20");
//	x_motor->setDirection(CCW);
//	y_motor->setDirection(CCW);
//	Motor::step_2d(x_motor, y_motor, (float)2 * (float)one_turn, (float)20 * (float)one_turn);

	home_all();

	digitalWrite(EN, HIGH);

	while(true) {
	}

}
