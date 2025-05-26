CC=g++
INC=-Inotcurses/include -Iargparse/include
DBG=-std=c++2c -g -O0 -Wall -Werror
REQUIRED_LIBS=libs/libnotcurses++.a libs/libnotcurses-core.a libs/libnotcurses.a
PROD=-std=c++2c -O3 -ffunction-sections -fdata-sections -fomit-frame-pointer -flto=auto -Wl,--gc-sections -s -Wl,--strip-all -Wl,--strip-debug -Wl,--as-needed -fmerge-constants
LIBS=-Llibs -Wl,-rpath,libs -l:libnotcurses++.a -l:libnotcurses.a -l:libnotcurses-core.a -Wl,-Bdynamic -lOpenImageIO_Util -lOpenImageIO -lncurses -lunistring -ldeflate -lgpm
OUT=mangakon
IN=*.cpp
MAKE=make

$(REQUIRED_LIBS):
	rm -rf libs/* notcurses/build/
	mkdir -p libs
	mkdir -p notcurses/build
	cd notcurses/build && \
	cmake -DUSE_GPM=ON -DUSE_MULTIMEDIA=none ../ && \
	cmake --build . --config Release
	cp notcurses/build/*.a libs/

all: $(REQUIRED_LIBS)
	$(MAKE) check-libs
	$(MAKE) prod

dbg: $(REQUIRED_LIBS)
	$(CC) $(DBG) $(INC) $(IN) $(LIBS) -o $(OUT).debug

prod: $(REQUIRED_LIBS)
	$(CC) $(PROD) $(INC) $(IN) $(LIBS) -o $(OUT)

native: $(REQUIRED_LIBS)
	$(CC) $(PROD) -march=native -mtune=native $(INC) $(IN) $(LIBS) -o $(OUT).native
