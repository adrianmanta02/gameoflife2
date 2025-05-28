build:
	gcc main.c frame.c coordlist.c genstack.c tree.c graph.c -o main
run:
	./checker-linux-amd64 -i
bonus:
	./main ./bonus_in/databonus6.in ./bonus_out/databonus6.out
	./main ./bonus_in/databonus7.in ./bonus_out/databonus7.out
	./main ./bonus_in/databonus8.in ./bonus_out/databonus8.out
	./main ./bonus_in/databonus9.in ./bonus_out/databonus9.out
	./main ./bonus_in/databonus10.in ./bonus_out/databonus10.out
clear:
	rm -rf main
	