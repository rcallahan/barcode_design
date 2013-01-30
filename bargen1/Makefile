CC=gcc
CFLAGS=   -ggdb3  
D=  
LINKERS= -lz
SOURCES=barGen_digit.c ED.c DP.c main.c
OBJECTS=${SOURCES:.c=.o}
Executable=bargen1_edit_distance_random
-include $(OBJS:.o=.d)
$(Executable): ${OBJECTS}
	${CC} -o ${Executable} ${OBJECTS} ${CFLAGS} ${LINKERS}
#	rm -f  *.d
%.o: %.c
	${CC} -c $(CFLAGS) $(D)  $*.c -o $*.o
	@${CC} -MM $(CFLAGS) $*.c > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

clean:
	rm -f ${Executable}  *.o  *.d  *~
clear:
	rm -f  *.o  *.d *~
