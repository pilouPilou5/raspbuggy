bin/main: obj/main.o obj/pwm.o obj/receive_udp.o obj/process_controller_inputs.o
	gcc -o bin/main obj/main.o obj/pwm.o obj/receive_udp.o obj/process_controller_inputs.o -lgpiod -lpthread

obj/main.o: main.c pwm/pwm.h receive_udp/receive_udp.h receive_udp/receive_udp.c process_controller_inputs/process_controller_inputs.h process_controller_inputs/process_controller_inputs.c
	gcc -o obj/main.o -I ./pwm -I ./receive_udp -I ./process_controller_inputs -c main.c

obj/pwm.o: pwm/pwm.c pwm/pwm.h
	gcc -o obj/pwm.o -I ./pwm -lgpiod -c pwm/pwm.c

obj/receive_udp.o: receive_udp/receive_udp.c receive_udp/receive_udp.h
	gcc -o obj/receive_udp.o -I ./receive_udp -c receive_udp/receive_udp.c

obj/process_controller_inputs.o: process_controller_inputs/process_controller_inputs.c process_controller_inputs/process_controller_inputs.h
	gcc -o obj/process_controller_inputs.o -I ./process_controller_inputs -c process_controller_inputs/process_controller_inputs.c

clean:
	rm obj/*
	rm bin/*