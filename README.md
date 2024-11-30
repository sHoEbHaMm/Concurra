# Concurra

*Problem Statement:* Shared variables need synchronization between threads. Mutexes can be used to avoid race-conditions. But they come with an additional overhead.
#
## What if each thread has its own variable to populate the result and the final result is simply the addition of these different variables?

*The variable array [4 Threads - 4 different vars] :*

![image](https://github.com/user-attachments/assets/931cbba1-44cd-40c3-a733-4af480d5e5c4)

Surprisingly, even this does not give the desired result where multi-threaded processing is quicker than single-threaded one. This is because of False Sharing.
- The Result array is a contiguous block of memory. Each of the variable, hence lies on the same cache line.
- As every core of the CPU maintains its own cache [at least L1, L2 & L3 maybe shared], an underlying cache synchronization mechanism occurs.
- Example: If t1 writes into result[0], the value at address result[0] needs to be updated.
- As this address occurs on every core's cache, it needs to be synchronized which happens under the hood.
- This slows down the program.
- A solution to this could be making sure each of these variables lie on a different cache line. As follows:
![image](https://github.com/user-attachments/assets/bcafd165-30fd-4425-8ce4-fc7c18f5e106)

*Problem Statement 2:* Realistically, performing operations on the entire dataset at the same time might not be optimal, because in a fully-fledged application resources are being used by other parts of the programs too. Moreover, data streaming might also be a possibility where we receive data to operate upon in chunks rather than whole.

## Dividing dataset into subsets:
![image](https://github.com/user-attachments/assets/94b012d7-897f-4605-a839-bce14ec2cf58)

## Multi-Threaded results of dataset ALL AT ONCE:
![image](https://github.com/user-attachments/assets/008ddfb6-ae10-4645-bb11-5803720f762d)

## Multi-Threaded results of dataset IN CHUNKS:
![image](https://github.com/user-attachments/assets/2682fdd9-1758-4747-a6bf-42ec27f3c09b)

## Why is procesing in chunks taking longer?
- In each iteration of the outer loop, you create four std::jthread objects for processing and then immediately clear the workerThreads vector, which destroys the threads after they complete.
- Creating and destroying threads repeatedly is expensive in terms of both CPU time and memory, as thread creation involves system calls and context initialization.
- Although you spawn multiple threads for parallel processing (4 in each iteration), clearing the workerThreads vector at the end of each iteration synchronizes all threads. This makes the outer loop essentially sequential because it waits for all threads to finish before starting the next iteration.
