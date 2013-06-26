from node import Node
import netfile
import constants
from connection import Connection

import sys
import os

if len(sys.argv) != 4:
    print("Run: python %s [self-ip] [friend-ip] [file-to-share]" %sys.argv[0])
    sys.exit(1)
if not os.path.isfile(sys.argv[2]):
    print("%s is not a file on the disk." %sys.argv[2])
    sys.exit(1)

node = Node(sys.argv[1], None)
friend = Node(sys.argv[3], None)
filename = sys.argv[2].split(os.sep)[-1]
netf = netfile.NetFile(filename, filename, sys.argv[2])
parts = netf.split(10)

