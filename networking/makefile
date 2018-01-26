select: sclient sserver

sserver: select_server.o networking.o
	gcc -o server select_server.o networking.o\

sclient: select_client.o networking.o
	gcc -o client select_client.o networking.o

client: client.o networking.o
	gcc -o client client.o networking.o

select_client.o: select_client.c networking.h ../start_screen.c ../final.h
	gcc -c ../start_screen.c select_client.c

client.o: client.c networking.h
	gcc -c client.c

select_server.o: select_server.c networking.h ../start_screen.c ../final.h
	gcc -c ../start_screen.c select_server.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm -f *.o
	rm -f *~
