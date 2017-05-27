CC =gcc
CFLAGS = -I. -lcrypto -lfuzzy -g -pthread -I/usr/include/postgresql -lpq 
DEPS = ../../includes/entropy.h ../../includes/md5.h ../../includes/overwatch.h ../../includes/fuzzyHash.h ../../includes/pgsqldb.h
OBJ = ../../srclib/overwatch.o  ../../srclib/entropy.o ../../srclib/fuzzyHash.o ../../srclib/pgsqldb.o  ../../srclib/backupSaver.o createDatabase.o

all = createDatabase

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

createDatabase: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)


.PHONY: clean

clean:
	rm -f   createDatabase *.o ../../srclib/*.o
