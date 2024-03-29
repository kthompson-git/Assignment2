Programming Assignment 2
Due date: Monday, 28 October 2019, 11:59 PM
Requested files: client.cpp, server.cpp (Download)
Type of work: Individual work
Reduction by automatic evaluation: 5 Free evaluations: 10
COSC 3360 – Operating System Fundamentals 
Assignment #2 for Fall 2019: The distributed compression algorithm. 
Similarity Threshold: 85%

### Objective  
This assignment will introduce you to interprocess communications mechanisms in UNIX using sockets.

### Specifications:  
You must write two programs to implement a distributed version of the compression algorithm proposed by Rincon et al. in 2008 (A Probabilistic Compression Algorithm based on Symbols Position). These programs are:

##### The client program:

The user will execute this program using the following syntax:
./exec_filename hostname port_no < input_filename
where exec_filename is the name of your executable file, hostname is the address where the server program is located, port_no is the port number used by the server program, and input_filename is the name of the file with the message to compress.
After calculating the frequency of each symbol in the message, your program must create a thread per each different symbol in the message. After the child threads finish processing the information, they will write the generated codes on a memory location available to the main thread. Finally, the main thread will print the information (step by step) about the compression process.

The child threads will execute the following tasks:

1. Send the message and the symbol to the server program using sockets. 
2. Wait for the generated code from the server program.
3. Write the received bit sequence into a memory location accessible by the main thread.
4. Finish its execution.

Input Format: Your program should read its input from stdin (C++ cin) and use input redirection. 
 
This input file will only have letters, numbers, and the EOL character (that will be represented as <EOL>). 
 
###### The server program:

The user will execute this program using the following syntax:

` ./exec_filename port_no `

where exec_filename is the name of your executable file, and port_no is the port number to create the socket.

This program receives requests from the child threads of the client program using sockets. It will create a child process per request. For this reason, the parent process needs to handle zombies processes implementing the fireman() call in the primer. Each child process will generate the bit sequence of the requested symbol, returning the result to the client program using sockets.

Example:
For a message equal to “aaaannnaaakkllaaaaap”, the corresponding output is:     

a frequency = 12  
n frequency = 3  
k frequency = 2  
l frequency = 2  
p frequency = 1  
Original Message:       aaaannnaaakkllaaaaap  
Symbol a code:		11110001110000111110  
Remaining Message:	nnnkkllp  
Symbol n code:		11100000  
Remaining Message:	kkllp  
Symbol k code:		11000  
Remaining Message:	llp  
Symbol l code:		110  
Remaining Message:	p  
Symbol p code:		1  
 
### HINTS: 

- You can safely assume that the input files will always be in the proper format and that the maximum number of symbols used to create a message is 100.
- You must use the format from the previous example when writing information into STDOUT.
- The parent will use the frequency of the symbol as a priority to sort the symbols used by the algorithm (the higher the frequency, the higher the priority). If two or more symbols have the same frequency, the parent process will use the ASCII value of the symbols as a priority to break the tie (the lowest the ASCII value, the highest the priority).


These specifications were written on Wednesday, September 29, 2019. Please refer to the course web site for corrections and updates. 

