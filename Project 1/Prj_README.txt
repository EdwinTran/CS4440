File #1 MyCompress.cpp:Takes an input file of 1s and 0s and if certain conditions are met compresses the file and writes the compressed version of the 1s and 0s into a new file.

File #2 MyDecompress.cpp:Takes a compressed file of 1s and 0s and expands the files to meet certain conditions.

File #3 ForkCompress.cpp:Creates a process where the child runs the compress function and the parent waits.

File #4	PipeCompress.cpp:Takes a file of 0s and 1s creates a process where the parent process will read from the file and write into the pipe while the child reads from the pipe and compress it and output the compressed version.	

File #5	ParFork.cpp: ParFork can split an input file in N number of chunks evenly and call fork to create different child processes to execute the compress function. The different child processes are then able to come together in the end to output a compressed file.

File #6	MiniShell.cpp:Created a shell like program that executes "argument-less" terminal commands such as ls and date.

File #7	MoreShell.cpp:Created a shell like program similar to MiniShell but accpected more argument terminal commands such as ls -l ~/tmp

File #8	DupShell.cpp:Added to MoreShell program and gave MoreShell the ability to run commands connected by pipes such as ls -l | wc.

File #9	ParThread.cpp:ParThread can split an input file in N number of chunks evenly and make N number of threads to compress the file. The files are then compressed individually and put together in order on to an output file.



---------- 8MB test -----------
MyCompress: .325451 seconds
ParThread: .143964 seconds