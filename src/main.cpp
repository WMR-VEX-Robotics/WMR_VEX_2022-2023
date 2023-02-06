/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Carlo, Jacob, and Erik                                    */
/*    Created:      Tue Sep 27 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <cmath>

using namespace vex;

// A global instance of competition
competition Competition;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor LeftFront = motor(PORT8, ratio18_1, false);
motor LeftRear = motor(PORT7, ratio18_1, false);
motor RightFront = motor(PORT10, ratio18_1, true);
motor RightRear = motor(PORT9, ratio18_1, true);
motor Vacuum = motor(PORT20, ratio18_1, true);
motor Launch = motor(PORT19, ratio18_1, true);
motor Flywheel1 = motor(PORT4, ratio6_1, true);
motor Flywheel2 = motor(PORT5, ratio6_1, false);
pneumatics P1 = pneumatics(Brain.ThreeWirePort.F);

// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
bool release = false;
int RobotLaunchVariable = 7;
int auton = 0;
int AutonMin = 0;
int AutonMax = 6;
int length = 0;
int lengthSum = 0;
int lengthAvg = 0;
int startTime = 0;


// ***************** DRIVE ********************

void StopAllChasis(void) {
  LeftFront.stop(hold);
  LeftRear.stop(hold);
  RightFront.stop(hold);
  RightRear.stop(hold);
}

// **************** VACUUM ********************

void useForwardVacuum(int percent){
  Vacuum.spin(forward, percent, pct);
}

void useReverseVacuum(int percent){
  Vacuum.spin(reverse, percent, pct);
}

// *************** LAUNCHER *******************

void useLauncher() {
  Launch.spinToPosition(180, degrees, 200, rpm, false);
  wait(0.25, seconds);
  Launch.spinToPosition(0, degrees, 200, rpm, false);
}

// *************** FLYWHEEL *******************

void useForwardFlywheel() {
  Flywheel1.setVelocity(300, rpm);
  Flywheel2.setVelocity(300, rpm);
  Flywheel1.setStopping(coast);
  Flywheel1.spin(reverse, RobotLaunchVariable, volt);
  Flywheel2.setStopping(coast);
  Flywheel2.spin(forward, RobotLaunchVariable, volt);
}



void drawGUI() {
  // Draws 2 buttons to be used for selecting auto
  Brain.Screen.clearScreen();
  Brain.Screen.setFillColor(red);
  Brain.Screen.drawRectangle(20, 50, 50, 50);
  Brain.Screen.printAt(25, 75, "0");
  Brain.Screen.setFillColor(orange);
  Brain.Screen.drawRectangle(120, 50, 50, 50);
  Brain.Screen.printAt(125, 75, "1");
  Brain.Screen.setFillColor(yellow);
  Brain.Screen.drawRectangle(220, 50, 50, 50);
  Brain.Screen.printAt(225, 75, "2");
  Brain.Screen.setFillColor(green);
  Brain.Screen.drawRectangle(320, 50, 50, 50);
  Brain.Screen.printAt(325, 75, "3");
  Brain.Screen.setFillColor(blue);
  Brain.Screen.drawRectangle(20, 150, 50, 50);
  Brain.Screen.printAt(25, 175, "4");
  Brain.Screen.setFillColor(purple);
  Brain.Screen.drawRectangle(120, 150, 50, 50);
  Brain.Screen.printAt(125, 175, "5");
  Brain.Screen.setFillColor(ClrPink);
  Brain.Screen.drawRectangle(220, 150, 50, 50);
  Brain.Screen.printAt(225, 175, "6");
  Brain.Screen.setFillColor(ClrGreenYellow);
  Brain.Screen.drawRectangle(320, 150, 50, 50);
  Brain.Screen.printAt(325, 175, "7");
  Brain.Screen.setFillColor(black);
}

void selectAuton() {
  Brain.Screen.printAt(400, 200, "Auton:");
  int x = Brain.Screen.xPosition(); // get the x position of last touch of the screen
  int y = Brain.Screen.yPosition(); // get the y position of last touch of the screen
  // check to see if buttons were pressed
  if (x >= 20 && x <= 70 && y >= 50 && y <= 100) { // select button pressed
      auton = 0;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if (x >= 120 && x <= 170 && y >= 50 && y <= 100) {
      auton = 1;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if (x >= 220 && x <= 270 && y >= 50 && y <= 100) {
      auton = 2;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if (x >= 320 && x <= 370 && y >= 50 && y <= 100) {
      auton = 3;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if (x >= 20 && x <= 70 && y >= 150 && y <= 200) {
      auton = 4;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if (x >= 120 && x <= 170 && y >= 150 && y <= 200) {
      auton = 5;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if (x >= 220 && x <= 270 && y >= 150 && y <= 200) {
      auton = 6;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if (x >= 320 && x <= 370 && y >= 150 && y <= 200) {
      auton = 7;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
  }
  wait(10, msec); // slow it down
  Brain.Screen.setFillColor(black);
}

// **************** AUTONOMOUS ****************

void pre_auton(void) {
  // P1.open();
  Brain.Screen.printAt(1, 40, "pre auton is running");
  drawGUI();
  Brain.Screen.pressed(selectAuton);
}

void autonomous(void) {
  switch (auton) {
    case 0:  
      // does nothing
      StopAllChasis();
      break;
    case 1:
      // spins up roller on left

      LeftFront.startSpinFor(180, degrees);
      LeftRear.startSpinFor(180, degrees);
      RightFront.startSpinFor(180, degrees);
      RightRear.spinFor(180, degrees);
      Vacuum.startSpinFor(-180, degrees);
      break;
    case 2:
      // spin up right roller

      LeftFront.startSpinFor(180, degrees);
      LeftRear.startSpinFor(180, degrees);
      RightFront.startSpinFor(180, degrees);
      RightRear.spinFor(180, degrees);

      LeftFront.startSpinFor(180, degrees);
      LeftRear.startSpinFor(180, degrees);
      RightFront.startSpinFor(-180, degrees);
      RightRear.spinFor(-180, degrees);

      LeftFront.startSpinFor(-180, degrees);
      LeftRear.startSpinFor(-180, degrees);
      RightFront.startSpinFor(-180, degrees);
      RightRear.spinFor(-180, degrees);

      LeftFront.startSpinFor(-180, degrees);
      LeftRear.startSpinFor(-180, degrees);
      RightFront.startSpinFor(180, degrees);
      RightRear.spinFor(180, degrees);

      LeftFront.startSpinFor(-180, degrees);
      LeftRear.startSpinFor(-180, degrees);
      RightFront.startSpinFor(-180, degrees);
      RightRear.spinFor(-180, degrees);

      Vacuum.startSpinFor(-180, degrees);
      break;
    case 3:
      // spin left roller then shoot

      LeftFront.startSpinFor(180, degrees);
      LeftRear.startSpinFor(180, degrees);
      RightFront.startSpinFor(180, degrees);
      RightRear.spinFor(180, degrees);
      Vacuum.startSpinFor(-180, degrees);

      LeftFront.startSpinFor(-180, degrees);
      LeftRear.startSpinFor(-180, degrees);

      Flywheel1.spin(forward);
      Flywheel2.spin(forward);
      wait(2, sec);
      for(int i = 0; i < 3; i++){
        useLauncher();
        wait(.25, sec);
      }
      break;
    case 4:
      // spin right roller then shoot

      LeftFront.startSpinFor(180, degrees);
      LeftRear.startSpinFor(180, degrees);
      RightFront.startSpinFor(180, degrees);
      RightRear.spinFor(180, degrees);

      LeftFront.startSpinFor(180, degrees);
      LeftRear.startSpinFor(180, degrees);
      RightFront.startSpinFor(-180, degrees);
      RightRear.spinFor(-180, degrees);

      LeftFront.startSpinFor(-180, degrees);
      LeftRear.startSpinFor(-180, degrees);
      RightFront.startSpinFor(-180, degrees);
      RightRear.spinFor(-180, degrees);

      LeftFront.startSpinFor(-180, degrees);
      LeftRear.startSpinFor(-180, degrees);
      RightFront.startSpinFor(180, degrees);
      RightRear.spinFor(180, degrees);

      LeftFront.startSpinFor(-180, degrees);
      LeftRear.startSpinFor(-180, degrees);
      RightFront.startSpinFor(-180, degrees);
      RightRear.spinFor(-180, degrees);

      Vacuum.startSpinFor(-180, degrees);

      Flywheel1.spin(forward);
      Flywheel2.spin(forward);
      wait(2, sec);
      for(int i = 0; i < 3; i++){
        useLauncher();
        wait(.25, sec);
      }
      break;
    case 5:
      // spin left roller, shoot, get more discs, shoot
      break;
    case 6:
      // spin right roller, shoot, get more discs, shoot
      break;
    case 7:
      // spin left rollers, shoot preload, pick up, shoot, spick up, shoot, spin right rollers, pick up, shoot, spick up, shoot
      break;
  }
}

// ************** DRIVER CONTROL **************

void drivercontrol(void) {
  // startTime = Brain.timer(sec);
 
  while (1) {
    if(Controller1.Axis3.value() == 0 && Controller1.Axis1.value() == 0) {StopAllChasis();}
    LeftFront.spin(forward, Controller1.Axis3.position() + Controller1.Axis1.position(), percent);
    RightFront.spin(forward, Controller1.Axis3.position() - Controller1.Axis1.position(), percent);
    LeftRear.spin(forward, Controller1.Axis3.position() + Controller1.Axis1.position(), percent);
    RightRear.spin(forward, Controller1.Axis3.position() - Controller1.Axis1.position(), percent);
    if(Controller1.ButtonL2.pressing()) {
      useForwardFlywheel();
    } 
    else {
      Flywheel1.stop(coast);
      Flywheel2.stop(coast);
    }
    if (Controller1.ButtonR2.pressing()) {
      useLauncher();
    }
    if(Controller1.ButtonA.pressing()){
      useForwardVacuum(100);
    }
    if(Controller1.ButtonB.pressing()){
      useForwardVacuum(0);
    }
    if(Controller1.ButtonX.pressing()){
      useReverseVacuum(100);
    }
    if(Controller1.ButtonY.pressing()){
      Vacuum.spinFor(180, degrees);
    }
    if(Controller1.ButtonLeft.pressing() && (RobotLaunchVariable != 7)){
        RobotLaunchVariable -= 1;
        Controller1.Screen.print(RobotLaunchVariable);
        wait(250, msec);
    }
    if(Controller1.ButtonRight.pressing() && (RobotLaunchVariable != 12)){
        RobotLaunchVariable += 1;
        Controller1.Screen.print(RobotLaunchVariable);
        wait(250, msec);
    }
    if(Controller1.ButtonUp.pressing() /*&& Brain.timer(sec) - startTime > 95*/){
      P1.open();
    }
    if(Controller1.ButtonDown.pressing() /*&& Brain.timer(sec) - startTime > 95*/){
      P1.close();
    }

  }
  wait(20, msec);
}

//******************** MAIN *******************

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(drivercontrol);
  pre_auton();
  while (true) {
    wait(100, msec);
  }
}
