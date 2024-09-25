bin/main: obj/main.o obj/pwm.o obj/receive_udp.o obj/process_controller_inputs.o obj/wifi_api.o obj/thread_send_stream.o obj/gpio_play_sound.o bin/rainbow
	gcc -o bin/main obj/main.o obj/pwm.o obj/receive_udp.o obj/process_controller_inputs.o obj/wifi_api.o obj/thread_send_stream.o obj/gpio_play_sound.o $(shell pkg-config --libs gstreamer-1.0 glib-2.0) -lgpiod -lpthread -lgpio_play_sound -lsndfile -L ./sound/play_sound/src/lib

obj/main.o: main.c pwm/pwm.h receive_udp/receive_udp.h receive_udp/receive_udp.c process_controller_inputs/process_controller_inputs.h process_controller_inputs/process_controller_inputs.c hotspot_scripts/wifi_api.c hotspot_scripts/wifi_api.h send_stream/threads.h send_stream/thread_send_stream.c sound/play_sound/src/gpio_play_sound.c sound/play_sound/src/gpio_play_sound.h
	gcc -o obj/main.o -I ./pwm -I ./receive_udp -I ./process_controller_inputs -I ./hotspot_scripts -I ./send_stream -I /usr/include/gstreamer-1.0 -I /usr/include/glib-2.0 -I /usr/lib/aarch64-linux-gnu/glib-2.0/include -I ./sound/play_sound/src -L ./sound/play_sound/src/lib -c main.c

obj/pwm.o: pwm/pwm.c pwm/pwm.h
	gcc -o obj/pwm.o -I ./pwm -lgpiod -c pwm/pwm.c

obj/receive_udp.o: receive_udp/receive_udp.c receive_udp/receive_udp.h
	gcc -o obj/receive_udp.o -I ./receive_udp -c receive_udp/receive_udp.c

obj/process_controller_inputs.o: process_controller_inputs/process_controller_inputs.c process_controller_inputs/process_controller_inputs.h
	gcc -o obj/process_controller_inputs.o -I ./process_controller_inputs -c process_controller_inputs/process_controller_inputs.c

obj/wifi_api.o: hotspot_scripts/wifi_api.c hotspot_scripts/wifi_api.h
	gcc -o obj/wifi_api.o -I ./hotspot_scripts -c hotspot_scripts/wifi_api.c

obj/thread_send_stream.o: send_stream/threads.h send_stream/thread_send_stream.c
	gcc -o obj/thread_send_stream.o -I ./send_stream $(shell pkg-config --cflags gstreamer-1.0 glib-2.0) -c send_stream/thread_send_stream.c

obj/gpio_play_sound.o: sound/play_sound/src/gpio_play_sound.c sound/play_sound/src/gpio_play_sound.h
	gcc -o obj/gpio_play_sound.o -I ./sound/play_sound/src -L ./sound/play_sound/src/lib -lgpio_play_sound -lsndfile -lgpiod -c sound/play_sound/src/gpio_play_sound.c

bin/rainbow: LED/rainbow.cpp LED/NeoSPI/src/D_RACE_LIB_LED.cpp LED/NeoSPI/src/NeoSPI.cpp
	g++ -I LED/NeoSPI/subprojects/yacppl/src -I LED/NeoSPI/subprojects/yacppl/src/include -I LED/NeoSPI/src/include \
    -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -std=c++17 -O0 -g \
    -o bin/rainbow \
    LED/rainbow.cpp LED/NeoSPI/src/NeoSPI.cpp LED/NeoSPI/src/D_RACE_LIB_LED.cpp \
    -L LED/NeoSPI/NeoSPI_obj/subprojects/yacppl/src -L LED/NeoSPI/NeoSPI_obj/src -Wl,-R LED/NeoSPI/NeoSPI_obj/subprojects/yacppl/src\
    -lYACPPL -lled \
    -lyaml-cpp

clean:
	rm obj/*
	rm bin/*