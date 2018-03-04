HOMEWORK 3: MATRIX CLASS


NAME:  < Haoyun ZHu >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< Beiqi Zou, Yiwei Chen >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 6 >



ORDER NOTATION:
For each of the functions below, write the order notation O().
Write each answer in terms of m = the number of rows and n = the
number of columns.  You should assume that calling new [] or delete []
on an array will take time proportional to the number of elements in
the array.

get O(1)

set O(1)

num_rows O(1)

get_column O(1)

operator<< O(n^2)

quarter O(n^3)

operator== O(n^2)

operator!= O(n^2)

swap_rows O(1)

rref (provided in matrix_main.cpp) O(n^3)



TESTING & DEBUGGING STRATEGY: 
Discuss your strategy for testing & debugging your program.  
What tools did you use (gdb/lldb/Visual Studio debugger,
Valgrind/Dr. Memory, std::cout & print, etc.)?  How did you test the
"corner cases" of your Matrix class design & implementation?

Dr. Memory

I tested datas that are out of the bounds of matrices and found that when rows = 0 , 
things got problematic, so i added several ifs to avoid "0 byte leak" and "unaccessible heap".


EXTRA CREDIT: 
Indicate here if you implemented resize() for extra credit.  
Also document the order notation for your resize() function.

I've implemented resize().

order notation:
resize O(n^2)



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

