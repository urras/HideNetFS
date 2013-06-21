import netfile
import constants

class Node(object):
    '''Information required by a network node to communicate.'''

    def __init__(self, ip, db_file):
        self.neighbours = {}
        self.files = netfile.load(db_file)
        self.time_since_ping = 0
        self.storage = db_file
        self.address = ip

    def connect_to(self, node, connection):
        self.neighbours[node] = connection

    def disconnect_from(self, node):
        self.neighbours[node].disconnect()

    def get_file(self, fileid):
        return self.files[fileid]

    def add_file(self, fileid, netf):
        if fileid in self.files:
            raise ValueError("File with id %s exists." %fileid)
        self.files[fileid] = netf

    def ping_alive(self):
        for connection in self.neighbours.keys():
            connection.send(constants.ALIVE_MSG %self.address)

    def handle_ping(self, src):
        self.neighbours[src].zero_ping_count()

    def handle_message(self):
        for neighbour in self.neighbours.keys():
            if self.neighbours[neighbour].has_msg():
                msg = self.neighbours[neighbour].recv()
                if msg == constants.ZERO_PING:
                    self.handle_ping(neighbour)
                else:
                    print(msg) # Just to test for now.

    def message(self, node, msg):
        if node not in self.neighbours:
            return False
        self.neighbours[node].send(msg)
        return True