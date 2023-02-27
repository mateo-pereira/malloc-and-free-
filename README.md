# malloc-and-free-
Test Plan:

	Our library is a redone malloc() and free(), but done an alternate way. We utilized the structure of an array in order to complete this.
Before each block of data, we used a 3-byte long chunk at the head of each block forming the Metadata, which had the status of the block and size of said block.

In order for our library to be fully functional and sufficient, it was able to carry a max size of 4096 bytes. However, depending on how big the 
chunks of data the user allocates, it could be less since it takes an extra 3 bytes per block of data for the Metadata. malloc() allows a user to 
allocate any number of bytes as long as it does not overflow the max size of 4096 bytes.It also includes free(), which deallocates any chunk of data,
and if the adjacent block(s) next to it are also free, it coalesces them. We did coalescing using only only a coalesce right function. For both free and malloc we started from 0 index and worked up each chunk by utilizing the size given by the metadata. For free we used both i and another ptr nextchunk to make coalescing easier, by having an index at the previous chunk and an index on the current chunk. 

----------------------

Correctness Tests

Error Statements:
- errors will be written with the file and line number in printf as so 
	("file":l'linenumber') -> "(%s, :l%d)", file, line)
	"Invalid Pointer(test.c:l90)"

To run correctness tests compile using the Makefile, then use ./test to run the tests.
Requirements: 
malloc()
-> allocates memory at the right places
-> allocates the right size of memory
-> does not allocate memory over other objects that had allocated memory
-> gives out an error when trying to allocate more memory than what is available
-> gives error and null pointer when trying to malloc 0 bytes

free()
-> frees the chunk and allows malloc to reuse said chunk
-> free coalesces adjacent free chunks
-> free() errors:
   will give an "Invalid Pointer" Error when:
   	1.trying to free an object that wasn't allocated by malloc
   	2. trying to free an ptr but was not that start of the object
   	3.trying to free an a ptr to an object when the object has already been freed
   	
Testing:
Error Testing is done at the end
	Error Test 1 -> showed an error for #1 on the free() error list
	Error Test 2 -> #2 on the list shows error
	Error Test 3 -> #3 on the error list
CorrectnessTesting:
	// both test 1 and test 2 have runtime errors for unalighnment for using int pointers
	Test 1 -> checks if malloc and free works.
	Test 2 -> checks if malloc returns a pointer than overlaps with another allocated object
	Test 3 -> checks if free coalescing works, 4 char ptrs totaling the max size in heap (4096)
		   2 ptrs in the middle are freed, and one of those ptrs uses malloc again to malloc a size greater 
		   than the size each of the two pointers that were originally freed.
	Test 4 -> Checks if many large objects were malloced and filled, if there is any that gets overwritten with memory from 
			another object.


-----------------------

StressTests

To run Stress tests compile using the Makefile, then use ./memgrind to run the tests.

One of the ways we checked our design by putting it under general stress tests. These tests handled most edge cases, including but not limited to:
- Filling up the entire memory to max capacity then freeing it (Extra Stress #1)
- Making sure coalescing works multiple times (Extra Stress #2)
- Randomly choosing between allocating a 1-byte chunk, or deallocating a 1-byte chunk if any are available

Another way we checked our design was by correctness, by having our library report any errors if the user tried to enter something invalid.
Our program reported these errors by using print statements.

Furthermore, we ran each task 50 times, and recorded their average time in seconds using clock():
- Stress test 1: 0.000005s
- Stress test 2: 0.000072s
- Stress test 3: 0.000063s
- Stress test 4: 0.000021s
- Stress test 5: 0.000818s
