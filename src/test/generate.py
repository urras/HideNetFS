"""Generate a file containing some junk data to test splitting."""

import string

with open("generateddata.txt", "w") as f:
    for char in string.uppercase:
        f.write(char * 80 + '\n')
