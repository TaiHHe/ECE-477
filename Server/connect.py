#!/usr/bin/env python3
import socket
from watchdog.observers import Observer
from watchdog.events import *
import sys
import time

class FileEventHandler(FileSystemEventHandler):
    def __init__(self, client):
        FileSystemEventHandler.__init__(self)
        self.client = client

    def send_to_esp8266(self, msg):
        try:
            self.client.send(msg.encode())
            print("message: '" + msg + "' sent")
        except:
            # self.client.send(b"")
            # print("Empty string sent")
            print("Error detected: ", sys.exc_info()[0])

    def on_modified(self, event):
        if event.is_directory:
            pass
        else:
            with open("log/log.txt", "r") as f:
                msg = f.readlines()[0]
                self.send_to_esp8266(msg)

def listen_port():
    mySocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    mySocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
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
        # print(client)
        # 80:7D:3A:75:E7:A0
        return client

def send_to_esp8266(client, msg):
    try:
        client.send(msg.encode())
        print("message: '" + msg + "' sent")
    except:
        client.send(b"")
        print("Empty string sent")


if __name__ == "__main__":
    mySocket = listen_port()
    client = connect_to_esp8266(mySocket)
    observer = Observer()
    event_handler = FileEventHandler(client)
    observer.schedule(event_handler,"/root/ECE-477/Server/log",True)
    observer.start()
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()