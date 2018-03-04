HOMEWORK 5: DSRADIO SONG GROUPS


NAME:  < Haoyun Zhu >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< Beiqi Zou >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 24 >



ORDER NOTATION:
For each of the functions below, write the order notation O().
Write each answer in terms of s = the number of songs in the library,
g = the number of song groups, a = the average number of songs by the same 
artist, and l = the average length of song groups.

AddSongToLibrary
o(s)
GroupExists
o(l)
AddToGroup
o(l)
MakeGroup
o(g)
PrintGroupRewind
o(l)
PrintLongestGroupRewind
o(l)*o(l)
RemoveGroup
o(l)+o(g)
DeleteAllSongGroups
o(l)*o(g)
RemoveFromGroup
o(g)+o(l)
PrintGroupMarathon
o(a)


TESTING & DEBUGGING STRATEGY: 
Discuss your strategy for testing & debugging your program.  
What tools did you use (gdb/lldb/Visual Studio debugger,
Valgrind/Dr. Memory, std::cout & print, etc.)?  How did you test the
"corner cases" of your implementation?

gdb & Dr. Memory

i wrote several simple test cases for corner cases like
empty lists, first/last of a list, etc to prevent segmentation fault.

MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

