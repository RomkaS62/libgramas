PROJECT_NAME=libutil

include config.mk

LIB_BASE_NAME=$(PROJECT_NAME)
LIB=$(LIB_BASE_NAME).so

# Install paths
SYS_LIB_DIR=$(BASE_DIR)/lib
PROJ_LIB_DIR=$(SYS_LIB_DIR)/$(LIB_BASE_NAME)
SYS_INCLUDE_DIR=$(BASE_DIR)/usr/include
PROJ_INCLUDE_DIR=$(SYS_INCLUDE_DIR)/$(LIB_BASE_NAME)

TARGETS=$(LIB) ptr_arr_echo buf_echo
CFLAGS+=-fPIC -g -Iinclude
MKDIR=mkdir -p

all: $(TARGETS)

libutil.so: buf.c ptr_array.c
	$(CC) $(CFLAGS) -shared -o $@ $^

ptr_arr_echo: ptr_arr_echo.c ptr_array.o
	$(CC) $(CFLAGS) -o $@ $^

buf_echo: buf_echo.c buf.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGETS) *.o *.so *.exe *.dll

install: $(LIB)
	$(MKDIR) $(PROJ_LIB_DIR)
	$(MKDIR) $(PROJ_INCLUDE_DIR)
	cp $(LIB) $(PROJ_LIB_DIR)
	cp include/* $(PROJ_INCLUDE_DIR)
