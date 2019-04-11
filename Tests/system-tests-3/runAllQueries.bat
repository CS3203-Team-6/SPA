@echo off
title system test 3 queries

..\..\Code\Debug\AutoTester sourceAlphanumeric.txt Calls.txt out-Calls.xml
..\..\Code\Debug\AutoTester sourceAlphanumeric.txt CallsStar.txt out-CallsStar.xml

..\..\Code\Debug\AutoTester sourceAlphanumeric.txt Affects.txt out-Affects.xml

echo FINISHED
pause
exit
