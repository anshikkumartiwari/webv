CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk+-3.0 webkit2gtk-4.1)
LIBS   = $(shell pkg-config --libs gtk+-3.0 webkit2gtk-4.1)
TARGET = webv

all:
	$(CC) $(CFLAGS) browser.c -o $(TARGET) $(LIBS)
	strip $(TARGET)

install: all
	# Create the destination directory if it doesn't exist
	install -d $(DESTDIR)/usr/bin
	install -m 755 $(TARGET) $(DESTDIR)/usr/bin/$(TARGET)

clean:
	rm -f $(TARGET)

distclean: clean
	@echo "No extra distclean steps."
