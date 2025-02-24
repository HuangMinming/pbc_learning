.PHONY:all
all:demo
demo:demo.c
	gcc -o $@ $^ -g -I/usr/local/include/pbc -L/usr/local/lib -Wl,--rpath=/usr/local/lib -lpbc -lgmp

.PHONY:clean
clean:
	rm -fr demo