#!/usr/bin/python3.2

# Echo server program
import socket
import threading

HOST = ""                 # Symbolic name meaning all available interfaces
PORT = 5000               # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
conn, addr = s.accept()
print("Connected by", addr)

def listen():
    while 1:
        data = conn.recv(1024)
        if data:
            print("> ",data)
    #conn.sendall(data)

def send():
    while 1:
        var = input("input:$ ")
        conn.send(bytes(var,"UTF-8"))

threading.Thread(target=listen).start()
threading.Thread(target=send).start()
#conn.close()