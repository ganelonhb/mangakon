CC=g++
FLGS=-std=c++2b -Inotcurses/include -Inotcurses/src -g -lnotcurses-core -lnotcurses -lnotcurses++ -O3 -march=native
LIBS=-Llibs -Wl,-rpath,/usr/local/lib -l:libnotcurses++.a -l:libnotcurses.a -l:libnotcurses-core.a -Wl,-Bdynamic -lOpenImageIO_Util -lOpenImageIO -lncurses -lunistring -ldeflate -lgpm
OUT=mangakon
IN=*.cpp


dbg:
	$(CC) $(FLGS) $(IN) $(LIBS) -o $(OUT)
