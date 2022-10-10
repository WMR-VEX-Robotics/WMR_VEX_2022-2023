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
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

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
  string printEfficiency = "Average Efficiency= " + avgEfficiency;
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
    int breakFirst = 1;
   } else {
      Controller1.Screen.print("CHECK GREEN");
      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.print(printEfficiency);
      wait(1, seconds);
      Controller1.Screen.clearScreen();
      int breakFirst = 0;
    }
  //this prevents the robot from running at the specified overclock if the efficiency is not balanced

  bool avgCurrent = ((Top_Left.current() + Top_Right.current() + Bottom_Left.current() + Bottom_Right.current()) / 4);
  string printCurrent = "Average Current= " + avgCurrent;
  Controller1.Screen.print(printCurrent);
  wait(1, seconds);
  Controller1.Screen.clearScreen();
  //this just tells you how much current you are pulling

  bool avgPower = ((Top_Left.power() + Top_Right.power() + Bottom_Left.power() + Bottom_Right.power()) / 4);
  string printPower = "Average power= " + avgPower;
  Controller1.Screen.print(printPower);
  wait(1, seconds);
  Controller1.Screen.clearScreen();
  //same as current but with power
  break_Factors;
}

void vacuum_configuration(void) {
  Vacuum.current();
  Vacuum.effciency();
  Vacuum.power();
  Vacuum.setMaxTorque(250, percent);
  Vacuum.setVelocity(1200, rpm);

  if (Vacuum.efficiency() < 90) {
    Vacuum.setMaxTorque(100, percent);
    Vacuum.setVelocity(600, rpm);
    Controller1.Screen.print("ERROR 2 VACUUM OVERCLOCK NOT POSSIBLE");
    int breakSecond = 1;
    wait(1, seconds);
    Controller1.Screen.clearScreen();
  } else {
    Controller1.Screen.print("CHECK GREEN");
    int breakSecond = 0;
    wait(1, seconds);
    Controller.Screen.clearScreen();
  }

  Vacuum.spin(0, degrees);
  break_Factors;
}

void break_Factors(void){
  if (breakFirst == 1){
    do{
      break;
    }
  } else {
    continue;
  }

  if (breakSecond == 1){
    do {
      break;
    }
  } else {
    continue;
  }
}

void steps_to_execute(void){
  setup_x_drive;
  vacuum_configuration;
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  

}
