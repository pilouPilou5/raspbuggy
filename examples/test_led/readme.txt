command to compile test_led.cpp

g++ -I ../../LED/NeoSPI/subprojects/yacppl/src -I ../../LED/NeoSPI/subprojects/yacppl/src/include -I ../../LED/NeoSPI/src/include \
    -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -std=c++17 -O0 -g \
    -o testLed \
    test_led.cpp ../../LED/NeoSPI/src/NeoSPI.cpp ../../LED/NeoSPI/src/D_RACE_LIB_LED.cpp \
    -L ../../LED/NeoSPI/NeoSPI_obj/subprojects/yacppl/src -L ../../LED/NeoSPI/NeoSPI_obj/src \
    -lYACPPL -lled \
    -lyaml-cpp