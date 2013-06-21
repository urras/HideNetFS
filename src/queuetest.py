import Queue
import threading

from node import Node
from connection import Connection
import netfile
import constants

class NodeThread(threading.Thread):
    def __init__(self, node):
        threading.Thread.__init__(self)
        self.node = node
        self.running = True

    def run(self):
        while self.running:
            self.node.handle_message()

    def terminate(self):
        self.running = False

def main():
    # Create a simple star network (clique).
    nodes = [Node(str(i), None) for i in range(5)]
    for i in range(len(nodes)):
        for j in range(len(nodes)):
            if i == j: 
                continue
            q = Queue.Queue()
            nodes[i].connect_to(nodes[j], Connection(q))
    # Put node in a thread to have it read out messages.
    threads = [NodeThread(nodes[i]) for i in range(len(nodes))]
    for thread in threads:
        thread.start()
    # Allow the user to send messages from nodes.
    while 1:
        if raw_input("Would you like to quit? [Y/n]: ") in ['Y', 'y']:
            break
        for i in range(len(nodes)):
            response = raw_input("Send a message from Node%i? [Y/n]" %i)
            if response not in ['Y', 'y']:
                continue
            receiver = int(raw_input("Id of receiver: [0..%i]: " %(len(nodes) - 1)))
            if receiver < 0 or receiver >= len(nodes):
                print("Invalid Id.")
                continue
            if nodes[i].message(nodes[receiver], raw_input("Enter message: ")):
                print("Message sent successfully.")
            else:
                print("Message failed to send.")
    # Tell all the threads to stop   
    for thread in threads:
        thread.terminate()
    for thread in threads:
        thread.join()
    print("Finished.")

if __name__ == "__main__":
    main()
