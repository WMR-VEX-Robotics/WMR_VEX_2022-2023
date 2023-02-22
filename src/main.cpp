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
motor Launch = motor(PORT3, ratio6_1, true);
motor Flywheel1 = motor(PORT4, ratio6_1, true);
motor Flywheel2 = motor(PORT5, ratio6_1, false);
pneumatics P1 = pneumatics(Brain.ThreeWirePort.G);
encoder EncoderA = encoder(Brain.ThreeWirePort.A);
encoder EncoderB = encoder(Brain.ThreeWirePort.C);
encoder EncoderC = encoder(Brain.ThreeWirePort.E);

// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
bool release = false;
double RobotLaunchVariable = 7.5;
int auton = 1;
int AutonMin = 0;
int AutonMax = 6;
int length = 0;
int lengthSum = 0;
int lengthAvg = 0;
int startTime = 0;
double Cx = 0;
double Cy = 0;
double dist = 0;

//odometry variable
double L_dist_to_center = 4.25;
double R_dist_to_center = 4.25;
double C_dist_to_center = 9.5;
double initial_L = EncoderB.position(degrees);
double initial_R = EncoderC.position(degrees);
double initial_C = EncoderA.position(degrees);
double Delta_L = 0;
double Delta_R = 0;
double Delta_C = 0;
double robot_odometry[3] = {0, 0, 0};

// ***************** DRIVE ********************

void StopAllChasis(void) {
  LeftFront.stop(hold);
  LeftRear.stop(hold);
  RightFront.stop(hold);
  RightRear.stop(hold);
}

void moveForward(double amount) {
  LeftFront.startSpinFor(amount, degrees);
  LeftRear.startSpinFor(amount, degrees);
  RightFront.startSpinFor(amount, degrees);
  RightRear.spinFor(amount, degrees);
}

void moveBackwards(double amount) {
  LeftFront.startSpinFor(-amount, degrees);
  LeftRear.startSpinFor(-amount, degrees);
  RightFront.startSpinFor(-amount, degrees);
  RightRear.spinFor(-amount, degrees);
}

void turnLeft(double amount) {
  LeftFront.startSpinFor(-amount, degrees);
  LeftRear.startSpinFor(-amount, degrees);
  RightFront.startSpinFor(amount, degrees);
  RightRear.spinFor(amount, degrees);
}

