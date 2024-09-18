bin/main: obj/main.o obj/pwm.o
	gcc -o bin/main obj/main.o obj/pwm.o -lgpiod -lpthread

obj/main.o: main.c pwm/pwm.h
	gcc -o obj/main.o -I./pwm -c main.c

obj/pwm.o: pwm/pwm.c pwm/pwm.h
	gcc -o obj/pwm.o -I./pwm -c pwm/pwm.c -lgpiod

clean:
	rm obj/*
	rm bin/*