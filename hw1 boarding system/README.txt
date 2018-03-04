HOMEWORK 1: AIRLINE SEATING


NAME:  < Haoyun Zhu >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
Piazza, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< insert collaborators / google.com; cplusplus.com >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 12 hours >



EXPLANATION OF REMOVING PASSENGERS FROM UPGRADE LIST: 
Describe the data structure you used to represent the upgrade lists,
and how you removed passengers from these lists.

My upgrade list is a vector consists of several vectors with each passenger's ID, class, flight frequency in it.
And once a passenger could not find a seat through srn, he gets into the upgrade list.
In order to remove those who updated, I create a new list for those who failed to update. And let the upgrade list equals to the new list.

MISC. COMMENTS TO GRADER:  
Optional, please be concise!