void turnRight(double amount) {
  LeftFront.startSpinFor(amount, degrees);
  LeftRear.startSpinFor(amount, degrees);
  RightFront.startSpinFor(-amount, degrees);
  RightRear.spinFor(-amount, degrees);

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
  Launch.spinToPosition(180, degrees, 600, rpm, false);
  wait(0.15, seconds);
  Launch.spinToPosition(0, degrees, 600, rpm, false);
  wait(0.15, seconds);
  if(Controller1.ButtonR2.pressing()) {
    useLauncher();
  }
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

// ***************** Distance *******************
double findDistance(double x, double y){
  dist = (y - Cy) / (sin(atan((y - Cy)/(x - Cx))));
  return dist;
}

// ***************** obamatree *******************

void updateOdometry()
{
  Delta_L = ((EncoderB.position(degrees)-initial_L)*M_PI/180)*(2.79/2);
  Delta_R = -1*(((EncoderC.position(degrees)-initial_R)*M_PI/180)*(2.79/2));
  Delta_C = ((EncoderA.position(degrees)-initial_C)*M_PI/180)*(2.79/2);
}

double findOrientation()
{
  updateOdometry();
  double temp_orientation;
  temp_orientation = (Delta_L - Delta_R) / (L_dist_to_center + R_dist_to_center);
  return temp_orientation;
}

double find_Position_X()
{
  updateOdometry();
  double temp_position_x;
  temp_position_x = 2 * sin(findOrientation()/2) * ((Delta_C / findOrientation()) + C_dist_to_center);
  return temp_position_x;
}

double find_Position_Y()
{
  updateOdometry();
  double temp_position_y;
  temp_position_y = 2 * sin(findOrientation()/2) * ((Delta_R / findOrientation()) + R_dist_to_center);
  return temp_position_y;
}

void odometrize()
{
  robot_odometry[0]=find_Position_X();
  robot_odometry[1]=find_Position_Y();
  robot_odometry[2]=findOrientation();
  Brain.Screen.printAt(25, 75, "x: %f", robot_odometry[0]);
  Brain.Screen.printAt(25, 125, "y: %f", robot_odometry[1]);
  Brain.Screen.printAt(25, 175, "Theta: %f", robot_odometry[2]*180/M_PI);
  //updateOdometry();
  // Brain.Screen.printAt(25, 75, "delta_l: %f", Delta_L);
  // Brain.Screen.printAt(25, 125, "delta_r: %f", Delta_R);
  // Brain.Screen.printAt(25, 175, "delta_c %f", Delta_C);
}

// ***************** end of obamatree *******************

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
      Vacuum.setPosition(0, degrees);
      LeftFront.setVelocity(100, percent);
      RightFront.setVelocity(100, percent);
      LeftRear.setVelocity(100, percent);
      RightRear.setVelocity(100, percent);
      Vacuum.setVelocity(100, percent);

      LeftFront.setPosition(0, degrees);
      RightFront.setPosition(0, degrees);
      LeftRear.setPosition(0, degrees);
      RightRear.setPosition(0, degrees);

      LeftFront.setMaxTorque(25, percent);
      RightFront.setMaxTorque(25, percent);
      LeftRear.setMaxTorque(25, percent);
      RightRear.setMaxTorque(25, percent);
      Vacuum.setMaxTorque(100, percent);


      LeftFront.spinToPosition(90, degrees, 200, rpm, false);
      LeftRear.spinToPosition(90, degrees, 200, rpm, false);
      RightFront.spinToPosition(90, degrees, 200, rpm, false);
      RightRear.spinToPosition(90, degrees, 200, rpm, false);

      wait(.5, sec);
      
      Vacuum.spinToPosition(-180, degrees, 200, rpm, true);

      wait(1, sec);

      moveBackwards(90);

       wait(.25, sec);

      turnRight(160);

      wait(.5, sec);

      LeftFront.startSpinFor(-2000, degrees, 100, rpm);
      LeftRear.startSpinFor(-2000, degrees, 100, rpm);
      RightFront.startSpinFor(-2000, degrees, 100, rpm);
      RightRear.spinFor(-2000, degrees, 100, rpm);
    
      wait(.5, sec);

      turnLeft(310);
     
      //wait(.25, sec);

      // Flywheel1.startSpinFor(-1160, degrees, 100, rpm);
      Flywheel1.spin(reverse, 8.85, volt);
      Flywheel2.spin(forward, 8.85, volt);
      // Flywheel2.spinFor(1160, degrees, 100, rpm);

      wait(2, sec);
      useLauncher();
      Flywheel1.spin(reverse, 8.5, volt);
      Flywheel2.spin(forward, 8.5, volt);
      wait(2, sec);
      useLauncher();

      break;
    case 2:
      // spin up right roller
      break;
    case 3:
      // spin left roller then shoot
      break;
    case 4:
      // spin right roller then shoot
      break;
    case 5:
      // spin left roller, shoot, get more discs, shoot
      break;
    case 6:
      // spin right roller, shoot, get more discs, shoot
      break;
    case 7:
      // spin left rollers, shoot preload, pick up, shoot, spick up, shoot, spin right rollers, pick up, shoot, spick up, shoot
      moveBackwards(90);

      turnRight(330);

      moveBackwards(775);

      turnRight(330);

      // LeftFront.setBrake(coast);
      // LeftRear.setBrake(coast);
      // RightFront.setBrake(coast);
      // RightRear.setBrake(coast);
      LeftFront.setVelocity(50, percent);
      RightFront.setVelocity(50, percent);
      LeftRear.setVelocity(50, percent);
      RightRear.setVelocity(50, percent);
      Vacuum.setVelocity(100, percent);

      moveForward(270);

      Vacuum.spin(forward);
      LeftFront.setVelocity(25, percent);
      RightFront.setVelocity(25, percent);
      LeftRear.setVelocity(25, percent);
      RightRear.setVelocity(25, percent);
      wait(1, sec);
      moveForward(360);
      break;
  }
}

// ************** DRIVER CONTROL **************

void drivercontrol(void) {
  // startTime = Brain.timer(sec);
  LeftFront.setBrake(coast);
  LeftRear.setBrake(coast);
  RightFront.setBrake(coast);
  RightRear.setBrake(coast);

  LeftFront.setMaxTorque(100, percent);
  RightFront.setMaxTorque(100, percent);
  LeftRear.setMaxTorque(100, percent);
  RightRear.setMaxTorque(100, percent);
  Brain.Screen.clearScreen();
     
  while (1) {
    odometrize();
    
    if(Controller1.Axis3.value() == 0 && Controller1.Axis1.value() == 0) {StopAllChasis();}
    LeftFront.spin(forward, (Controller1.Axis3.position() + Controller1.Axis1.position()) / 7.8740157480314, volt);
    RightFront.spin(forward, (Controller1.Axis3.position() - Controller1.Axis1.position()) / 7.8740157480314, volt);
    LeftRear.spin(forward, (Controller1.Axis3.position() + Controller1.Axis1.position()) / 7.8740157480314, volt);
    RightRear.spin(forward, (Controller1.Axis3.position() - Controller1.Axis1.position()) / 7.8740157480314, volt);
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

    if (Controller1.ButtonR1.pressing() || Controller1.ButtonL1.pressing()) {
      if (Controller1.ButtonR1.pressing()) {
        Vacuum.spin(forward, 100, pct);
      } else {
        Vacuum.spin(reverse, 100, pct);
      }
    } else {
      Vacuum.spin(forward, 0, pct);
    }
    /*
    }
    if (Controller1.ButtonR1.pressing()) {
      Vacuum.spin(reverse, 100, pct);
    } else if (!Controller1.ButtonR1.pressing() && !Controller1.ButtonL1.pressing()) {
      Vacuum.spin(forward, 0, pct);
    }
    */
    
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
      Vacuum.spinFor(-180, degrees);
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
    if(Controller1.ButtonDown.pressing() /*&& Brain.timer(sec) - startTime > 95*/){
      //P1.close();
      Brain.Screen.clearScreen();
    Brain.Screen.print(EncoderA.position(degrees));
    wait(.5, sec);
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
