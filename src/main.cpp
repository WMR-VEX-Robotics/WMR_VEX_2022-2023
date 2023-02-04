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
motor LeftFront = motor(PORT8, ratio6_1, false);
motor LeftRear = motor(PORT7, ratio6_1, false);
motor RightFront = motor(PORT10, ratio6_1, true);
motor RightRear = motor(PORT9, ratio6_1, true);
motor Vacuum = motor(PORT20, ratio6_1, true);
motor Launch = motor(PORT19, ratio18_1, true);
motor Flywheel1 = motor(PORT4, ratio18_1, true);
motor Flywheel2 = motor(PORT5, ratio18_1, false);
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
  Launch.spinToPosition(180, degrees, 200, rpm, true);
  Launch.spinToPosition(0, degrees, 200, rpm, true);
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

// *************** PNEUMATICS *****************

// void usePneumatics(void){
//   if (release == true) {
//     vex::pneumatics::open();
//   } else {
//     vex::pneumatics::closed();
//   }
// }

// *************** BRAIN PRINT ****************

// void print(int per, bool man) {
//   Controller1.Screen.clearScreen();
//   Controller1.Screen.print("%i%", per);
//   Controller1.Screen.newLine();
//   Controller1.Screen.print("%s", manual ? "true" : "false");
// }

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

//********* AUTONOMOUS DRIVE FUNCTIONS ********

// Example: driveForward(14, 75);
// Inches is in inches, velocity is in percent
// 12.57 is the conversion from rotations to inches, making the function easier to use in the scale of the arena
// 12.57 isnt the right number, needs to be changed
// spinFor (double rotation, rotationUnits units, double velocity, velocityUnits units_v, bool waitForCompletion=true)

// void driveForward(double inches, double velocity) {
//   LeftFront.spinFor(inches / 12.57, rev, velocity, pct, true);
//   RightFront.spinFor(inches / 12.57, rev, velocity, pct, true);
//   LeftRear.spinFor(inches / 12.57, rev, velocity, pct, true);
//   RightRear.spinFor(inches / 12.57, rev, velocity, pct, true);
// }

// void driveBackward(double inches, double velocity) {
//   LeftFront.spinFor(inches / -12.57, rev, velocity, pct, true);
//   RightFront.spinFor(inches / -12.57, rev, velocity, pct, true);
//   LeftRear.spinFor(inches / -12.57, rev, velocity, pct, true);
//   RightRear.spinFor(inches / -12.57, rev, velocity, pct, true);
// }

// void turnRight(double degrees, double velocity) {
//   LeftFront.spinFor(degrees / 12.57, rev, velocity, pct, true);
//   RightFront.spinFor(degrees / -12.57, rev, velocity, pct, true);
//   LeftRear.spinFor(degrees / 12.57, rev, velocity, pct, true); 
//   RightRear.spinFor(degrees / -12.57, rev, velocity, pct, true);
// }

// void turnLeft(double degrees, double velocity) {
//   LeftFront.spinFor(degrees / -12.57, rev, velocity, pct, true);
//   RightFront.spinFor(degrees / 12.57, rev, velocity, pct, true);
//   LeftRear.spinFor(degrees / -12.57, rev, velocity, pct, true);
//   RightRear.spinFor(degrees / 12.57, rev, velocity, pct, true);
// }

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
      StopAllChasis();
      // LeftFront.spin(forward, 100, percent);
      // RightFront.spin(forward, 100, percent);
      // LeftRear.spin(forward, 100, percent);
      // RightRear.spin(forward, 100, percent);
      // useForwardVacuum(100);
      // wait(2000, msec);
      // useForwardVacuum(0);
      // LeftFront.spin(forward, 100, percent);
      // RightFront.spin(forward, 100, percent);
      // LeftRear.spin(forward, 100, percent);
      // RightRear.spin(forward, 100, percent);
      // wait(5000, msec);
      // LeftFront.stop();
      // RightFront.stop();
      // LeftRear.stop();
      // RightRear.stop();
      // LeftFront.spin(reverse, 50, percent);
      // RightFront.spin(forward, 50, percent);
      // LeftRear.spin(reverse, 50, percent);
      // RightRear.spin(forward, 50, percent);
      // wait(90, msec);
      // useForwardFlywheel(100);
      // wait(1500, msec);
      // for(int i=0; i<3; i++) {
      //   useLauncher(100);
      //   wait(150, msec);
      // }
      break;
    case 1:
      Vacuum.spinToPosition(90, degrees);
      break;
    case 2:
      Flywheel1.spin(forward);
      Flywheel2.spin(forward);
      wait(1, sec);
      for(int i = 0; i < 2; i++){
        useLauncher();
      }
      break;
    case 3:
      Vacuum.spinToPosition(90, degrees);
      LeftFront.spinFor(forward, 360, degrees);
      RightFront.spinFor(forward, 360, degrees);
      LeftRear.spinFor(forward, 360, degrees);
      RightRear.spinFor(forward, 360, degrees);

      LeftFront.spinFor(reverse, 360, degrees);
      RightFront.spinFor(forward, 360, degrees);
      LeftRear.spinFor(reverse, 360, degrees);
      RightRear.spinFor(forward, 360, degrees);

      Vacuum.spin(forward);
      LeftFront.spinFor(forward, 720, degrees);
      RightFront.spinFor(forward, 720, degrees);
      LeftRear.spinFor(forward, 720, degrees);
      RightRear.spinFor(forward, 720, degrees);
      Vacuum.stop();

      LeftFront.spinFor(forward, 360, degrees);
      RightFront.spinFor(reverse, 360, degrees);
      LeftRear.spinFor(forward, 360, degrees);
      RightRear.spinFor(reverse, 360, degrees);

      Flywheel1.spin(forward);
      Flywheel2.spin(forward);
      wait(1, sec);
      for(int i = 0; i < 3; i++){
        useLauncher();
      }
      break;
    case 4:
      Vacuum.spinToPosition(90, degrees);
      LeftFront.spinFor(forward, 360, degrees);
      RightFront.spinFor(forward, 360, degrees);
      LeftRear.spinFor(forward, 360, degrees);
      RightRear.spinFor(forward, 360, degrees);

      LeftFront.spinFor(forward, 360, degrees);
      RightFront.spinFor(reverse, 360, degrees);
      LeftRear.spinFor(forward, 360, degrees);
      RightRear.spinFor(reverse, 360, degrees);

      Flywheel1.spin(forward, RobotLaunchVariable, volt);
      Flywheel2.spin(forward, RobotLaunchVariable, volt);
      wait(1, sec);
      for(int i = 0; i < 3; i++){
        useLauncher();
      }
      break;
    case 5:
      Vacuum.spinToPosition(90, degrees);
      LeftFront.spinFor(forward, 360, degrees);
      RightFront.spinFor(forward, 360, degrees);
      LeftRear.spinFor(forward, 360, degrees);
      RightRear.spinFor(forward, 360, degrees);

    LeftFront.spinFor(reverse, 360, degrees);
      RightFront.spinFor(forward, 360, degrees);
      LeftRear.spinFor(reverse, 360, degrees);
      RightRear.spinFor(forward, 360, degrees);

      Flywheel1.spin(forward, RobotLaunchVariable, volt);
      Flywheel2.spin(forward, RobotLaunchVariable, volt);
      wait(1, sec);
      for(int i = 0; i < 3; i++){
        useLauncher();
      }
      break;
    case 6:
      
      break;
    case 7:
      break;
  }
}

