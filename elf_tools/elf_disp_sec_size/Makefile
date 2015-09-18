OBJS=	elf_disp_sec_size.elf

all :	$(OBJS)

elf_disp_sec_size.elf :	elf_disp_sec_size.c
	echo "% gcc -c elf_disp_sec_size.c -Wall" > elf_disp_sec_size.comp
	gcc -c elf_disp_sec_size.c -Wall >> elf_disp_sec_size.comp 2>&1
	echo "% gcc elf_disp_sec_size.o -Wall -o elf_disp_sec_size" >> elf_disp_sec_size.comp
	gcc elf_disp_sec_size.o -Wall -o elf_disp_sec_size.elf >> elf_disp_sec_size.comp 2>&1

elfdump.exec :	elfdump elfsamp
	echo "% ./elfdump elfsamp" > elfdump.exec
	./elfdump elfsamp | head -n 3 >> elfdump.exec
	cat ../../ellipsis.txt >> elfdump.exec
	./elfdump elfsamp | head -n 37 | tail -n 11 >> elfdump.exec
	cat ../../rest.txt >> elfdump.exec
	echo "% " >> elfdump.exec

elfdump-o.exec :	elfdump elfsamp.o
	echo "% ./elfdump elfsamp.o" > elfdump-o.exec
	./elfdump elfsamp.o >> elfdump-o.exec
	echo "% " >> elfdump-o.exec

clean :
	rm -f $(OBJS) *.o *.comp
