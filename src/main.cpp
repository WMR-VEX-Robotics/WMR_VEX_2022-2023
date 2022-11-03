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
controller controller1 = controller(primary);
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
  while(true)
    {
      FrontRight.spin(forward, ((controller1.Axis3.value() - controller1.Axis1.value() - controller1.Axis4.value())/2), percent);  
      BackRight.spin(forward, ((controller1.Axis3.value() - controller1.Axis1.value() + controller1.Axis4.value())/2), percent);
      FrontLeft.spin(forward, ((controller1.Axis3.value() + controller1.Axis1.value() + controller1.Axis4.value())/2), percent);
      BackLeft.spin(forward, ((controller1.Axis3.value() + controller1.Axis1.value() - controller1.Axis4.value())/2), percent);
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

  while (true)
  {
    wait(100, msec);
  }
}