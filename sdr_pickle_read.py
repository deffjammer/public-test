#! /usr/bin/python -u

import pprint

try:
    import cPickle as pickle
except:
    import pickle

pickleFileName = "/tmp/dfults/service0.sdrcache"

#pickleFile = open(pickleFileName, 'rb')
with open(pickleFileName, "rb") as data:
    self.data = pickle.load(data)

#bin_data = pickleFile.read()
#sio = StringIO(bin_data)
#graph_data = pickle.load(sio)
#data = pickle.load(pickleFile)
pickleFile.close()

   

