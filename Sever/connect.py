import socket

def listen_port():
    mySocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    mySocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    host = socket.gethostname()
    port = 9090
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
        if ('192.168.0.19' in address[0]):
            print("esp8266 connected")
            return client
        else:
            continue

def send_to_esp8266(msg, client):
    try:
        client.send(msg.encode())
        print("message: '" + msg + "' sent")
    except:
        client.send(b"")

# def send_to_esp8266(msg, client):
#     mySocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#     host = socket.gethostname()
#     port = 8080
#     mySocket.bind((host, port))
#     mySocket.listen(10)
#     while True:
#         print("Waiting for client connection....")
#         client, address = mySocket.accept()
#         print("New connection from:")
#         print("IP: " + address[0])
#         print("port: " + str(address[1]))
#         if ('116' in address[0]):
#             client.send(msg.encode())
#             print("message: '" + msg + "' send to " + str(address[0]))
#             break
#         else:
#             continue

