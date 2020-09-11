for FILE in test/*.c
do
	cc $FILE -Iinclude -o "$FILE.test" -lgramas
	$FILE.test
	if [ $? -ne 0 ]
	then
		RET=$?
		echo "Test failed: $FILE"
		return $RET
	fi
done
