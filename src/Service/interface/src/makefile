PROG=../build/interface
ASVER=$(shell cut -d" " -f7 /etc/redhat-release | sed "s/\.//")
ifeq ($(strip $(ASVER)),)
        AS=CentOS
        ASVER=$(shell cut -d" " -f3 /etc/redhat-release | sed "s/\.//")
else
	AS=as
endif
OSVER=$(shell uname -i)
ifeq ($(OSVER),x86_64)
        OSVER=x64
        MFLAGS=-Dx86_64
else
        OSVER=x32
endif
TIME:=$(shell date +'%Y%m%d')

BIN=$(PROG)_$(AS)$(ASVER)_$(OSVER)_$(TIME)

COMMONDIR=common/
MAINDIR=main/

TIME:=$(shell date +'%Y%m%d')
CC=g++
INCLUDE=-I.
LOG=-DLOGGER -D_DEBUG

CFLAGS:=-Wall -g $(INCLUDE) $(LOG) $(MFLAGS)

CPPFLAGS:=$(CFLAGS) 

LIB=-L/lib64/ -lrt -lpthread -lcrypto


COMMONSRC:=$(wildcard $(COMMONDIR)*.cpp)
MAINSRC:=$(wildcard $(MAINDIR)*.cpp)

COMMONOBJ:=$(patsubst %.cpp,%.o,$(COMMONSRC))
MAINOBJ:=$(patsubst %.cpp,%.o,$(MAINSRC))

COMMONDEP:=$(patsubst %.cpp,%.d,$(COMMONSRC))
MAINDEP:=$(patsubst %.cpp,%.d,$(MAINSRC))

CPPFLAGS += -MD

all: $(BIN)

$(BIN): $(COMMONOBJ) $(MAINOBJ) /usr/local/lib/liblog4cplus.a
	$(CC) $^ $(CFLAGS) -o $@ $(LIB) 

clean:
	rm $(COMMONDIR)/*.o $(MAINDIR)/*.o -fv
	rm $(COMMONDIR)/*.d $(MAINDIR)/*.d -fv

clear:
	rm $(COMMONDIR)/* $(MAINDIR)/* -fv

-include $(COMMONDEP) $(MAINDEP)

rebuild: clean all
