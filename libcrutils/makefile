PKG := dbus-glib-1

PKG_CFLAGS  := $(shell pkg-config --cflags $(PKG))
PKG_LDFLAGS := $(shell pkg-config --libs $(PKG))

CFLAGS  := -Wall $(PKG_CFLAGS) $(CFLAGS)
LDFLAGS := -Wl,--as-needed
LDLIBS  := $(PKG_LDFLAGS)


%.o: %.cc
	$(CXX) -o $@ -c $(CFLAGS) $+


test: main.o crutils.o
	$(CXX) -o $@ $+ $(LDFLAGS) $(LDLIBS)


.PHONY: all
all: test


.PHONY: clean
clean:
	rm -f *.o
	rm -f test
