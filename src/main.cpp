/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Carlo, Jacob, and Erik and a little bit of Jeffrey        */
/*    Created:      Tue Sep 27 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <cmath>
#include <algorithm>

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
int intake = 2;
bool spinvacmore = false;


//odometry variable
double OdomWheelDiam = 2.79;
double L_dist_to_center = 4.13;
double R_dist_to_center = 3.85;
double C_dist_to_center = 6.5;
double Delta_L = 0;
double Delta_R = 0;
double Delta_C = 0;
double Delta_theta = 0;
double delta_fwd = 0;
double delta_strafe = 0;
double robot_odometry_initialvalues[3] = {0, 0, 0};
double robot_odometry_coordinates[3] = {0, 0, 0};
double encoder_positions_old[3] = {0,0,0};
double encoder_positions_new[3] = {0,0,0};
int obamatreecounter = 0;

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

void useForwardVacuum(int percent) {
  Vacuum.spin(forward, percent, pct);
}

void useReverseVacuum(int percent) {
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
  Flywheel1.setStopping(coast);
  Flywheel2.setStopping(coast);
  if(Flywheel2.velocity(rpm) < 325) {
    Flywheel1.spin(reverse, 15, volt);
    Flywheel2.spin(forward, 15, volt);
  } else {
    Flywheel1.spin(reverse, 7.5, volt);
    Flywheel2.spin(forward, 7.5, volt);
  }
}

void useReverseFlywheel() {
  Flywheel1.setStopping(coast);
  Flywheel2.setStopping(coast);
  Flywheel1.spin(forward, 15, volt);
  Flywheel2.spin(reverse, 15, volt);
}

void useForwardFlywheelVariable() {
  Flywheel1.setVelocity(300, rpm);
  Flywheel2.setVelocity(300, rpm);
  Flywheel1.setStopping(coast);
  Flywheel1.spin(reverse, RobotLaunchVariable, volt);
  Flywheel2.setStopping(coast);
  Flywheel2.spin(forward, RobotLaunchVariable, volt);
}

// **************** DISTANCE *****************
double findDistance(double x, double y) {
  dist = (y - Cy) / (sin(atan((y - Cy)/(x - Cx))));
  return dist;
}

// *************** ODAMATREE *****************

void updateOdometry() {
  Delta_L = ((EncoderB.position(degrees))*M_PI/180)*(OdomWheelDiam/2);
  Delta_R = ((EncoderC.position(degrees))*M_PI/180)*(OdomWheelDiam/2);
  Delta_C = ((EncoderA.position(degrees))*M_PI/180)*(OdomWheelDiam/2);
}

double RectifyAngle(double Angle)
{
  double normalizedAngle = fmod(Angle,360);
  if(normalizedAngle<0)
  {
    return normalizedAngle+360;
  }
  else
  {
    return normalizedAngle;
  }

}

void obamatree()
{
  encoder_positions_new[0] = ((EncoderB.position(degrees))*M_PI/180)*(OdomWheelDiam/2);
  encoder_positions_new[1] = ((EncoderC.position(degrees))*M_PI/180)*(OdomWheelDiam/2);
  encoder_positions_new[2] = ((EncoderA.position(degrees))*M_PI/180)*(OdomWheelDiam/2);

  Delta_L = encoder_positions_new[0] - encoder_positions_old[0];
  Delta_R = encoder_positions_new[1] - encoder_positions_old[1];
  Delta_C = encoder_positions_new[2] - encoder_positions_old[2];
  
  delta_strafe = Delta_C - (C_dist_to_center*Delta_theta);

  delta_fwd = (Delta_L + Delta_R)/2;

  encoder_positions_old[0] = encoder_positions_new[0];
  encoder_positions_old[1] = encoder_positions_new[1];
  encoder_positions_old[2] = encoder_positions_new[2];

  double ThetaOne = (Delta_L - Delta_R) / (L_dist_to_center + R_dist_to_center) + robot_odometry_coordinates[2];
  
  Delta_theta = ThetaOne - robot_odometry_coordinates[2];
  
  robot_odometry_coordinates[2] = robot_odometry_coordinates[2] + Delta_theta;

  robot_odometry_coordinates[0] += (delta_fwd*cos(robot_odometry_coordinates[2]) - delta_strafe * sin(robot_odometry_coordinates[2]));
  robot_odometry_coordinates[1] += (delta_fwd*sin(robot_odometry_coordinates[2]) + delta_strafe * cos(robot_odometry_coordinates[2]));

  Brain.Screen.printAt(25, 125, "x: %f", robot_odometry_coordinates[0]);
  Brain.Screen.printAt(25, 150, "y: %f", robot_odometry_coordinates[1]);
  Brain.Screen.printAt(25, 175, "Theta: %f", RectifyAngle((-robot_odometry_coordinates[2]*180/M_PI)));
  //Brain.Screen.printAt(25, 175, "Theta: %f", robot_odometry_coordinates[2]);
}

double distanceBetweenTwoAngles (double angle1, double angle2)
{
  double dist1 = angle1-angle2;
  double dist2 = angle2-angle1;
  dist1 = std::max(dist1,dist2);
  return std::min(dist1,360-dist1);
}

void OdomSpinTo(double Target_theta)
{
  obamatree();
  double actualtheta = RectifyAngle((-robot_odometry_coordinates[2]*180/M_PI));
  double startingtheta = actualtheta;
  double ToGo = distanceBetweenTwoAngles(startingtheta, Target_theta);
  double ToGoFull = ToGo;
  //Brain.Screen.printAt(25, 75, "togofull: %f", ToGoFull);
  while(fabs(actualtheta-Target_theta)>0.3)
  {
    obamatree();
    actualtheta = RectifyAngle((-robot_odometry_coordinates[2]*180/M_PI));
    ToGo = distanceBetweenTwoAngles(actualtheta, Target_theta);

    //Brain.Screen.printAt(25, 200, "athets: %f", robot_odometry_coordinates[2]);

    LeftFront.setVelocity(((100*(ToGo/ToGoFull)+6)*ToGoFull/360)+5, percent);
    RightFront.setVelocity(((100*(ToGo/ToGoFull)+6)*ToGoFull/360)+5, percent);
    LeftRear.setVelocity(((100*(ToGo/ToGoFull)+6)*ToGoFull/360)+5, percent);
    RightRear.setVelocity(((100*(ToGo/ToGoFull)+6)*ToGoFull/360)+5, percent);
    //Brain.Screen.printAt(25, 100, "togo/togofull: %f", ToGo/ToGoFull);
    
    if(Target_theta > 180)
    {
      if((Target_theta - 180) < startingtheta && startingtheta < Target_theta)
      {
        //Brain.Screen.printAt(25, 50, "path 1");

        LeftFront.spin(reverse);
        LeftRear.spin(reverse);
        RightFront.spin(forward);
        RightRear.spin(forward);
      }
      else
      {
        //Brain.Screen.printAt(25, 50, "path 2");
        LeftFront.spin(forward);
        LeftRear.spin(forward);
        RightFront.spin(reverse);
        RightRear.spin(reverse);
      }
    }
    else
    {
      if(Target_theta < startingtheta && startingtheta < (Target_theta + 180))
      {
        //Brain.Screen.printAt(25, 50, "path 3");
        LeftFront.spin(forward);
        LeftRear.spin(forward);
        RightFront.spin(reverse);
        RightRear.spin(reverse);

        //Brain.Screen.printAt(25, 25, "target theta: %f", Target_theta);
        //Brain.Screen.printAt(25, 50, "strating theta: %f", startingtheta);
        //Brain.Screen.printAt(725, 75, "target theta+180: %f", Target_theta+180);
      }
      else
      {
        //Brain.Screen.printAt(25, 50, "path 4");
        LeftFront.spin(reverse);
        LeftRear.spin(reverse);
        RightFront.spin(forward);
        RightRear.spin(forward);
      }
    }
  }
  LeftFront.stop(hold);
  LeftRear.stop(hold);
  RightFront.stop(hold);
  RightRear.stop(hold);
}

void OdomMoveTo (double Target_x, double Target_y)
{
  obamatree();
  double diff_x = Target_x+robot_odometry_coordinates[0];
  double diff_y = Target_y-robot_odometry_coordinates[1];
  double pythTheta = atan(diff_y/diff_x);
  //Brain.Screen.printAt(25, 50, "pyththeta: %f", RectifyAngle(pythTheta*180/M_PI));
  OdomSpinTo(RectifyAngle(pythTheta*180/M_PI));
  OdomSpinTo(RectifyAngle(pythTheta*180/M_PI));
  obamatree();
  diff_x = Target_x+robot_odometry_coordinates[0];
  diff_y = Target_y-robot_odometry_coordinates[1];
  pythTheta = atan(diff_y/diff_x);
  Brain.Screen.printAt(25, 50, "dx: %f", diff_x);
  Brain.Screen.printAt(25, 75, "dy %f", diff_y);
  Brain.Screen.printAt(25, 100, "pyththeta %f", pythTheta);
  wait(0.5, sec);
  OdomSpinTo(RectifyAngle(pythTheta*180/M_PI));
  OdomSpinTo(RectifyAngle(pythTheta*180/M_PI));
  obamatree();
  diff_x = Target_x+robot_odometry_coordinates[0];
  diff_y = Target_y-robot_odometry_coordinates[1];
  pythTheta = atan(diff_y/diff_x);
  Brain.Screen.printAt(25, 50, "dx: %f", diff_x);
  Brain.Screen.printAt(25, 75, "dy %f", diff_y);
  Brain.Screen.printAt(25, 100, "pyththeta %f", pythTheta);
  wait(0.5, sec);
  OdomSpinTo(RectifyAngle(pythTheta*180/M_PI));
  OdomSpinTo(RectifyAngle(pythTheta*180/M_PI));
  Brain.Screen.printAt(25, 50, "dx: %f", diff_x);
  Brain.Screen.printAt(25, 75, "dy %f", diff_y);
  Brain.Screen.printAt(25, 100, "pyththeta %f", pythTheta);
  obamatree();
  wait(0.5, sec);

  //wait(0.5,sec);

  while (fabs(diff_x)>0.2) {
    obamatree();
    diff_x = Target_x + robot_odometry_coordinates[0];
    diff_y = Target_y - robot_odometry_coordinates[1];

    /*LeftFront.setVelocity(30, percent);
    RightFront.setVelocity(30, percent);
    LeftRear.setVelocity(30, percent);
    RightRear.setVelocity(30, percent);

    Brain.Screen.printAt(25, 50, "dx: %f", diff_x);
    Brain.Screen.printAt(25, 75, "dy %f", diff_y);
    LeftFront.spin(forward);
    LeftRear.spin(forward);
    RightFront.spin(forward);
    RightRear.spin(forward);*/
    LeftFront.spin(forward, 7.8740157480314, volt);
    RightFront.spin(forward, 7.8740157480314, volt);
    LeftRear.spin(forward,  7.8740157480314, volt);
    RightRear.spin(forward,  7.8740157480314, volt);

  }
    LeftFront.stop(hold);
    RightFront.stop(hold);
    LeftRear.stop(hold);
    RightRear.stop(hold);
    obamatree();
  //LeftFront.stop(hold);
  //LeftRear.stop(hold);
  //RightFront.stop(hold);
  //RightRear.stop(hold);
  //OdomSpinTo(Target_theta);
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
  if(x >= 20 && x <= 70 && y >= 50 && y <= 100) { // select button pressed
      auton = 0;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if(x >= 120 && x <= 170 && y >= 50 && y <= 100) {
      auton = 1;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if(x >= 220 && x <= 270 && y >= 50 && y <= 100) {
      auton = 2;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if(x >= 320 && x <= 370 && y >= 50 && y <= 100) {
      auton = 3;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if(x >= 20 && x <= 70 && y >= 150 && y <= 200) {
      auton = 4;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if(x >= 120 && x <= 170 && y >= 150 && y <= 200) {
      auton = 5;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if(x >= 220 && x <= 270 && y >= 150 && y <= 200) {
      auton = 6;
      Brain.Screen.printAt(400, 200, "Auton: %d", auton);
    } else if(x >= 320 && x <= 370 && y >= 150 && y <= 200) {
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
  //drawGUI();
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
  LeftFront.setBrake(brake);
  LeftRear.setBrake(brake);
  RightFront.setBrake(brake);
  RightRear.setBrake(brake);

  LeftFront.setMaxTorque(100, percent);
  RightFront.setMaxTorque(100, percent);
  LeftRear.setMaxTorque(100, percent);
  RightRear.setMaxTorque(100, percent);
  Brain.Screen.clearScreen();

  //robot_odometry_coordinates[0] = 0;
  //robot_odometry_coordinates[1] = 0;
  //robot_odometry_coordinates[2] = 0;
     
  while (1) {
    obamatree();
    
    
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
    if(Controller1.ButtonR2.pressing()) {
      useLauncher();
      // OdomSpinTo(45);
    }

    
    // int toggle;
    // if(Controller1.ButtonR1.pressing()) {
    //   toggle = 0;
    //   dir = 1;
    // }
    // if(Controller1.ButtonL1.pressing()) {
    //   toggle = 0;
    //   dir = 2;
    // }
    // if(Controller1.ButtonA.pressing()) {
    //   toggle = 1;
    //   dir = 1;
    // }
    // if(Controller1.ButtonX.pressing()) {
    //   toggle = 1
    //   dir = 2;
    // }
    // if(Controller1.ButtonB.pressing()) {
    //   dir = 0;
    // }


   if(Controller1.ButtonR1.pressing()||Controller1.ButtonL1.pressing()) {
      if(Controller1.ButtonR1.pressing()) {
        intake = 1;
        spinvacmore = false;
      } 
      else if(Controller1.ButtonL1.pressing()) {
        intake = 3;
        spinvacmore = false;
      }
      
    }
    else if(spinvacmore == false) {
      intake = 2;
      spinvacmore = true;
    }
    // if(Controller1.ButtonR1.pressing()) {
    //   Vacuum.spin(reverse, 100, pct);
    // } else if(!Controller1.ButtonR1.pressing() && !Controller1.ButtonL1.pressing()) {
    //   Vacuum.spin(forward, 0, pct);
    // }
    
    if(Controller1.ButtonA.pressing()) {
      intake = 1;
      spinvacmore = true;
    }
    if(Controller1.ButtonB.pressing()) {
      intake = 2;
      spinvacmore = false;
    }
    if(Controller1.ButtonX.pressing()) {
      intake = 3;
      spinvacmore = true;
    }
    if(Controller1.ButtonY.pressing()) {
      Vacuum.spinFor(-180, degrees);
    }

    if(intake == 1) {
      useForwardVacuum(100);
    }
    if(intake == 2) {
      useForwardVacuum(0);
    }
    if(intake == 3) {
      useReverseVacuum(100);
    }

    if(Controller1.ButtonLeft.pressing() && (RobotLaunchVariable != 7)) {
      RobotLaunchVariable -= 1;
      Controller1.Screen.print(RobotLaunchVariable);
      wait(250, msec);
    }
    if(Controller1.ButtonRight.pressing() && (RobotLaunchVariable != 12)) {
      RobotLaunchVariable += 1;
      Controller1.Screen.print(RobotLaunchVariable);
      wait(250, msec);
    }
    if(Controller1.ButtonUp.pressing() && Brain.timer(sec) - startTime > 95) {
      P1.open();
    }
    if(Controller1.ButtonDown.pressing() /*&& Brain.timer(sec) - startTime > 95*/) {
      OdomMoveTo(0, 0, 0);
      // useReverseFlywheel();
      // Brain.Screen.print(EncoderA.position(degrees));
      // wait(.5, sec);
    }
  }
  wait(20, msec);
}

// ******************** MAIN *******************

int main() {
  EncoderA.resetRotation();
  EncoderB.resetRotation();
  EncoderC.resetRotation();
  wait(0.3, sec);
  obamatree();

  //OdomSpinTo(45);
  OdomMoveTo(10,10);

  wait(1, sec);

  obamatree();

  OdomMoveTo(20,-10);

  //wait(5, sec);

  //obamatree();

  //OdomMoveTo(35,15);

  //wait(5,sec);

  obamatree();

  //OdomSpinTo(120);
  //wait(0.5, sec);
  //obamatree();
  //OdomSpinTo(240);
  //wait(0.5, sec);
  //obamatree();
  //OdomSpinTo(0);
  //wait(0.5, sec);
  //obamatree();

  Competition.autonomous(autonomous);
  Competition.drivercontrol(drivercontrol);
  pre_auton();
  while (true) {
    wait(100, msec);
  }
}
