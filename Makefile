all:
	gcc -g -o pub pub.c lib/net.c
	gcc -g -o sub sub.c lib/net.c

clean:
	rm pub sub
