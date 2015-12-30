myexip: myexip.o
	$(CC) -o $@ $< $(shell pkg-config --libs libcurl)

myexip.o: myexip.c
	$(CC) -o $@ -c $(shell pkg-config --cflags libcurl) $<

clean:
	rm -f myexip myexip.o
