CC=g++
INC=-Inotcurses/include -Ithirdparty
DBG=-std=c++2b -g -O3 -march=native -Wall
PROD=-std=c++2b -O3 -ffunction-sections -fdata-sections -fomit-frame-pointer -flto=auto -Wl,--gc-sections -s -Wl,--strip-all -Wl,--strip-debug -Wl,--as-needed -fmerge-constants
LIBS=-Llibs -Wl,-rpath,libs -l:libnotcurses++.a -l:libnotcurses.a -l:libnotcurses-core.a -Wl,-Bdynamic -lOpenImageIO_Util -lOpenImageIO -lncurses -lunistring -ldeflate -lgpm -lssl -lcrypto
OUT=mangakon
IN=*.cpp


dbg:
	$(CC) $(DBG) $(INC) $(IN) $(LIBS) -o $(OUT).debug

prod:
	$(CC) $(PROD) $(INC) $(IN) $(LIBS) -o $(OUT)
