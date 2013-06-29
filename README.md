HideNetFS
=========

HNFS is an anonymized network file system for hiding ownership of sensitive information.
It works by splitting sensitive files to be stored in the network into pieces, encrypting
them, and having them distributed across the network to be stored separately by other
nodes in the network.

# Goal #
The goal of the project is to create a distributed tool to anonymize file owners so that
people with access to sensitive information can never be associated with it.

# How It Works #

![HNFS Diagram](https://raw.github.com/identityFn/HideNetFS/master/hnfsdiagram.png)

## Storing a File ##

1. Split the file into pieces.
2. Disperse the pieces to neighbours to be further distributed at random.
3. Delete file from the local machine.

## Receiving a File from Neighbour ##

1. At random, forward the file to a random neighbour (including the one received from).
2. If the file is kept, store information about it to allow recovery (id, part, path on disk).

## Retrieving Files from Network ##

1. Propagate a request over the whole network.
2. Nodes containing part of a requested file send it back via a random path along which the request was forwarded.
3. The owner waits until it has all the parts of the file and reconstructs it.

# Messages #

## Storage and Retrieval ##

[PADDING | FILE ID | PART ID | FILE CONTENT]

## Request for Retrieval ##

[PADDING | FILE ID | PADDING               ]

# On-Disk Storage #

PATH/FILEID|PARTID (| means concatenation)
