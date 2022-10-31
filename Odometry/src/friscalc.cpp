/*
* Created 10/29/2022 @ 11:26 AM
* Created by alias Viridian
* Last updated: 
*/

#include "vex.h";

namespace BFris {
  
  void speedfromRPMgeneral(void){
    Velod = radIus * angVelo;
    acD = Velod * contTime;
    wVelod = woFrisKG * acD;
    nwVelod = wVelod - (Ffric1 * tTime);
 }

 int main(){
    odometry();
    angVelo = 3200;
    contTime = 0.368;
    speedfromRPMgeneral();
 }

}
