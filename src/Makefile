TARGET := unpage.exe
LDFLAGS := -s
CXXFLAGS := -O2 -W -Wall -Wextra -pedantic -std=c++11
LIBS := -lpsapi
ARCHFLAGS := $(ARCHFLAGS)

OBJS := main.o cmdline.o listprocs.o unpage.o

ifdef SystemRoot
	RM = del /Q
else
	ifeq ($(shell uname), Linux)
		RM = rm -f --
	endif
endif

all: $(TARGET)

.default: all
.phony: all clean

clean:
	$(RM) $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(ARCHFLAGS) -o $@ $+ $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(ARCHFLAGS) -c -o $@ $<
