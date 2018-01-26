select: sclient sserver

sserver: select_server.o networking.o
	gcc -g -o server select_server.o networking.o\

sclient: select_client.o networking.o
	gcc -g -o client select_client.o networking.o

client: client.o networking.o
	gcc -o -g client client.o networking.o

select_client.o: select_client.c networking.h start_screen.c final.h
	gcc -g -c start_screen.c select_client.c

client.o: client.c networking.h
	gcc -g -c client.c

select_server.o: select_server.c networking.h start_screen.c final.h
	gcc -g -c start_screen.c select_server.c

networking.o: networking.c networking.h
	gcc -g -c networking.c

clean:
	rm -f *.o
	rm -f *~
