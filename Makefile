PROGRAM = libpcsclite.so.1.0.0
SRCS = winscard.cpp Crypto.cpp  Decoder-ECM.cpp  Keys.cpp  Keyset.cpp
OBJS = $(subst .cpp,.o,$(SRCS))

RM := rm
CXX := g++
CC := g++

CPPFLAGS = -g -fPIC -O2 -I /usr/include/PCSC
LDFLAGS = -g -fPIC -shared

$(PROGRAM): $(OBJS)
	$(LINK.o) $^ $(LOADLIBES) -shared -o $@

.PHONY: clean

clean:
	$(RM) $(OBJS) $(PROGRAM)