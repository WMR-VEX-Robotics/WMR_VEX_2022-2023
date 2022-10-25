#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor Top_Left = motor(PORT1, ratio18_1, false);
motor Top_Right = motor(PORT2, ratio18_1, false);
motor Bottom_Left = motor(PORT3, ratio18_1, false);
motor Bottom_Right = motor(PORT4, ratio18_1, false);
controller Controller1 = controller(primary);
/*vex-vision-config:begin*/
signature Vision5__DISK = signature (1, 2127, 2473, 2300, -5239, -4555, -4897, 3, 0);
signature Vision5__DISK2 = signature (2, -1, 627, 313, -4755, -4337, -4546, 3, 0);
signature Vision5__DISK3 = signature (3, 3059, 3601, 3330, -4511, -3537, -4024, 3, 0);
signature Vision5__BLUENET = signature (4, -2629, -2009, -2319, 8013, 11397, 9705, 3, 0);
signature Vision5__REDNET = signature (5, 9291, 10507, 9899, -1489, -797, -1143, 3, 0);
signature Vision5__DISK6 = signature (6, 0, 0, 0, 0, 0, 0, 3, 0);
signature Vision5__DISK7 = signature (7, 0, 0, 0, 0, 0, 0, 3, 0);
vision Vision5 = vision (PORT5, 44, Vision5__DISK, Vision5__DISK2, Vision5__DISK3, Vision5__BLUENET, Vision5__REDNET, Vision5__DISK6, Vision5__DISK7);
/*vex-vision-config:end*/
motor Vacuum = motor(PORT6, ratio6_1, false);
motor Flywheel = motor(PORT7, ratio6_1, false);
limit MagCap = limit(Brain.ThreeWirePort.A);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}