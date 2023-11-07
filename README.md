# Operating-System-Assignment-3

README
Firstly, we have implemented some essential 
libraries:
This code implements a custom memory management system (MeMS) in C. The 
MeMS is designed to allocate and deallocate memory using the `mmap` and 
`munmap` system calls, and it keeps track of memory segments in a 
hierarchical structure. It's also capable of managing virtual and physical memory 
addresses.
Here are the key components and functionalities 
of this code:
1. **Header Files and Definitions**: The code includes necessary 
header files, defines `PAGE_SIZE` using the `sysconf` function, and defines data 
structures for memory segments and primary nodes.
2. **`mems_init()`**: This function initializes the MeMS system. It sets the
`headNode` and `headNode2` to `NULL`.
3. **`mems_malloc(size_t size)`**: This function is used to allocate 
memory. It first searches for a suitable free segment in the MeMS system. If a 
suitable segment is found, it is marked as a "PROCESS" segment. If not, it 
allocates a new segment from the OS using `mmap`. It calculates the required 
page size and maintains information about the allocated memory.
4. **`mems_get(void* v_ptr)`**: This function translates a MeMS 
virtual address (`v_ptr`) to a MeMS physical address. It searches through the 
MeMS hierarchical structure to find the corresponding physical address and 
returns it.
5. **`mems_free(void* po_ptr)`**: This function frees a memory 
segment by marking it as "HOLE" in the MeMS system. If the adjacent segments 
are also "HOLE," they are combined to reduce fragmentation.
6. **`mems_print_stats()`**: This function prints statistics about the 
MeMS system. It shows information about the main chain, sub-chains, the 
number of pages used, unused memory space, and the length of the main chain.
7. **`mems_finish()`**: This function is used to clean up and deallocate 
all memory used by the MeMS system. It unmaps memory using `munmap` for 
both primary nodes and memory segments.
8. **` main()`**: The main function demonstrates using the MeMS system. 
It allocates virtual addresses, accesses and modifies memory, and prints 
statistics. It also includes an example of freeing memory and reallocating it from 
the free list.
In summary, this code offers a fundamental implementation of a custom memory
management system that enables memory allocation, deallocation, and effective
memory management. It also showcases translating virtual addresses into their 
corresponding physical addresses within the MeMS system.
The output is as follows:

We used the following commands to generate the 
makefile:
# this is makefile for MeMS
all: clean example 
example: example.c mems.h
gcc -o example example.c
clean:
rm -rf example
