#!/usr/bin/env python3
import socket

def listen_port():
    mySocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    mySocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    # host = socket.gethostname()
    host = "67.209.189.144"
    port = 9999
    mySocket.bind((host, port))
    mySocket.listen(10)
    print("Start listening")
    return mySocket

def connect_to_esp8266(mySocket):
    while True:
        print("Waiting for client connection....")
        client, address = mySocket.accept()
        print("New connection from:")
        print("IP: " + address[0])
        print("port: " + str(address[1]))
        print(client)
        # 80:7D:3A:75:E7:A0
    return clinet

def send_to_esp8266(msg, client):
    try:
        client.send(msg.encode())
        print("message: '" + msg + "' sent")
    except:
        client.send(b"")
        print("Empty string sent")

if __name__ == "__main__":
    mySocket = listen_port()
    client = connect_to_esp8266(mySocket)
#    while True:
#        try:
#            with open("log.txt", "r") as f:
#                msg = f.readlines()[0]
#            if(msg != ""):
#                send_to_esp8266(msg, client)
#                with open("log.txt", "w") as f:
#                    pass
#        except:
#            continue
