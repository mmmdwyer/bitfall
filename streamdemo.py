#!/usr/bin/python3

# This is a sample program to give you an idea of what the COINSIDES
# config does to the output.
#
# If you give a single number, it'll use that as the COINSIDES value,
# where a larger number yields a sparser output. Default is 2.
#
# If you give it a second number, it will use that as the fraction of
# a second to wait between frames. Larger numbers yield faster animation.
# Default is 20.  Unfortunately, this doesn't coincide with the FRAMEDELAY
# setting inside the Arduino code, but it will give you an idea of what
# various speed settings do for the visual effect.
#
# python3 streamdemo.py       # Run the default coinsides=2, 1/20th sec
# python3 streamdemo.py 16    # Run with coinsides=16, 1/20th sec delay
# python3 streamdemo.py 16 50 # Run with coinsides=16, 1/50th sec delay

import array
import random
import time
import sys
import os

# Gets the width of the terminal
try:
    termwidth=os.get_terminal_size()[0]-1
finally:
    termwidth=79

bitmap = [0] * termwidth
pos = 0

coinsides = 2
if len(sys.argv) > 1:
    coinsides = int(sys.argv[1])

delayfrac = 1.0/20
if len(sys.argv) > 2:
    delayfrac = 1.0/int(sys.argv[2])


while True:
    for i in range(0,termwidth):
        if bitmap[((pos+i) % termwidth)] == 0:
            print('*',end="")
        else:
            print(' ',end="")
    bitmap[pos] = (random.randint(0,255) % coinsides);
    pos = ((pos + 1) % termwidth );
    print ('',end='\r',flush=True)
    time.sleep(delayfrac)


