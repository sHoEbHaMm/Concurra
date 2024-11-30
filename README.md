# Concurra
*Problem Statement:* Perform the same operation using a shared variable to accumulate the final result between different threads using Mutexes & Lockguard

*Mutex:* Similar to cages with a lock. Lock the mutex before performing any operations on a shared variable. Remember to unlock it once the operation is complete.
#
*Example:* Two threads - t1 & t2 are performing an operation on the same variable. Let's say t1 gets to operate the first time. t1 will request a lock, perform the operation, write the result into memory & unlock the mutex. If t2 wanted to perform any operations on the variable during this time [before t1 actually unlocks the mutex], it couldn't because the mutex was already locked by t1. Hence, t2 has to wait before it gets unlocked to perform its operations.

## Mutex code block:
![image](https://github.com/user-attachments/assets/91fe24b4-20d0-4cdd-a665-c843cae40b01)

*Lockguards:* If you forget to unlock the mutex after operation is finished, all other threads that are requesting a lock will be denied. Eventually, no other thread in the program will be able to request a lock because of the initial failure to unlock the mutex. This will stall the program and the program will attain DEADLOCK. To prevent DEADLOCKS, mutexes can be used with a "std::lock_guard".

*What do lockguards do?:* std::lock_guard works on the principle of RAII (Resource Acquisition Is Initialization). When you use a std::lock_guard with a mutex, the mutex is automatically locked upon the creation of the lock_guard object. There's no need to explicitly unlock the mutex, as it is automatically unlocked when the lock_guard object goes out of scope.

## Mutex with Lockguard code block:
![image](https://github.com/user-attachments/assets/c4a14dc0-3421-410f-9c37-b70d5aa84993)

## Multi-Threaded result:
![image](https://github.com/user-attachments/assets/fbd94261-f5db-48bd-81d1-b7f2ebf00ace)

## Non Multi-Threaded result:
![image](https://github.com/user-attachments/assets/4e598fa7-2668-4706-9b74-2c738b20f6d7)

# Why does Non Multi-Threaded code work faster here?
- Mutexes introduce overhead due to locking and unlocking operations, which can be costly, especially in scenarios where locks are frequently acquired and released.
- If threads frequently contend for a mutex, it can lead to increased waiting time, diminishing the benefits of parallelism.
- If a significant portion of the workload is spent waiting for locks rather than doing actual work, the multithreaded program may perform worse than a single-threaded program that processes data sequentially without interruptions.
