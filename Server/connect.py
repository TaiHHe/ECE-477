#!/usr/bin/env python3
import socket
from watchdog.observers import Observer
from watchdog.events import *
import sys
import time

class FileEventHandler(FileSystemEventHandler):
    def __init__(self):
        FileSystemEventHandler.__init__(self)
        self.listen_port()
        self.connect_to_esp8266()
        print("init here")

    def listen_port(self):
        self.mySocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.mySocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        host = "67.209.189.144"
        port = 9999
        self.mySocket.bind((host, port))
        self.mySocket.listen(10)
        print("Start listening")
        return

    def connect_to_esp8266(self):
        while True:
            print("Waiting for client connection....")
            self.client, self.address = self.mySocket.accept()
            print("New connection from:")
            print("IP: " + self.address[0])
            print("port: " + str(self.address[1]))
            # print(client)
            # 80:7D:3A:75:E7:A0
            return

    def send_to_esp8266(self, msg):
        try:
            self.client.send(msg.encode())
            print("message: '" + msg.strip() + "' sent")
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
                self.listen_port()
                self.connect_to_esp8266()


if __name__ == "__main__":
    observer = Observer()
    event_handler = FileEventHandler()
    observer.schedule(event_handler,"/root/ECE-477/Server/log",True)
    observer.start()
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()
