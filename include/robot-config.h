using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor LeftFront;
extern motor LeftRear;
extern motor RightFront;
extern motor RightRear;
extern motor Flywheel1;
extern motor Flywheel2;
extern motor Launch;
extern pneumatics P1;
extern brain Brain;
extern encoder EncoderA;


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );