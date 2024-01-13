all:
	gcc -g -o pub pub.c libnet/net.c
	gcc -g -o sub sub.c libnet/net.c

clean:
	rm pub sub
