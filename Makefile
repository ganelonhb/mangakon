CXXC=g++
CC=gcc
RSRC=resources
C_FLAGS=-std=c23 -ffunction-sections -fdata-sections -Os -fomit-frame-pointer -flto=auto -fmerge-constants -fvisibility=hidden
C_DBG=-std=c23 -g -o0 -Wall -Werror
NATIVE=-march=native -mtune=native
INC=-Inotcurses/include -Iargparse/include
DBG=-std=c++2c -g -O0 -Wall -Werror
PROD=-std=c++2c -Os -ffunction-sections -fdata-sections -fomit-frame-pointer -flto=auto -Wl,-flto=auto -Wl,--gc-sections -s -Wl,--strip-all -Wl,--strip-debug -Wl,--as-needed -fmerge-constants -Wl,-Os -fvisibility=hidden -fvisibility-inlines-hidden
LIBS=-Llibs -Wl,-rpath,libs -Wl,--exclude-libs=ALL -l:libnotcurses++.a -l:libnotcurses.a -l:libnotcurses-core.a -Wl,-Bdynamic -lOpenImageIO_Util -lOpenImageIO -lncurses -lunistring -ldeflate -lgpm
OUT=mangakon
IN=*.o *.cpp
MAKE=make

NOTCURSES:
	rm -rf libs/* notcurses/build/
	mkdir -p libs
	mkdir -p notcurses/build
	cd notcurses/build && \
	cmake -DUSE_GPM=ON -DUSE_MULTIMEDIA=none -DUSE_PANDOC=OFF ../ && \
	cmake --build . --config Release
	cp notcurses/build/*.a libs/

rsrc-native:
	$(CC) $(C_FLAGS) $(NATIVE) -c $(RSRC).c -o $(RSRC).o

rsrc-dbg:
	$(CC) $(C_DBG) -c $(RSRC).c -o $(RSRC).o

rsrc:
	$(CC) $(C_FLAGS) -c $(RSRC).c -o $(RSRC).o


dbg: rsrc-dbg NOTCURSES
	$(CXXC) $(DBG) $(INC) $(IN) $(LIBS) -o $(OUT).debug

prod: rsrc NOTCURSES
	$(CXXC) $(PROD) $(INC) $(IN) $(LIBS) -o $(OUT)

native: rsrc-native NOTCURSES
	$(CXXC) $(PROD) $(INC) $(IN) $(LIBS) -o $(OUT).native

clean:
	rm -rf notcurses/build/
	rm -rf *.o

clean-hard: clean
	rm -rf libs/*
