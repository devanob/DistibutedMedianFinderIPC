# DistibutedMedianFinderIPC
This Distributed Median Finder Using IPC And MutiProcesses
Abstract :
The design and development of the Inter process communication using pipes program. The program was written in C to run under the Unix based operating system.
The program uses process to implement the median finding algorithms.
The program involves the use of pipes to return requested values from child processes to the parent and the communication between each child and parent process. The report includes the design process and the pseudo-code for the program as well as some results
Design :
The project assigned us with the task to implement distributed median finding algorithm using pipes. The parent process spawns 5 identical child process with the use of 10 pipes. Two pipes for each child-parent pair. Each child reads an array of 5 integers, the numbers are read from 5 files called input_i.txt where i is from 0-4.
