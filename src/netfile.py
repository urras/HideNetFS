import pickle

class NetFile(object):
    '''A file that will be stored on the network.'''

    def __init__(self, fid, partid, ptr):
        self._file_id = fid
        self._part_id = partid
        self._path = ptr

    def get_file_id(self):
        return self._file_id

    def get_part_id(self):
        return self._part_id

    def get_path(self):
        return self._path

    file_id = property(get_file_id, doc='Identification code of the file')
    part_id = property(get_part_id, doc='Identification code of the file part')
    path = property(get_path, doc='Path to the file in local storage')

def load(dbfile):
    if not dbfile:
        return []
    with open(dbfile, 'r') as datastore:
        return pickle.load(datastore)

def dump(dbfile, netfiles):
    with open(dbfile, 'w') as datastore:
        pickle.dump(datastore, netfiles)
