LIB_LINK_NAME=gramas
PROJECT_NAME=lib$(LIB_LINK_NAME)

include config.mk

LIB_BASE_NAME=$(PROJECT_NAME)
LIB=$(LIB_BASE_NAME).so

# Install paths
SYS_LIB_DIR=$(BASE_DIR)/lib
PROJ_LIB_DIR=$(SYS_LIB_DIR)
SYS_INCLUDE_DIR=$(BASE_DIR)/include
PROJ_INCLUDE_DIR=$(SYS_INCLUDE_DIR)/$(LIB_BASE_NAME)

TARGETS=$(LIB) ptr_arr_echo buf_echo utf8-echo
CFLAGS+=-fPIC -g -Iinclude
MKDIR=mkdir -p

all: $(TARGETS)

libgramas.so: buf.c ptr_array.c matrix.c cmd_parse.c gr_str.c utf8.c hash_map.c space.c
	$(CC) $(CFLAGS) -shared -o $@ $^ -lm

ptr_arr_echo: ptr_arr_echo.c ptr_array.o
	$(CC) $(CFLAGS) -o $@ $^

buf_echo: buf_echo.c buf.o
	$(CC) $(CFLAGS) -o $@ $^

utf8-echo: utf8-echo.c utf8.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGETS) *.o *.so *.exe *.dll test/*.test

run-tests: all
	CC=$(CC) CFLAGS="$(CFLAGS)" LIB_LINK_NAME=$(LIB_LINK_NAME) ./build-tests.sh
	./test.sh

install: $(LIB)
	$(MKDIR) $(PROJ_LIB_DIR)
	$(MKDIR) $(PROJ_INCLUDE_DIR)
	cp $(LIB) $(PROJ_LIB_DIR)
	cp include/* $(PROJ_INCLUDE_DIR)
