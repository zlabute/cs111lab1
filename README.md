## UID: 506057506

## Pipe Up

This program acts as the pipeline operator between each of the arguments, which should be shell commands following ./pipe but does not handle flags and other inputs necesarily.

## Building

To build the program simply run the following command:

make

and then you are able to execute the program.

## Running

Run the program by calling the executable

i.e.

./pipe ls cat

should output a list of all files in the current directory

## Cleaning up

To clean up the binary files run:

make clean
