
# Program Name: server.py
# Author: Howard Chen
# Date created: 8-1-2017
#
# Sources for this program:
#	1. https://docs.python.org/3.5/library/socket.html, for explanations of how to
#		use the socket functions
#	2. www.tutorialspoint.com/python3/python_networking.htm, for explanations of how to
#		use the socket functions
#	3. Computer Networking: A Top-Down Approach, by Kurose
#		and Ross, for examples of socket API usage

import socket
import readline
import sys

#Check the number of command line arguments. Exit with error message if the port number isn't specified.
if len(sys.argv) != 2:
	print("ERROR: need one command line argument: <port number>, got " + str(len(sys.argv) - 1) )
	sys.exit(1)

#Create the TCP socket
tcp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#get host name
server_host_name = socket.gethostname()
print("Hostname: " + server_host_name);
server_process_port = int(sys.argv[1]);

#properly bind the socket to the host and port so listening can occur
tcp_server_socket.bind((server_host_name, server_process_port))

#listen to requests, queuing up to 1 request
tcp_server_socket.listen(1)
print("Server is listening on " + str(server_process_port))
print("Press Ctrl-C (SIGINT/2) to interrupt the server process")

#Hard-coded server user handle
user_handle = "SERVER> "

#This function simply gets user input from the console, and returns it.
def getChat():
	command = input(user_handle)
	return command

#This function sends a specified string through a specified TCP socket to
# the receiver at the other end of the TCP connection
def sendChat(socket, message):
	whole_mess = user_handle + message
	socket.send(whole_mess.encode())

#Process incoming TCP requests forever, until the server process ends
while True:
	#Create a new connection socket for the client request
	new_connection_socket, client_address = tcp_server_socket.accept()
	print("Connection with other user started on port " + str(server_process_port))
	chat = ""

	#Wait for a first message from the client
	sentence = new_connection_socket.recv(1000).decode()

	#if the message wasn't to quit, print the sentence
	if sentence != "\quit":
		print(sentence)

		#While neither client nor server chooses to quit, keep sending and receiving
		#	messages
		chat = getChat()
		while chat != "\quit" and sentence != "\quit":
			sendChat(new_connection_socket, chat)
			sentence = new_connection_socket.recv(1000).decode()
			if sentence != "\quit":
				print(sentence)
				chat = getChat()

	#if server chose to quit, tell client about it before closing the connection
	if chat == "\quit": 
		new_connection_socket.send(chat.encode());	

	print("Closing the client-server connection...")
	new_connection_socket.close()

	#The server will keep listening for incoming TCP requests
