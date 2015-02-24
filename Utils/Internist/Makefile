
CC=/opt/mingw32ce/bin/arm-mingw32ce-gcc
CXX=/opt/mingw32ce/bin/arm-mingw32ce-g++
LD=/opt/mingw32ce/bin/arm-mingw32ce-g++
STRIP=/opt/mingw32ce/bin/arm-mingw32ce-strip
CCFLAGS=-I/opt/mingw32ce/arm-mingw32ce/include/SDL -DEV_PLATFORM_WIN32 -DUNICODE -D_UNICODE -DEV_UNSAFE_SWPRINTF -mwin32 -Os -mcpu=arm926ej-s -D_WIN32_WCE=0x600 -D_LARGEFILE_SOURCE=1 -D_LARGEFILE64_SOURCE=1 -D_FILE_OFFSET_BITS=64 -DNDEBUG
CXXFLAGS=$(CCFLAGS)
LDFLAGS=-mwin32 -mthreads -static -lceshell
TARGET=Internist.exe
OBJS=Internist.o

.PHONY:				all clean
all:				$(TARGET)
clean:				
					rm -f $(OBJS) $(TRAGET)

$(TARGET):			$(OBJS)
					$(LD) $(OBJS) -o $(TARGET) $(LDFLAGS)
					$(STRIP) $(TARGET)

Internist.o:		Internist.cpp
					$(CXX) -c $< $(CXXFLAGS)



