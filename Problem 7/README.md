# Problem 7: TCP Sockets: Debug Logging Server 

### Problem Description

In real world applications debug/status messages from applications are often sent to a central logging server so that any issues that may occur can be investigated as to the cause. This project creates a TCP debug logging server.

### Inputs and Outputs

#### Server

Write a console program that takes as a single command line argument the port number on which the TCP Server will listen for connection requests. Your server application needs to be able to maintain the connections from multiple clients. The clients will be sending text string messages that the server will save in a file called server.log in the same directory as the server application. The server will add a newline character to the end of each message when saving to the text file. All messages shall be appended to the server.log file if it already exists, otherwise a new file will be created. The server logs when clients connect and disconnect.

#### Client

A client console program takes as a command line argument the IP Address and port number of the server as shown below:

./a.out localhost 61717 The program should continuously prompt the user for messages to send to the server. Here is example prompting the user for messages

**Please enter a message:**

### Additional Rules:

1. Make sure that you test your server using multiple clients.

2. Make sure that your server can handle clients closing and reconnecting.

3. Make sure that both your server and client check the command line arguments for invalid entries:

   1. Negative port numbers

   2. Non numeric values

   3. Port number outside the range 61000-65535

   4. If an invalid command line is detected then the following message should be displayed showing the invalid value:

      **Invalid command line argument detected: invalid argument**

      **Please check your values and press any key to end the program!**


4. If the client application is not able to connect to the server, then client shall display the following error message:

   **Failed to connect to the server at "IP address entered" on "port num". **

   **Please check your values and press any key to end program!**