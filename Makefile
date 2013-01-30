CC=gcc
CFLAGS=   -ggdb3 
D=  
LINKERS= -lz  -lm
SOURCES= bb.c
OBJECTS=${SOURCES:.c=.o}
Executable= balancer  bargen2_Hamming bargen2_Hamming_random  bargen1_edit_distance_random bargen1_edit_distance 
-include $(OBJS:.o=.d)
all:
	gcc -O3 bargen.c -o bargen2_Hamming -lz -lm
	gcc -O3 -D_RANDOM_START=1 bargen.c -o bargen2_Hamming_random -lz -lm
	gcc -O3 balancer.c -o balancer -lz -lm
	@cd bargen1 && make  clean && make && mv  bargen1_edit_distance_random   ../bargen1_edit_distance
	@cd bargen1 && make clean && make CFLAGS=-D_RANDOM_START=1  && mv  bargen1_edit_distance_random   ../bargen1_edit_distance_random
	@rm -rf  bin/
	mkdir -p bin/
	mv  bargen1_edit_distance bin 
	mv bargen1_edit_distance_random bin
	mv bargen2_Hamming bin	
	mv bargen2_Hamming_random bin
	mv balancer bin
%.o: %.c
	${CC} -c $(CFLAGS) $(D)  $*.c -o $*.o
	@${CC} -MM $(CFLAGS) $*.c > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

clean:
	rm -rf bin/
	rm -f ${Executable}  *.o  *.d  *~
clear:
	rm -f  *.o  *.d *~
