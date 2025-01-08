CC=g++
FLGS=-std=c++2b -Inotcurses/include -Inotcurses/src -Ithirdparty -g -lnotcurses-core -lnotcurses -lnotcurses++ -O3 -march=native -Wall
LIBS=-Llibs -Wl,-rpath,/usr/local/lib -l:libnotcurses++.a -l:libnotcurses.a -l:libnotcurses-core.a -Wl,-Bdynamic -lOpenImageIO_Util -lOpenImageIO -lncurses -lunistring -ldeflate -lgpm -lssl -lcrypto
OUT=mangakon
IN=*.cpp


dbg:
	$(CC) $(FLGS) $(IN) $(LIBS) -o $(OUT)
