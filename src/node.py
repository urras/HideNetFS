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
        if node not in self.neighbours:
            raise KeyError("Not connection to %s from %s." %(node.address, self.address))
        self.neighbours[node].disconnect()

    def get_file(self, fileid):
        if fileid not in self.files:
            raise KeyError("No file with id %s is stored." %fileid)
        return self.files[fileid]

    def add_file(self, fileid, netf):
        if fileid in self.files:
            raise KeyError("File with id %s exists." %fileid)
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
                    print(msg) # temporary

    def send_file(self, node, netfile, maxlen=constants.DEFAULT_PACKET_SIZE, padchar='{'):
        if node not in self.neighbours:
            raise KeyError("No connection to %s from %s." %(node.address, self.address))
        partdat = "File Part %s" %netfile.get_pard_id()
        content = open(netfile.get_path(), "r").read()
        content = (padchar * (maxlen - len(content) - len(partdat))) + partdat + content
        self.neighbours[node].send(content)
