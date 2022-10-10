#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor Top_Left = motor(PORT1, ratio6_1, false);
motor Top_Right = motor(PORT2, ratio6_1, false);
motor Bottom_Left = motor(PORT3, ratio6_1, false);
motor Bottom_Right = motor(PORT4, ratio6_1, false);
controller Controller1 = controller(primary);

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