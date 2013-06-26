from node import Node
import netfile
import constants
from connection import Connection

import socket
import sys
import os

if len(sys.argv) < 4:
    print("Run: python %s [self-ip] [friend-ip] [file-to-share] <client>" %sys.argv[0])
    print("If anything is provided for the <client> arg, this instance will be the client.")
    sys.exit(1)
if not os.path.isfile(sys.argv[3]):
    print("%s is not a file on the disk." %sys.argv[2])
    sys.exit(1)

if len(sys.argv) >= 5:
    # Connect to the friend node that is acting as the server.
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((sys.argv[2], constants.PORT))
else:
    # Create a server to listen in for the neighbour client connection.
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((sys.argv[1], constants.PORT))
    server.listen(1)
    client = server.accept()[0]

# Connect the node on this machine to its neighbour.
node = Node(sys.argv[1], None)
friend = Node(sys.argv[2], None)
node.connect_to(friend, client)

# Split the file into 10 different parts to share easily.
filename = sys.argv[3].split(os.sep)[-1]
netf = netfile.NetFile(filename, filename, sys.argv[2])
parts = netf.split(10)

if len(sys.argv) >= 5:
    for i in range(5):
        read = client.recv()
        print("received: " + read)
    for i in range(5, 10):
        node.send_file(friend, parts[i])
else:
    for i in range(5):
        node.send_file(friend, parts[i])
    for i in range(5):
        read = client.recv()
        print("received: " + read)
