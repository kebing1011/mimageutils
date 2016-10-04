INCLUDE = -I /c/MinGW/include/
LIBS    = /c/MinGW/lib/libpng.a /c/MinGW/lib/libjpeg.a /c/MinGW/lib/libz.a
OBJECTS = imgtl.o rwjpg.o rwpng.o oprgb.o
TARGET  = imgtl.exe
CC      = gcc

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)
imgtl.o : imgtl.c
	$(CC) -c imgtl.c $(INCLUDE)
rwjpg.o : rwjpg.c
	$(CC) -c rwjpg.c $(INCLUDE)
rwpng.o : rwpng.c
	$(CC) -c rwpng.c $(INCLUDE)
oprgb.o : oprgb.c
	$(CC) -c oprgb.c $(INCLUDE)
clean:
	rm $(OBJECTS) $(TARGET)
install:
	install -p -m 0755 $(TARGET) /c/windows/system32
