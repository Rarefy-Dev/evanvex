/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       mark                                                      */
/*    Created:      8/10/2024, 3:38:30 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    R1, L2
// PusherMotor          motor         4               
// ArmMotor             motor         3              
// ---- END VEXCODE CONFIGURED DEVICES ----

using namespace vex;

// Define the motors for the drivetrain
motor LeftDrive1Smart = motor(PORT2, ratio18_1, false);
motor RightDrive1Smart = motor(PORT1, ratio18_1, true);
drivetrain Drivetrain = drivetrain(LeftDrive1Smart, RightDrive1Smart, 319.19, 295, 40, mm, 1);

// Define the motor controlling the arm
motor ArmMotor = motor(PORT3, ratio18_1, false);

// Define the motor controlling the Pusher
motor PusherMotor = motor(PORT4, ratio18_1, false); // Update PORT4 to the correct port if needed

// Define the controller
controller Controller1 = controller();

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  while (true) {
    // Drivetrain control using the left joystick (Axis3 for forward/backward, Axis4 for turning)
    int32_t forwardBackward = Controller1.Axis3.position();
    int32_t turn = Controller1.Axis4.position();

    int32_t leftMotorSpeed = forwardBackward + turn;
    int32_t rightMotorSpeed = forwardBackward - turn;

    LeftDrive1Smart.spin(vex::directionType::fwd, leftMotorSpeed, vex::velocityUnits::pct);
    RightDrive1Smart.spin(vex::directionType::fwd, rightMotorSpeed, vex::velocityUnits::pct);

    // Arm control using the right joystick X-axis (Axis1)
    int32_t armSpeed = Controller1.Axis2.position();
    if (abs(armSpeed) > 5) {  // Apply a small deadzone to avoid unintended movement
      ArmMotor.spin(vex::directionType::fwd, armSpeed, vex::velocityUnits::pct);
    } else {
      ArmMotor.stop(hold);  // Hold the arm position when the joystick is not being moved
    }

    // Pusher control using the triggers
    if (Controller1.ButtonR2.pressing()) {
      PusherMotor.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct); // Move pusher up
    } else if (Controller1.ButtonL2.pressing()) {
      PusherMotor.spin(vex::directionType::rev, 100, vex::velocityUnits::pct); // Move pusher down
    } else {
      PusherMotor.stop(hold);  // Hold the pusher position when neither button is pressed
    }

    // Small delay to avoid excessive processing
    vex::wait(10, msec);
  }
}
