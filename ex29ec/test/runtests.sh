echo "Running unit tests..."

for i in test/*_tests
do
    if test -f $i
    then
        if $VALGRIND ./$i build/libex29.so print_a_message "HI" 2>> test/tests.log
        then
            echo $i PASS
        else
            echo "ERROR in test $i: here's test/tests.log"
            echo "********"
            tail test/tests.log
            exit 1
        fi
    fi
done

echo ""
