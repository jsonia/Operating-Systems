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

This implementation uses only one mutex lock. Whenever we need to add an entry to the hastable using a thread that thread acquire the lock on the complete hash table, get the hashtable entry where the value need to be inserted, insert the value and then releases the lock.
### Why the Strategy is correct
This strategy will work as when a thread is accessing the hashtable and doing the insert operation no other thread can access the hash table and  add_entry code thus there will not be any conflicts and no issue of missing entries.


### Performance

There is a speedup by 0.92 times on using low number of threads as compared to using high number of threads.

since in the first implementation when the thread is locking the critical section other threads can not perform any operation even if they have to insert at other hashkeys thus if we increase the number of threads waiting time for each thread will increase and overall time will increase with high number of threads.

Run the tester such that the base hash table completes in 1-2 seconds.
Report the relative speedup (or slow down) with a low number of threads and a
high number of threads. Note that the amount of work (`-t` times `-s`) should
remain constant. Explain any differences between the two.

## Second Implementation

In the second implementation, instead of having a single lock for the entire hashtable, we instead add a lock per hash table entry. Thus if the insert operation is being performed at an entry only that entry will be locked and if some other thread wants to insert at other entry they can do so by acquiring the lock at that entry. This way there will not be any conflicts in inserting and there will not be any miss and also its performance will increase in terms of speed. 


Describe your second implementation strategy here (the one with a multiple
mutexes). Argue why your strategy is correct.

### Performance

Run the tester such that the base hash table completes in 1-2 seconds.
Report the relative speedup with number of threads equal to the number of
physical cores on your machine (at least 4). Note again that the amount of work
(`-t` times `-s`) should remain constant. Report the speedup relative to the
base hash table implementation. Explain the difference between this
implementation and your first, which respect why you get a performance increase.

## Cleaning up

`make clean` this command will clean up all the binary files
