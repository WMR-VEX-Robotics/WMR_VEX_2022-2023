using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern motor Top_Left;
extern motor Top_Right;
extern motor Bottom_Left;
extern motor Bottom_Right;
extern controller Controller1;
extern signature Vision5__DISK;
extern signature Vision5__DISK2;
extern signature Vision5__DISK3;
extern signature Vision5__BLUENET;
extern signature Vision5__REDNET;
extern signature Vision5__DISK6;
extern signature Vision5__DISK7;
extern vision Vision5;
extern motor Vacuum;
extern motor Flywheel;
extern limit MagCap;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );