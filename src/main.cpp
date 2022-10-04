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
competition Competition;
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

void pre_auton(void)
{
  usercontrol()
}

void autonomous(void)
{
}

void usercontrol(void)
{
  while (1)
  {
    controlla.Axis1.value()

    // "wait" prevents overactivity
    wait(20, msec);
  }
}

// Startup Sequence
int main()
{
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();

  while (true)
  {
    wait(100, msec);
  }
}