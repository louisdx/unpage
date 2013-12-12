TARGET = unpage.exe
LDFLAGS = -s
CXXFLAGS = -O2
LIBS = -lpsapi

all: $(TARGET)

.default: all
.phony: all clean

clean:
	rm -rf unpage.exe cmdline.o main.o

$(TARGET): main.o cmdline.o listprocs.o unpage.o
	$(CXX) $(LDFLAGS) -o $@ $+ $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
