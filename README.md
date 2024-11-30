# Concurra

*Problem Statement:* Shared variables need synchronization between threads. Mutexes can be used to avoid race-conditions. But they come with an additional overhead.
#
## What if each thread has its own variable to populate the result and the final result is simply the addition of these different variables?

## The variable array [4 Threads - 4 different vars] :
![image](https://github.com/user-attachments/assets/931cbba1-44cd-40c3-a733-4af480d5e5c4)

Surprisingly, even this does not give the desired result where multi-threaded processing is quicker than single-threaded one. This is because of False Sharing.
- The Result array is a contiguous block of memory. Each of the variable, hence lies on the same cache line.
- As every core of the CPU maintains its own cache [at least L1, L2 & L3 maybe shared], an underlying cache synchronization mechanism occurs.
- Example: If t1 writes into result[0], the value at address result[0] needs to be updated.
- As this address occurs on every core's cache, it needs to be synchronized which happens under the hood.
- This slows down the program.
- A solution to this could be making sure each of these variables lie on a different cache line. As follows:
![image](https://github.com/user-attachments/assets/bcafd165-30fd-4425-8ce4-fc7c18f5e106)

