using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor Top_Left;
extern motor Top_Right;
extern motor Bottom_Left;
extern motor Bottom_Right;
extern motor Vacuum;
extern controller Controller1;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );