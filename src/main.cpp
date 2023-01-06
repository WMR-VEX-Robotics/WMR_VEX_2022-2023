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
motor Roll = motor(PORT6, ratio18_1, true);
motor Launch = motor(PORT6, ratio18_1, true);
pneumatics P1 = pneumatics(Brain.ThreeWirePort.F);

// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
int RobotReverseVariable = 1;
bool manual = false;
int amount = 0;
int AutonSelected = 0;
int AutonMin = 0;
int AutonMax = 6;

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
  if (x >= 20 && x <= 70 && y >= 50 && y <= 100) // select button pressed
  {
    AutonSelected = 0;
      
    Brain.Screen.printAt(400, 200, "Auton: %d", AutonSelected);
  }
  else if (x >= 120 && x <= 170 && y >= 50 && y <= 100){
    AutonSelected = 1;
      
    Brain.Screen.printAt(400, 200, "Auton: %d", AutonSelected);
  }
  else if (x >= 220 && x <= 270 && y >= 50 && y <= 100){
    AutonSelected = 2;
      
    Brain.Screen.printAt(400, 200, "Auton: %d", AutonSelected);
  }
  else if (x >= 320 && x <= 370 && y >= 50 && y <= 100){
    AutonSelected = 3;
      
    Brain.Screen.printAt(400, 200, "Auton: %d", AutonSelected);
  }
  else if (x >= 20 && x <= 70 && y >= 150 && y <= 200){
    AutonSelected = 4;
      
    Brain.Screen.printAt(400, 200, "Auton: %d", AutonSelected);
  }
  else if (x >= 120 && x <= 170 && y >= 150 && y <= 200){
    AutonSelected = 5;
      
    Brain.Screen.printAt(400, 200, "Auton: %d", AutonSelected);
  }
  else if (x >= 220 && x <= 270 && y >= 150 && y <= 200){
    AutonSelected = 6;
      
    Brain.Screen.printAt(400, 200, "Auton: %d", AutonSelected);
  }
  else if (x >= 320 && x <= 370 && y >= 150 && y <= 200){
    AutonSelected = 7;
      
    Brain.Screen.printAt(400, 200, "Auton: %d", AutonSelected);
  }

  wait(10, msec); // slow it down
  Brain.Screen.setFillColor(black);
}
void pre_auton(void) {
  P1.open();
  Brain.Screen.printAt(1, 40, "pre auton is running");
  drawGUI();
  Brain.Screen.pressed(selectAuton);
}

void autonomous(void) {
  switch (AutonSelected) {
  case 0:
  StopAllChasis();
  break;
    case 1:
  Roll.spinFor(.5, sec);
  break;
    case 2:
  //code 2
  break;
    case 3:
  //code 3
  break;
    case 4:
  //code 4
  break;
    case 5:
  //code 5
  break;
    case 6:
  //code 6
  break;
  case 7:
  //code 7
  break;
  }

//spin roller
//get some disc
//lauch them into the goal
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
    if(Controller1.ButtonUp.pressing()) {
      Launch.spin(forward);
      wait(.1, sec);
      Launch.spin(reverse);
      wait(.1, sec);
      Launch.stop();

    }
  }
  P1.close();
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