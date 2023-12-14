#!/bin/bash

#######################################################
#         Simple Shell Installation Script            #
#   Author: Victor Nwaogu <xtineviktor@gmail.com>     #
#                                                      #
#  This script builds the shell and installs it in     #
#  /usr/bin/ to make it accessible system-wide.        #
#######################################################

if [ $UID -ne 0 ]; then
    echo "You need to be root!"
    exit 1
fi

# Build the executable file
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

if [ $? -eq 0 ]; then
    # Copy the shell executable to /usr/bin/
    cp hsh /usr/bin/hsh

    # Clean up
    echo
    echo "Cleaning up..."
    # If you have a 'make clean' command, you can add it here

    echo
    echo "Success: Simple Shell is now installed! :-)"
    echo "Execute it by running: hsh"
else
    # Probably an error occurred
    echo "An error occurred while installing!"
    echo "Please try again."
fi

