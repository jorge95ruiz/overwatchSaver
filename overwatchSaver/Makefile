FILES = src/*.c srclib/*.c includes/*.h
INPUT =  src/*.c srclib/*.c
OBJ = obj/
OS = *.o 
CFLAGS = -I. -lcrypto -lfuzzy -g -pthread -I/usr/include/postgresql -lpq 

ALL: monitoring move

monitoring: $(OS)
	gcc -o monitoring $(OS) $(CFLAGS)

move:
	@mv -fu *.o ./obj/ &> /dev/null


$(OS) : $(FILES) 
	gcc -c  $(CFLAGS) $(INPUT) 

.PHONY: clean

clean:
	rm -f monitoring *.o obj/*.o *.txt
