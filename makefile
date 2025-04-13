build:
	gcc main.c frame.c coordlist.c genstack.c tree.c -o main
run:
	./checker-linux-amd64 -i
clear:
	rm -rf main
