import constants

class Connection(object):
    '''Represents a link to a neighbour used to communicate across.'''

    def __init__(self, medium):
        self.medium = medium
        self.ping_count = 0

    def disconnect(self):
        self.medium.close()

    def send(self, data):
        self.medium.send(data)

    def zero_ping_count(self):
        self.ping_count = 0

    def recv(self, transmission_size=constants.DEFAULT_PACKET_SIZE):
        return self.medium.recv(transmission_size)
