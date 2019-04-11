@echo off
title system test 2 queries

..\..\Code\Debug\AutoTester source-nesting.txt Calls.txt out-Calls.xml
..\..\Code\Debug\AutoTester source-nesting.txt CallsStar.txt out-CallsStar.xml

..\..\Code\Debug\AutoTester source-nesting.txt Parent.txt out-Parent.xml
..\..\Code\Debug\AutoTester source-nesting.txt ParentStar.txt out-ParentStar.xml

..\..\Code\Debug\AutoTester source-nesting.txt Affects.txt out-Affects.xml

echo FINISHED
pause
exit