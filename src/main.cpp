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
competition comp;
brain Brain;
controller controlla = controller(primary);
motor motor1 = motor(PORT1, ratio36_1, false);
/*
motor [name] = motor(PORT2, ratio18_1, false);
motor [name] = motor(PORT3, ratio[?]_1, false);
motor [name] = motor(PORT4, ratio36_1, false);
pneumatics name = pneumatics(Brain.ThreeWirePort.[lettter]);
*/
bool RemoteControlCodeEnabled = true;

void usercontrol(void)
{
  Brain.Screen.setPenColor(black);
  int xPos;
  int yPos;
  Brain.Screen.setPenColor(black);
  Brain.Screen.setFillColor(red);

  while(1)
  {
    Brain.Screen.clearScreen();
    int xPos = controlla.Axis4.position()*0.75 + 245;
    int yPos = controlla.Axis3.position()*-0.75 + 120;
    Brain.Screen.drawCircle(xPos, yPos, 40);
    wait(20, msec);
  }
  // "wait" prevents overactivity
  wait(20, msec);
}

void autonomous(void)
{
  usercontrol();
}

void pre_auton(void)
{
  autonomous();
}

// Startup Sequence
int main()
{
  comp.autonomous(autonomous);
  comp.drivercontrol(usercontrol);
  pre_auton();

  while(true)
  {
    wait(100, msec);
  }
}