#!/bin/bash

success_test_count=0
test_count=0
flags_string="e i v c l n h s o"
IFS=' ' read -r -a flags_list <<< "$flags_string"

tests() {
    local flag="$1"
    local reg_file="$2"
    reg="char"
    local test_number=0
    echo " $reg_file"
    echo "Flag: -$flag:"
    for test_files in "./test_files/test_1.txt" "./test_files/test_2.txt ./test_files/test_2.txt ./test_files/test_2.txt" "./test_files/test_3.txt" "./test_files/test_4.txt" "./test_files/test_5.txt" "./test_files/test_6.txt" "./test_files/test_7.txt" "./test_files/test_8.txt" "./test_files/test_9.txt";
    do
        ./../grep/s21_grep -$flag $reg $test_files > 1.txt
        grep -$flag $reg $test_files > 2.txt
        if cmp -s 1.txt 2.txt ; then
            success_test_count=$(( success_test_count + 1 ))
            echo "Test $test_number: OK"
        else
            echo "Test $test_number: ERROR"
                "./s21_grep -$flag $reg $test_files > 1.txt"
        fi
        rm 1.txt 2.txt
        test_count=$(( test_count + 1 ))
        test_number=$(( test_number + 1 ))
    done
}

tests_f() {
    local flag="$1"
    local reg_file="$2"
    local test_number=0
    echo " "
    echo "Flag: -$flag:"
    for test_files in "./test_files/test_1.txt" "./test_files/test_2.txt ./test_files/test_2.txt ./test_files/test_2.txt" "./test_files/test_3.txt" "./test_files/test_4.txt" "./test_files/test_5.txt" "./test_files/test_6.txt" "./test_files/test_7.txt" "./test_files/test_8.txt" "./test_files/test_9.txt";
    do
        ./../grep/s21_grep -$flag $reg_file $test_files > 1.txt
        grep -$flag $reg_file $test_files > 2.txt
        if cmp -s 1.txt 2.txt ; then
            success_test_count=$(( success_test_count + 1 ))
            echo "Test $test_number: OK"
        else
            echo "Test $test_number: ERROR"
        fi
        rm 1.txt 2.txt
        test_count=$(( test_count + 1 ))
        test_number=$(( test_number + 1 ))
    done
}

for x in "${flags_list[@]}"; 
do
    tests "$x"
done

for x in "${flags_list[@]}"; 
do
    for y in "${flags_list[@]}"; 
    do
        flag="$x$y"
        tests "$flag"
    done
done

f="f"
for x in "${flags_list[@]}"; 
do
    flag="$x$f"
    tests_f "$flag" "./test_files/reg.txt"
done

echo " "
echo "$success_test_count / $test_count" 
echo " "
