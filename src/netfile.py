import pickle
import os

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

    def split(self, num_pieces):
        data = open(self._path, "r").read()
        size = len(data)
        newfiles = []
        for i in range(0, num_pieces-1):
            path = self._path + "part%i" %(i + 1)
            open(path, "w").write(data[size*i : size*(i+1)])
            newfiles.append(self._path, fid + "part%i" %(i+1), path)
        open(self._path + "part%i" %num_pieces, "w").write(data[-size:])
        os.remove(self._path)
        return newfiles

class FileTransit(object):
    ''''''

def path_to(netfile):
    return os.sep.join(netfile.get_path().split(os.sep)[:-1])

def join(netfiles):
    #return "".join(open(nf.get_path(), "r").read() for nf in netfile)
    whole_file_id = netfiles[0].get_file_id()
    with open(whole_file_id, "w") as f:
        for nf in netfiles:
            f.write(open(nf.get_path(), "r").read())
    return NetFile(whole_file_id, whole_file_id, 

def load(dbfile):
    if not dbfile:
        return []
    with open(dbfile, 'r') as datastore:
        return pickle.load(datastore)

def dump(dbfile, netfiles):
    with open(dbfile, 'w') as datastore:
        pickle.dump(datastore, netfiles)
