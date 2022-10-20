/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       23_clichtenberger                                         */
/*    Created:      Tue Sep 27 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

/// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor LeftFront = motor(PORT1, ratio36_1, true);
motor LeftRear = motor(PORT5, ratio36_1, false);
motor RightFront = motor(PORT3, ratio36_1, false);
motor RightRear = motor(PORT4, ratio36_1, true);
pneumatics P1 = pneumatics(Brain.ThreeWirePort.F);

// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
int RobotReverseVariable = 1;

void RobotReverse() {
  if (RobotReverseVariable == 1) {
    RobotReverseVariable = -1;
    Brain.Screen.print(RobotReverseVariable);
    wait(250, msec);
    Brain.Screen.clearLine();
    return;
  };
  if (RobotReverseVariable == -1) {
    RobotReverseVariable = 1;
    Brain.Screen.print(RobotReverseVariable);
    wait(250, msec);
    Brain.Screen.clearLine();
    return;
  };
};

void StopAllChasis() {
  LeftFront.stop(hold);
  LeftRear.stop(hold);
  RightFront.stop(hold);
  RightRear.stop(hold);
}

void pre_auton(void) {

}

void autonomous(void) {

}

void usercontrol(void) {
  Brain.Screen.clearLine();
  while (1) {
    LeftFront.spin(forward, (Controller1.Axis3.position() * RobotReverseVariable) + Controller1.Axis1.position(), percent);
    RightFront.spin(forward, (Controller1.Axis3.position() * RobotReverseVariable) - Controller1.Axis1.position(), percent);
    LeftRear.spin(forward, (Controller1.Axis3.position() * RobotReverseVariable), percent);
    RightRear.spin(forward, (Controller1.Axis3.position() * RobotReverseVariable), percent);
    if(Controller1.Axis3.value() == 0 && Controller1.Axis1.value() == 0) {
      StopAllChasis();
    }
    if (Controller1.ButtonL1.pressing()) {
      RobotReverse();
    }
  }
  wait(20, msec);
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();
  while (true) {
    wait(100, msec);
  }
}