class Connection(object):
    '''Represents a link to a neighbour used to communicate across.'''

    def __init__(self, medium):
        self.medium = medium
        self.ping_count = 0

    def disconnect(self):
        # For queue medium:
        pass

    def send(self, data):
        self.medium.put(data)

    def zero_ping_count(self):
        self.ping_count = 0

    def recv(self):
        return self.medium.get()
