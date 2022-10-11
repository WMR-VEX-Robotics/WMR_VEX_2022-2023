/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\bowma                                            */
/*    Created:      Sun Oct 09 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Top_Left             motor         1               
// Top_Right            motor         2               
// Bottom_Left          motor         3               
// Bottom_Right         motor         4   
// Vacuum               motor         5
// Flywheel             motor         6            
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"


using namespace vex;

 int userDrive;
 int gameMode;
//Game Mode 0 = pregame. Game Mode 1 = Autonomous. Game Mode 2 = User Control.
 int vSpin;
 int bF;
 int bS;
 int bT;
 int bFo;

void setup_x_drive(void) {
  
  Top_Left.current();
  Top_Left.efficiency();
  Top_Left.power();
  Top_Left.setMaxTorque(1000, percent);
  Top_Left.setVelocity(3200, rpm);

  Bottom_Left.current();
  Bottom_Left.efficiency();
  Bottom_Left.power();
  Bottom_Left.setMaxTorque(1000, percent);
  Bottom_Left.setVelocity(3200, rpm);

  Top_Right.current();
  Top_Right.efficiency();
  Top_Right.power();
  Top_Right.setMaxTorque(1000, percent);
  Top_Right.setVelocity(3200, rpm);

  Bottom_Right.current();
  Bottom_Right.efficiency();
  Bottom_Right.power();
  Bottom_Right.setMaxTorque(1000, percent);
  Bottom_Right.setVelocity(3200, rpm);

  bool avgEfficiency = ((Top_Left.efficiency() + Bottom_Left.efficiency() + Top_Right.efficiency() + Bottom_Right.efficiency()) / 4);
  std::string printEfficiency = "Average Efficiency= " + avgEfficiency;
   if (avgEfficiency < 90 ) {
    Controller1.Screen.print("ERROR 1 IMPROPER EFFICIENCY FOR OVERCLOCK DISCONTINUE");
    wait(2, seconds);
    Top_Left.setMaxTorque(100, percent);
    Top_Left.setVelocity(600, rpm);
    Bottom_Left.setMaxTorque(100, percent);
    Bottom_Left.setVelocity(600, rpm);
    Top_Right.setMaxTorque(100, percent);
    Top_Right.setVelocity(600, rpm);
    Bottom_Right.setMaxTorque(100, percent);
    Bottom_Right.setVelocity(600, rpm);
    Controller1.Screen.clearLine(1);
    bF = 1;
   } else {
      Controller1.Screen.print("CHECK GREEN");
      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.print(printEfficiency);
      wait(1, seconds);
      Controller1.Screen.clearScreen();
      bF = 0;
    }
  //this prevents the robot from running at the specified overclock if the efficiency is not balanced

  bool avgCurrent = ((Top_Left.current() + Top_Right.current() + Bottom_Left.current() + Bottom_Right.current()) / 4);
  std::string printCurrent = "Average Current= " + avgCurrent;
  Controller1.Screen.print(printCurrent);
  wait(1, seconds);
  Controller1.Screen.clearScreen();
  //this just tells you how much current you are pulling

  bool avgPower = ((Top_Left.power() + Top_Right.power() + Bottom_Left.power() + Bottom_Right.power()) / 4);
  std::string printPower = "Average power= " + avgPower;
  Controller1.Screen.print(printPower);
  wait(1, seconds);
  Controller1.Screen.clearScreen();
  //same as current but with power
  break_Factors;
}

void vacuum_configuration(void) {
  Vacuum.current();
  Vacuum.efficiency();
  Vacuum.power();
  Vacuum.setMaxTorque(250, percent);
  Vacuum.setVelocity(1200, rpm);
  
  if (Vacuum.efficiency() < 90) {
    Vacuum.setMaxTorque(100, percent);
    Vacuum.setVelocity(600, rpm);
    Controller1.Screen.print("ERROR 2 VACUUM OVERCLOCK NOT POSSIBLE");
    bS = 1;
    wait(1, seconds);
    Controller1.Screen.clearScreen();
  } else {
    Controller1.Screen.print("CHECK GREEN");
    bS = 0;
    wait(1, seconds);
    Controller1.Screen.clearScreen();
  }

  Vacuum.spinToPosition(0, degrees, true);
  break_Factors;
}

void break_Factors(void){
  if (bF == 1){
    while (bF == 1){
      break;
    }
  } else {
    while (bF == 0){
      continue;
    }
  }

  if (bS == 1){
      while (bS == 1){
      break;
    }
  } else {
    while(bS == 0){
      continue;
    }
  }

  if (bT == 1){
    while (bT == 1){
      break;
    }
  } else {
    while(bT == 0){
      continue;
    }

  }

  if (bFo == 1){
    while (bFo == 1){
      break;
    }
  } else {
    while (bFo == 0){
      continue;
    }
  }
  
}

void rvacuum_check(void){
  if (vSpin == 0){
    Vacuum.stop();
    wait(10, seconds);
    Vacuum.spin(forward, 1200, rpm);
  }
}

void flywheel_calibration(void){
  if(Flywheel.efficiency() >= 90){
    Flywheel.spin(forward, 3200, rpm);
    if (Flywheel.isSpinning() == false){
      Flywheel.stop(brake);
      Controller1.Screen.print("HELP");
      Brain.Screen.print("FLYWHEEL MALFUNCTION");
      bFo = 1;
    } else {
      bFo = 0;
    }
    wait(50, msec);
    Flywheel.stop(brake);
    Flywheel.spin(reverse, 3200, rpm);
    if (Flywheel.isSpinning() == false){
      Flywheel.stop(brake);
      Controller1.Screen.print("HELP");
      Brain.Screen.print("FLYWHEEL MALFUNCTION");
      bFo = 1;
    } else {
      bFo = 0;
    }
    wait(50, msec);
    Flywheel.stop(brake);
    Flywheel.setPosition(0, degrees);
  }
  break_Factors;
}

void flywheel_config(void){
  Flywheel.current();
  Flywheel.efficiency();
  Flywheel.power();
  Flywheel.setMaxTorque(2000, percent);
  Flywheel.setVelocity(3200, rpm);
  if (Flywheel.efficiency() < 90){
    Controller1.Screen.print("ERROR 4 FLYWHEEL NOT OVERCLOCKABLE");
    bT = 1;
    wait(1, seconds);
    Controller1.Screen.clearScreen();
  } else {
    Controller1.Screen.print("CHECK GREEN");
    bT = 0;
    wait(1, seconds);
    Controller1.Screen.clearScreen();
  }
  flywheel_calibration;
  break_Factors;
}

void vacuum_check(void){
  if (Vacuum.isSpinning() == true){
    Brain.Screen.print("Vacuum Check");
    vSpin = 1;
  } else {
    Brain.Screen.print("HELP ERROR 3: VACUUM NOT STARTED");
    while(Vacuum.isSpinning() == false){
      for (int i = 1; i <= 10; ++i) {
        Controller1.Screen.print("SEND HELP");
        wait(1, seconds);
        Controller1.Screen.clearScreen();
        vSpin = 0;
      }
    }
  }
  if (vSpin == 0){
    rvacuum_check;
  }
}

void auton_mode(void){
  if (gameMode == 1) {
      Controller1.Screen.print("Auton");
      wait(50, msec);
      Controller1.Screen.clearScreen();
  }
  Vacuum.spin(forward, 1200, rpm);
  vacuum_check;
  gameMode = 2;
}

void xdrive_user_control(void){
  double velocityControl1 = (Controller1.Axis2.position() + 100);
  double velocityControl2 = (velocityControl1 / 200);
  Top_Left.spin(forward, (((-Controller1.Axis3.position()) - Controller1.Axis4.position() - (Controller1.Axis1.position() / 2))) * velocityControl2, percent);
  Bottom_Left.spin(forward, (((-Controller1.Axis3.position()) + Controller1.Axis4.position() - (Controller1.Axis1.position() / 2))) * velocityControl2, percent);
  Top_Right.spin(forward, ((Controller1.Axis3.position() - Controller1.Axis4.position() -  (Controller1.Axis1.position() / 2))) * velocityControl2, percent);
  Bottom_Right.spin(forward, ((Controller1.Axis3.position() + Controller1.Axis4.position() -  (Controller1.Axis1.position() / 2))) * velocityControl2, percent);
  if (Controller1.ButtonA.pressing()){
    Top_Left.stop(brake);
    Top_Right.stop(brake);
    Bottom_Left.stop(brake);
    Bottom_Right.stop(brake);
  }
}

void user_mode(void){
  if (gameMode == 2) {
     Controller1.Screen.print("User");
      wait(50, msec);
      Controller1.Screen.clearScreen();
  }
  userDrive = 0;
  if (Vacuum.isSpinning() == false){
    Vacuum.spin(forward, 1200, rpm);
    Brain.Screen.print("Restarting Vacuum...");
    vacuum_check;
    wait(1, seconds);
    Brain.Screen.clearScreen();
  } else{
    Brain.Screen.print("Rechecking Vacuum...");
    vacuum_check;
  }
  userDrive = 1;
  while(userDrive == 1){
    xdrive_user_control;
    if (Controller1.ButtonR1.pressing()){
      Flywheel.spin(forward, 3200, rpm);
    }
  }
}

void temperature_grab(void){
  Brain.Screen.print(Top_Left.temperature(celsius));
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print(Top_Right.temperature(celsius));
  Brain.Screen.setCursor(3, 1);
  Brain.Screen.print(Bottom_Left.temperature(celsius));
  Brain.Screen.setCursor(4, 1);
  Brain.Screen.print(Bottom_Right.temperature(celsius));
  Brain.Screen.setCursor(5, 1);
  Brain.Screen.print(Vacuum.temperature(celsius));
  Brain.Screen.setCursor(6, 1);
  Brain.Screen.print(Flywheel.temperature(celsius));
  wait(20, seconds);
  Brain.Screen.clearScreen();
}

void execute_intial_config(void){
  gameMode = 0;
  if (gameMode == 0){
    Controller1.Screen.print("Pregame");
    wait(50, msec);
    Controller1.Screen.clearScreen();
  }
  setup_x_drive;
  vacuum_configuration;
  flywheel_config;
  gameMode = 1;
}
//^^^This is some heavy abstraction however this makes it all work so don't forget it.

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  if (Controller1.ButtonDown.pressing()){
    gameMode = 0;
  }
  if (Controller1.ButtonLeft.pressing()){
    gameMode = 1;
  }
  if (Controller1.ButtonRight.pressing()){
    gameMode = 2;
  }
  if (Controller1.ButtonB.pressing()){
    temperature_grab;
  }
  execute_intial_config;
  auton_mode;
  user_mode;
}
