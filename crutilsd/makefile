PKG := dbus-glib-1

PKG_CFLAGS  := $(shell pkg-config --cflags $(PKG))
PKG_LDFLAGS := $(shell pkg-config --libs $(PKG))

CFLAGS  := -Wall $(PKG_CFLAGS) $(CFLAGS) -g
LDFLAGS := -Wl,--as-needed
LDLIBS  := $(PKG_LDFLAGS)


%.o: %.c
	$(CXX) -o $@ -c $(CFLAGS) $+

%.o: %.cc
	$(CXX) -o $@ -c $(CFLAGS) $+


crutilsd: crutilsd.o config.o log.o dbus.o keytoc.o device.o
	$(CXX) -o $@ $+ $(LDFLAGS) $(LDLIBS)


.PHONY: all
all: crutilsd


.PHONY: clean
clean:
	rm -f *.o
	rm -f crutilsd
