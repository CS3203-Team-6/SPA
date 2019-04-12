@echo off
title system test 1 queries

..\..\Code\Debug\AutoTester sourceDemo.txt Calls.txt out-Calls.xml
..\..\Code\Debug\AutoTester sourceDemo.txt CallsStar.txt out-CallsStar.xml

..\..\Code\Debug\AutoTester sourceDemo.txt Parent.txt out-Parent.xml
..\..\Code\Debug\AutoTester sourceDemo.txt ParentStar.txt out-ParentStar.xml

..\..\Code\Debug\AutoTester sourceDemo.txt Follows.txt out-Follows.xml
..\..\Code\Debug\AutoTester sourceDemo.txt FollowsStar.txt out-FollowsStar.xml

..\..\Code\Debug\AutoTester sourceDemo.txt Modifies.txt out-Modifies.xml
..\..\Code\Debug\AutoTester sourceDemo.txt Uses.txt out-Uses.xml


..\..\Code\Debug\AutoTester sourceDemo.txt Multiple-easy.txt out-Multiple-easy.xml

..\..\Code\Debug\AutoTester sourceDemo.txt Next.txt out-Next.xml
..\..\Code\Debug\AutoTester sourceDemo.txt NextStar.txt out-NextStar.xml

..\..\Code\Debug\AutoTester sourceDemo.txt NoClause.txt out-NoClause.xml

..\..\Code\Debug\AutoTester sourceDemo.txt Boolean.txt out-Boolean.xml

..\..\Code\Debug\AutoTester sourceDemo.txt Pattern.txt out-Pattern.xml

..\..\Code\Debug\AutoTester sourceDemo.txt WithClause.txt out-WithClause.xml

..\..\Code\Debug\AutoTester sourceDemo.txt Tuples.txt out-Tuples.xml


echo FINISHED
pause
exit