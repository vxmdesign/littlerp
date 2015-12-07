OBJS=littlerp.o backgroundLayer.o imageLayer.o loadpng.o image.o key.o SliceMngr.o GCodeUtil.o dirmngr.o i2cbusses.o lcd.o lrc.o UdevMonitor.o
BIN=littlerp

CFLAGS+=-Wall -g -O3 -I../common $(shell libpng-config --cflags)
LDFLAGS+=-L/opt/vc/lib/ -lbcm_host -lm $(shell libpng-config --ldflags) -ludev

INCLUDES+=-I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux

all: $(BIN)

%.o: %.cpp
	@rm -f $@ 
	g++ $(CFLAGS) $(INCLUDES) -g -c $< -o $@ -Wno-deprecated-declarations

$(BIN): $(OBJS)
	g++ -o $@ -Wl,--whole-archive $(OBJS) $(LDFLAGS) -Wl,--no-whole-archive -rdynamic

clean:
	@rm -f $(OBJS)
	@rm -f $(BIN)
