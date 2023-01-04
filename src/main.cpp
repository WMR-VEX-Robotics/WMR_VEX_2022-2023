/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Carlo and Erik                                            */
/*    Created:      Tue Sep 27 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <cmath>

using namespace vex;

// A global instance of competition
competition Competition;

/// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor LeftFront = motor(PORT1, ratio6_1, true);
motor LeftRear = motor(PORT5, ratio6_1, false);
motor RightFront = motor(PORT3, ratio6_1, false);
motor RightRear = motor(PORT4, ratio6_1, true);
motor Spin = motor(PORT5, ratio6_1, true);
motor Launch = motor(PORT6, ratio18_1, true);
pneumatics P1 = pneumatics(Brain.ThreeWirePort.F);

// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
int RobotReverseVariable = 1;
bool manual = false;
int amount = 0;

void StopAllChasis() {
  LeftFront.stop(hold);
  LeftRear.stop(hold);
  RightFront.stop(hold);
  RightRear.stop(hold);
}

void SpinUp(int per){
  Spin.spin(forward, per, percent);
}
void print(int per, bool man){
  Controller1.Screen.clearScreen();
  Controller1.Screen.print("%i%", per);
  Controller1.Screen.newLine();
  Controller1.Screen.print("%s", manual ? "true" : "false");
}

void pre_auton(void) {
  
}

void autonomous(void) {

}

void usercontrol(void) {
  while (1) {
    LeftFront.spin(forward, (Controller1.Axis3.position() * RobotReverseVariable) + Controller1.Axis1.position(), percent);
    RightFront.spin(forward, (Controller1.Axis3.position() * RobotReverseVariable) - Controller1.Axis1.position(), percent);
    LeftRear.spin(forward, (Controller1.Axis3.position() * RobotReverseVariable), percent);
    RightRear.spin(forward, (Controller1.Axis3.position() * RobotReverseVariable), percent);
    if(Controller1.Axis3.value() == 0 && Controller1.Axis1.value() == 0) {
      StopAllChasis();
    }
    if (Controller1.ButtonR1.pressing()){
      if(manual){
        manual = false;
      }
      manual = true;
      wait(250, msec);
    }
    if (Controller1.ButtonL1.pressing()) {
      if (RobotReverseVariable == 1) {
        RobotReverseVariable = -1;
      }
        RobotReverseVariable = 1;
      wait(250, msec);
      print(amount, manual);
    }
    if(Controller1.ButtonA.pressing()){
      SpinUp(100);
      amount = 100;
      print(amount, manual);
    }
    if(Controller1.ButtonB.pressing()){
      SpinUp(75);
      amount = 75;
      print(amount, manual);
    }
    if(Controller1.ButtonX.pressing()){
      SpinUp(50);
      amount = 50;
      print(amount, manual);
    }
    if(Controller1.ButtonY.pressing()){
      SpinUp(25);
      amount = 25;
      print(amount, manual);
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