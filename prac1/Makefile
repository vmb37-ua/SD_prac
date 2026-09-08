PROGRAMAS = cliente servidor

CFLAGS = -Wall

todo: $(PROGRAMAS)

cliente: cliente.c
	gcc cliente.c -o cliente $(CFLAGS)

servidor: servidor.c
	gcc servidor.c -o servidor $(CFLAGS)

borrar:
	rm -f *.o $(PROGRAMAS) *~

