@echo off
title system test 3 queries

..\..\Code\Debug\AutoTester sourceAlphanumeric.txt Affects.txt out-Affects.xml

..\..\Code\Debug\AutoTester sourceAlphanumeric.txt Calls.txt out-Calls.xml
..\..\Code\Debug\AutoTester sourceAlphanumeric.txt CallsStar.txt out-CallsStar.xml

..\..\Code\Debug\AutoTester sourceAlphanumeric.txt Next.txt out-Next.xml
..\..\Code\Debug\AutoTester sourceAlphanumeric.txt NextStar.txt out-NextStar.xml

..\..\Code\Debug\AutoTester sourceAlphanumeric.txt NoCondition.txt out-NoCondition.xml

echo FINISHED
pause
exit