for SOURCE in test/tst*.c
do
	$CC $CFLAGS $SOURCE -o ${SOURCE%%.c} -l$LIB_LINK_NAME
done
