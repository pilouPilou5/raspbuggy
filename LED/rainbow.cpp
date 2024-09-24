#include <D_RACE_LIB_LED.hpp>
#include <time.h>

int main(int argc, char const *argv[]){
    ledInit(8,0);//gpio 10 pin 19
    while(1){
        rainbow3(10);
    }
    return 0;
}