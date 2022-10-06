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

void autonomous(void)
{
}

void usercontrol(void)
{
  Brain.Screen.setPenColor(black);
  int xPos = 100;
  int yPos = 100;
  Brain.Screen.setPenColor(black);
  while(1)
  {
    Brain.Screen.clearScreen();
    int xPos = controlla.Axis4.position() + 300;
    int yPos = controlla.Axis3.position()*-1 + 100;
    Brain.Screen.setFillColor(red);
    Brain.Screen.drawCircle(xPos, yPos, 50);
    wait(20, msec);
  }

  // "wait" prevents overactivity
  wait(20, msec);
}

void pre_auton(void)
{
  usercontrol();
}

// Startup Sequence
int main()
{
  comp.autonomous(autonomous);
  comp.drivercontrol(usercontrol);
  pre_auton();

  while (true)
  {
    wait(100, msec);
  }
}