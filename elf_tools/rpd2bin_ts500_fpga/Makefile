OBJS=	rpd2bin_ts500_fpga

all :	$(OBJS)

rpd2bin_ts500_fpga :	rpd2bin_ts500_fpga.c
	echo "% gcc -c rpd2bin_ts500_fpga.c -Wall" > rpd2bin_ts500_fpga.comp
	gcc -c rpd2bin_ts500_fpga.c -Wall >> rpd2bin_ts500_fpga.comp 2>&1
	echo "% gcc rpd2bin_ts500_fpga.o -Wall -o rpd2bin_ts500_fpga" >> rpd2bin_ts500_fpga.comp
	gcc rpd2bin_ts500_fpga.o -Wall -o rpd2bin_ts500_fpga >> rpd2bin_ts500_fpga.comp 2>&1

clean :
	rm -f $(OBJS) *.o *.comp
