# A Kernel Seedling
This project creates a kernel module that adds a file "count" to  /proc/ which contains the code to show the current number of running process.

## Building

`make` - to build kernel module

## Running

`sudo insmod proc_count.k` - This inserts the module into the kernel

`cat /proc/count` -  returns an integer with the number of running process

## Cleaning Up

`sudo rmmod proc_count.ko` - removes the module from the kernel

`make clean` - removes the built module object

## Testing

Linux kernel version - 5.14.8-arch1-1
`
