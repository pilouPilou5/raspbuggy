bin/main: obj/main.o obj/pwm.o obj/receive_udp.o obj/process_controller_inputs.o obj/wifi_api.o obj/thread_send_stream.o
	gcc -o bin/main obj/main.o obj/pwm.o obj/receive_udp.o obj/process_controller_inputs.o obj/wifi_api.o obj/thread_send_stream.o -lgpiod -lpthread

obj/main.o: main.c pwm/pwm.h receive_udp/receive_udp.h receive_udp/receive_udp.c process_controller_inputs/process_controller_inputs.h process_controller_inputs/process_controller_inputs.c hotspot_scripts/wifi_api.c hotspot_scripts/wifi_api.h send_stream/threads.h send_stream/thread_send_stream.c
	gcc -o obj/main.o -I ./pwm -I ./receive_udp -I ./process_controller_inputs -I ./hotspot_scripts -I ./send_stream -I /usr/include/gstreamer-1.0 -I /usr/include/glib-2.0 -I /usr/lib/aarch64-linux-gnu/glib-2.0/include -c main.c

obj/pwm.o: pwm/pwm.c pwm/pwm.h
	gcc -o obj/pwm.o -I ./pwm -lgpiod -c pwm/pwm.c

obj/receive_udp.o: receive_udp/receive_udp.c receive_udp/receive_udp.h
	gcc -o obj/receive_udp.o -I ./receive_udp -c receive_udp/receive_udp.c

obj/process_controller_inputs.o: process_controller_inputs/process_controller_inputs.c process_controller_inputs/process_controller_inputs.h
	gcc -o obj/process_controller_inputs.o -I ./process_controller_inputs -c process_controller_inputs/process_controller_inputs.c

obj/wifi_api.o: hotspot_scripts/wifi_api.c hotspot_scripts/wifi_api.h
	gcc -o obj/wifi_api.o -I ./hotspot_scripts -c hotspot_scripts/wifi_api.c

obj/thread_send_stream.o: send_stream/threads.h send_stream/thread_send_stream.c
	gcc -o obj/thread_send_stream.o -I ./send_stream -I /usr/include/gstreamer-1.0 -I /usr/include/glib-2.0 -I /usr/lib/aarch64-linux-gnu/glib-2.0/include -c send_stream/thread_send_stream.c

clean:
	rm obj/*
	rm bin/*