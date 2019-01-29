lib: components.o particle.o
	gcc -fPIC -shared -o libmini.so support.o control.o object.o particle.o components.o -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

components.o: components.c components.h object.o
	gcc -g -fPIC -c components.c

particle.o: particle.c particle.h object.o
	gcc -g -fPIC -c particle.c

object.o: object.c object.h control.o
	gcc -g -fPIC -c object.c

control.o: control.c control.h support.o
	gcc -g -fPIC -c control.c

support.o: support.c support.h
	gcc -g -fPIC -c support.c

install: lib
	sudo cp -a libmini.so /usr/lib/
	sudo mkdir -p /usr/include/mini
	sudo cp -a *.h /usr/include/mini/

clear:
	rm libmini.so *.o

remove:
	sudo rm -r /usr/include/mini
	sudo rm /usr/lib/libmini.so

doc: doxygen.config
	doxygen doxygen.config
