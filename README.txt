README for project 1

Once the files have been copied into a directory on the FLIP servers, ssh to flip3.engr.oregonstate.edu, and then navigate to the directory containing the program files. To launch the server program, simply enter "python3 server.py <port number>" into the command line to start the server to listen at the specified port number. For example, the command "python3 server.py 5445" would start the server program, and the server would be listening on port 5445.

Next, open a separate window of your ssh utility program and ssh to flip1.engr.oregonstate.edu. Navigate to the directory containing the program files. To launch the client program, enter "make" into the command line to compile the client c program, and then enter "./prog <server hostname> <server port number>" into the command line. This will cause the client program to create a TCP connection to the server program at the specified host name and port number. For example, to connect to the example server above, we would use the command "./prog flip3.engr.oregonstate.edu 5445".

Now that the client has connected to the server, the client will prompt the user for a username. Enter the desired username. It should be no longer than 10 characters, and at least 1. The client program will then prompt the user for a message to send to the client, using the prompt "username> ", where the username is the one specified by the user. Enter a message to send it to the server.

The server will receive this message, and it will be the server user's turn to send a message to the client. The server will prompt the user for a message, using the prompt "SERVER> ". Enter a message to send to the client.

The client and server will alternate sending and receiving messages this way for the duration of the connection. However, if either the client or the server sends "\quit" (without the quotes) as its message, the TCP connection between the client and server will be closed, and the client program will exit. The server program will continue listening for new connections, and if the client program is executed again, a new connection will start.

To safely end the server program, make sure there is no connection between the client and server (by entering "\quit" if necessary). The client program should not be running. Then press Ctrl-C to send a SIGINT to the server program, and the server program will exit.

When you are done, enter "make clean" into the command line to restore the directory to its original state.
