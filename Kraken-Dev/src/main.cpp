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
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

void setup_x_drive(void){
  
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

   if (Top_Left.efficiency() < 90 ) {
    Controller1.Screen.print("ERROR 1 IMPROPER EFFICIENCY FOR OVERCLOCK DISCONTINUE");
    wait(5, seconds);
    Top_Left.setMaxTorque(100, percent);
    Top_Left.setVelocity(600, rpm);
    Controller1.Screen.clearLine(1);

   } else {
      Controller1.Screen.print("TOP_LEFT CHECK GREEN");
    }

}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  

}
