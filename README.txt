Simulate the implementation of RPC using tuple spaces. 
The idea here is to simulate not to implement at the system level, so basically you will build a multithreaded application using multiple threads (n+1 threads), n threads are those thread that simulate the client RPC calls for the operation to be performed, and the one thread is a looping server that takes client calls, and write results to the tuple space.

So, the client will place an operation, using the ‘write’ method of tuple spaces, to place a tuple that contains the operation id (unique), the operation itself, and the operation parameters (you can simulate these however way you want). 
Basically the operations are going to be the same operations that are in assignment 1, except now they are associated with an operation ID and they have simulated parameters (1, 2, or 3 of different types in the tuple space). 

Once again, the server doesn’t actually do the work, it just places the results that match the operation in the tuple space for the calling thread to unblock and take the results from the tuple space. 

The server thread will run forever, so you may consider a forked process for this if you’d like, but you don’t have to. This program must be developed in C.
