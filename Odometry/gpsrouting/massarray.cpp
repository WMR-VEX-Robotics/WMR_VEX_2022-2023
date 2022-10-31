#include "vex.h";

namespace maRelay {

    int startingPositionyd;
    int startingPositionft;
    int startingPositionin;
    int ydMatrix[4][4];
    int ftMatrix[12][12];
    int inMatrix[144][144];
    

    void set_initial_Position(void){
        startingPositionyd = ydMatrix[0][0];
        startingPositionft = ftMatrix[0][0];
        startingPositionin = inMatrix[0][0];
    }

    void if_moved(void){

    }

    void convert_freedom(void){

    }

}