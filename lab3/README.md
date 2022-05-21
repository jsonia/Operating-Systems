# Hash Hash Hash

In this assignment we make the hash table implementation safe to use concurrently.

## Building

`make` command is used to build the code.

## Running
`command:`./hash-table-tester -t 8 -s 20000 \
`output:`\
Generation: 107,422 usec\
Hash table base: 1,197,457 usec\
  `-` 0 missing
Hash table v1: 1,879,044 usec\
  `-` 0 missing
Hash table v2: 246,956 usec\
  `-` 0 missing`


Show an example run of your (completed) program on using the `-t` and `-s` flags
of a run where the base hash table completes in between 1-2 seconds.

## First Implementation
### Strategy
- Only one mutex lock is used.
- lock is added as soon as we enter the `hash_table_v1_add_entry()` function. Lock is released before the return statement(incase the value already exist) and before leaving the `hash_table_v1_add_entry()` .
- the lock is destroyed in the destroy() function to free the memory

This implementation uses only one mutex lock. Whenever we need to add an entry to the hastable using a thread that thread acquire the lock on the complete hash table, get the hashtable entry where the value need to be inserted, insert the value and then releases the lock.
### Why the Strategy is correct
This strategy will work as when a thread is accessing the hashtable and doing the insert operation no other thread can access the hash table and  add_entry code thus there will not be any conflicts and no issue of missing entries.


### Performance

v1 takes more time than the base implementation both with the lower number and the higher number of threads. But if we increase the number of threads keeping the amount of work same the time for v1 increases further more since in the first implementation when the thread is locking the critical section other threads can not perform any operation even if they have to insert at other hashkeys as a result the waiting time increases for each thread.
 

## Second Implementation

### Strategy
- In the second implementation, instead of having a single lock for the entire hashtable, we instead use multiple locks and add a lock per hash table entry. 
- we first get the hashtable entry using `get_hash_table_entry()` then apply the lock corresponding to that hash table entry
- lock of the corresponding entry is released after the insert operation is performed at that entry
- all the locks are destroyed in the destroy function to free the memory

### Why the Startegy is correct
Thus if the insert operation is being performed at an entry only that entry will be locked and if some other thread wants to insert at other entry they can do so by acquiring the lock at that entry. This way there will not be any conflicts in inserting and there will not be any miss and also its performance will increase in terms of speed. 



### Performance
The time taken by v2 is less than the base_time/(number_of_cores/2)

on using the total number of threads equal to the total number of cores the time for v2 reduced further more as compared to using lesser number of threads.

The v2 is taking lesser time as compared to v1 as we have added more parallelization by using multiple locks as on each insert operation we are only locking an entry instead of locking the complete hashtable. 



## Cleaning up

`make clean` this command will clean up all the binary files
