TARGETS=libutil.so ptr_arr_echo buf_echo

CFLAGS+=-fPIC -g

all: $(TARGETS)

libutil.so: buf.c ptr_array.c
	$(CC) $(CFLAGS) -shared -o $@ $^

ptr_arr_echo: ptr_arr_echo.c ptr_array.o
	$(CC) $(CFLAGS) -o $@ $^

buf_echo: buf_echo.c buf.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGETS) *.o *.so *.exe *.dll
