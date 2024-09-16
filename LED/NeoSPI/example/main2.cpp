#include <D_RACE_LIB_LED.hpp>
#include <time.h>

int main(int argc, char const *argv[])
{
    ledInit(8,0) ;
    //sleep(5) ;
    //rainbow3(5);
    //colorWipe(0,0,0,50);
    //sleep(3) ;
    //pixelSet(255,0,0,3);
    //sleep(3) ;
    //colorWipe(0,0,0,50);
    blink(255, 0, 0,500,10 );
    return 0;
}



