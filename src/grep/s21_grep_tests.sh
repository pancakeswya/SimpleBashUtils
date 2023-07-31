#!/bin/bash

DIFF_FILES=""
SUCCESS_COUNT=0
FAIL_COUNT=0

./s21_grep e s21_grep.c > s21_grep.txt
grep e s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
DIFF_FILES="$(diff -s s21_grep.txt grep.txt)"
if [ "$DIFF_FILES" == "Files s21_grep.txt and grep.txt are identical" ]
  then
    (( SUCCESS_COUNT++ ))
      echo "SUCCESS"
  else
    #echo "$TEST_1" >> log.txt
      (( FAIL_COUNT++ ))
fi
rm s21_grep.txt grep.txt

./s21_grep -e "for" s21_grep.c > s21_grep.txt
grep -e "for" s21_grep.c > grep.txt
DIFF_FILES="$(diff -s s21_grep.txt grep.txt)"
if [ "$DIFF_FILES" == "Files s21_grep.txt and grep.txt are identical" ]
  then
    (( SUCCESS_COUNT++ ))
      echo "SUCCESS"
  else
    #echo "$TEST_1" >> log.txt
      (( FAIL_COUNT++ ))
fi
rm s21_grep.txt grep.txt

./s21_grep -i e s21_grep.c > s21_grep.txt
grep -i e s21_grep.c > grep.txt
DIFF_FILES="$(diff -s s21_grep.txt grep.txt)"
if [ "$DIFF_FILES" == "Files s21_grep.txt and grep.txt are identical" ]
  then
    (( SUCCESS_COUNT++ ))
      echo "SUCCESS"
  else
    #echo "$TEST_1" >> log.txt
      (( FAIL_COUNT++ ))
fi
rm s21_grep.txt grep.txt

./s21_grep -v e s21_grep.c > s21_grep.txt
grep -v e s21_grep.c > grep.txt
DIFF_FILES="$(diff -s s21_grep.txt grep.txt)"
if [ "$DIFF_FILES" == "Files s21_grep.txt and grep.txt are identical" ]
  then
    (( SUCCESS_COUNT++ ))
      echo "SUCCESS"
  else
    #echo "$TEST_1" >> log.txt
      (( FAIL_COUNT++ ))
fi
rm s21_grep.txt grep.txt

./s21_grep -c e s21_grep.c > s21_grep.txt
grep -c e s21_grep.c > grep.txt
DIFF_FILES="$(diff -s s21_grep.txt grep.txt)"
if [ "$DIFF_FILES" == "Files s21_grep.txt and grep.txt are identical" ]
  then
    (( SUCCESS_COUNT++ ))
      echo "SUCCESS"
  else
    #echo "$TEST_1" >> log.txt
      (( FAIL_COUNT++ ))
fi
rm s21_grep.txt grep.txt

./s21_grep -l e s21_grep.c s21_grep.c s21_grep.c > s21_grep.txt
grep -l e s21_grep.c s21_grep.c s21_grep.c > grep.txt
DIFF_FILES="$(diff -s s21_grep.txt grep.txt)"
if [ "$DIFF_FILES" == "Files s21_grep.txt and grep.txt are identical" ]
  then
    (( SUCCESS_COUNT++ ))
      echo "SUCCESS"
  else
    #echo "$TEST_1" >> log.txt
      (( FAIL_COUNT++ ))
fi
rm s21_grep.txt grep.txt

./s21_grep -n e s21_grep.c > s21_grep.txt
grep -n e s21_grep.c > grep.txt
DIFF_FILES="$(diff -s s21_grep.txt grep.txt)"
if [ "$DIFF_FILES" == "Files s21_grep.txt and grep.txt are identical" ]
  then
    (( SUCCESS_COUNT++ ))
      echo "SUCCESS"
  else
    #echo "$TEST_1" >> log.txt
      (( FAIL_COUNT++ ))
fi
rm s21_grep.txt grep.txt


echo "SUCCESS COUNT : $SUCCESS_COUNT"
echo "FAIL_COUNT: $FAIL_COUNT"
