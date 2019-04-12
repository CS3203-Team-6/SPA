@echo off
title system test 2 queries

..\..\Code\Debug\AutoTester source-nesting.txt Affects.txt out-Affects.xml

..\..\Code\Debug\AutoTester source-nesting.txt Calls.txt out-Calls.xml
..\..\Code\Debug\AutoTester source-nesting.txt CallsStar.txt out-CallsStar.xml

..\..\Code\Debug\AutoTester source-nesting.txt Next.txt out-Next.xml
..\..\Code\Debug\AutoTester source-nesting.txt NextStar.txt out-NextStar.xml

..\..\Code\Debug\AutoTester source-nesting.txt Parent.txt out-Parent.xml
..\..\Code\Debug\AutoTester source-nesting.txt ParentStar.txt out-ParentStar.xml

..\..\Code\Debug\AutoTester source-nesting.txt PatternAssign.txt out-PatternAssign.xml
..\..\Code\Debug\AutoTester source-nesting.txt PatternIf.txt out-PatternIf.xml
..\..\Code\Debug\AutoTester source-nesting.txt PatternWhile.txt out-PatternWhile.xml

..\..\Code\Debug\AutoTester source-nesting.txt With.txt out-With.xml

echo FINISHED
pause
exit