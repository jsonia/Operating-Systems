# Hey! I'm Filing Here

This assignment is about building ext2 file system

## Building

`make` command is used to build the file system

## Running

`make` compile the executable \
` ./ext2-create` run the executable to create cs111-base.img \
`dumpe2fs cs111-base.img` dumps the filesystem information to help debug \
`fsck.ext2 cs111-base.img` this will check that  filesystem is correct \
 `mkdir mnt` create a directory to mnt  filesystem to \
`sudo mount -o loop cs111-base.img mnt` mount the filesystem , loop lets us use a file \

example output of `ls -ain`  mounted filesystem. \
total 7 \
     2 drwxr-xr-x 3    0    0 1024 Jun  3 05:44 .  \
942318 drwxr-xr-x 3 1000 1000 4096 Jun  3 05:44 ..  \
    13 lrw-r--r-- 1 1000 1000   11 Jun  3 05:44 hello -> hello-world. \
    12 -rw-r--r-- 1 1000 1000 1024 Jun  3 05:44 hello-world \
    11 drwxr-xr-x 2    0    0 1024 Jun  3 05:44 lost+found



## Cleaning up
`sudo umount mnt` unmount the filesystem when  done \
`rmdir mnt` delete the directory used for mounting when  done \
`make clean` to clean all the binary files


