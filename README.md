# wipe
A simple program to overwrite a filesystem's free space with zeros.

This program is used to "wipe" (overwrite with zeroes) the free space on a filesystem.
It is useful when shrinking/cloning a disk.

It creates a random named file and proceeds to fill it up with zeroes, until the filesystem is
full with it. The file is then deleted.

## usage

> wipe \<path\>

If \<path\> is omitted, ./ is used
