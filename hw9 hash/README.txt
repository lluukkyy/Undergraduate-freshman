HOMEWORK 7: MINIBLAST


NAME:  < Haoyun ZHu >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< http://www.partow.net/programming/hashfunctions/ >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 8 hours >


HASH FUNCTION DESCRIPTION

the hash function mentioned in the lecture is implemented from:
 http://www.partow.net/programming/hashfunctions/






HASH TABLE IMPLEMENTATION
my hash table is a vector contains of pairs.
the index of the pair is the hash value of a certain string.
in each pair, the first element is the string itself.
the second element is a vector contains of positions where 
the piece of string appears in the original genome.

and i wrote a function add() to add each piece of kmer of the genome
to the hash table, mean while expands it if necessary






ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(order notation & concise paragraph, < 200 words)

L = length of the genome sequence
q - query length
p - number of different locations where key is found
k - key size (k-mer size)

How much memory will the map data structure require (order notation for memory use)?

O(L*p) the hash table contains 2L entries and each contains p positions

What is the order notation for performance (running time) of each of
the commands?

genome: O(lines) where lines refers to number of lines of the genome txt
table_size: O(L*L)
occupancy: O(L*L)
kmer: O((L-k)*k)
query: O(p*q)
quit: O(1)

EXTRA CREDIT
Add a new command to implement the database using one of the other
 data structures that we have covered so far in the course: 
vectors, lists, arrays etc. 

Compare the performance your alternative method to the homework method
by making a table of run times for each of the genomes and query 
sets provided with the homework  and compare the times to build
the index and the times to process the queries. 
Document any new commands you have added in your README file.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