// ************** DRIVER CONTROL **************

void drivercontrol(void) {
  startTime = Brain.timer(sec);
  while (1) {
    if(Controller1.Axis3.value() == 0 && Controller1.Axis1.value() == 0) {StopAllChasis();}
    LeftFront.spin(forward, Controller1.Axis3.position() + Controller1.Axis1.position(), percent);
    RightFront.spin(forward, Controller1.Axis3.position() - Controller1.Axis1.position(), percent);
    LeftRear.spin(forward, Controller1.Axis3.position() + Controller1.Axis1.position(), percent);
    RightRear.spin(forward, Controller1.Axis3.position() - Controller1.Axis1.position(), percent);
    if(Controller1.ButtonL2.pressing()) {
      useForwardFlywheel();
    // } else if(Controller1.ButtonL1.pressing()) {
    //     useReverseFlywheel();
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
      Vacuum.spinFor(360, degrees);
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
    if(Controller1.ButtonUp.pressing() && Brain.timer(sec) - startTime > 95){
      P1.open();
    }
    if(Controller1.ButtonDown.pressing() && Brain.timer(sec) - startTime > 95){
      P1.close();
    }
    // Controller1.Screen.print("Efficiency: ");
    // Controller1.Screen.print(Launcher.efficiency(percent));
    // Controller1.Screen.setCursor(2, 2);
    // Controller1.Screen.clearLine();
    // Controller1.Screen.newLine();
    // Controller1.Screen.print("Temps: ");
    // Controller1.Screen.print(Launcher.temperature(percent));
    // Controller1.Screen.setCursor(3, 3);
    // Controller1.Screen.clearLine();
    // Controller1.Screen.newLine();
    // Controller1.Screen.print("Torque: ");
    // Controller1.Screen.print(Launcher.torque(Nm));
    // Controller1.Screen.setCursor(4, 4);
    // Controller1.Screen.clearLine();
    // Controller1.Screen.newLine();
    // Controller1.Screen.print("Wattage: ");
    // Controller1.Screen.print(Launcher.power());
    
    // char color;
    // if (Launcher.temperature(percent) > 75) {
    //   color = red;
    // } else if (Launcher.temperature(percent) > 50 && Launcher.temperature(percent) <= 75) {
    //   color = orange;
    // } else if (Launcher.temperature(percent) > 25 && Launcher.temperature(percent) <= 50) {
    //   color = yellow;
    // } else {
    //   color = green;
    // }
    
    // Controller1.Screen.clearScreen();
    
    // length = Launcher.efficient(percent);
    // for (int i = 0; i < 5; i++) {
    //   lengthSum = lengthSum + length;
    //   if (i == 5) {
    //     lengthAvg = lengthSum / 5;
    //     lengthSum = 0;
    //     Brain.Screen.clearScreen();
    //     Brain.Screen.setFillColor(transparent);
    //     Brain.Screen.drawRectangle(20, 300, 50, 300);
    //     Brain.Screen.printAt(25, 75, "0");
    //     Brain.Screen.setFillColor(tempColor);
    //     Brain.Screen.drawRectangle(20, lengthAvg * 3, 50, lengthAvg * 3);
    //     Brain.Screen.printAt(25, 75, "0");
    //   }
    // }
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
