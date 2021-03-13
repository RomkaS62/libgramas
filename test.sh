for TEST in tst_*
do
	if [[ $TEST =~ \.[a-zA-Z]+$ ]]
	then
		continue
	fi
	./$TEST
	RETVAL=$?
	if [[ $RETVAL -ne 0 ]]
	then
		echo "[ERROR] Test $TEST failed. Returned with value $RETVAL"
	else
		echo "[OK] $TEST"
	fi
done
