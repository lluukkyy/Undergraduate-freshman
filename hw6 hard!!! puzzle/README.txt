HOMEWORK 6: INVERSE WORD SEARCH


NAME:  < Haoyun Zhu >


COLLABORATORS AND OTHER RESOURCES: 
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< Beiqi Zou >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.

ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 12 hours >


ALGORITHM ANALYSIS:
What's the order notation of your algorithm?

main() = sort() + run() +O(n^3)
	sort() = O(logn)
	run() = sub_run() + unique()
		sub_run() = n + ( O( contains() ) + available_positions() + add_letters() )
			contains() = O(is_same()^2)
				is_same() = O(n)
			available_positions() = is_rword() + O(compare()^2)
				is_rword() = O(n)
				compare() = O(is_same()^2)
					is_same() = O(n)
			add_letters() = n +  O(contains()^4)
				contains() = O(is_same()^2)
				is_same() = O(n)
		unique()  = O(n^2)



TEST CASE SUMMARY:
How did your program perform on the different test cases?  Summarize
the running times.  (It's ok if it didn't finish the harder examples.)
What new test cases did you create and how did it perform on those
tests?

I thought about 2 test cases:
	1. The required word has the form of "aba":
		In that case, the reversed position would be the same as the original one.
		So I eliminated the repeated forms which phenomenoly
		decreased the running time when there were large number of words like that.  
	2. The list of required words contains pairs of words which is the reversed form 
		of each other like "abc" and "cba":
		Simply removing one of them from the required list would hugely cut off the
		amount of calculation. 

MISC. COMMENTS TO GRADER:  
Optional, please be concise!


