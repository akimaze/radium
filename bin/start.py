#/* Copyright 2001 Kjetil S. Matheussen
#
#This program is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public License
#as published by the Free Software Foundation; either version 2
#of the License, or (at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program; if not, write to the Free Software
#Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. */

import sys,os
print sys

print sys.argv[0]
print "program path:",sys.g_program_path
print "sys.path:",sys.path

sys.path = [os.path.abspath(os.path.dirname(sys.argv[0]))] + sys.path

#import platform

#if platform.system() != "Linux" and platform.system() != "mingw":
#    sys.path = [sys.path[0], os.path.join(sys.path[0],"python2.7")]
#    sys.path = sys.path + [os.path.join(sys.path[1],"lib-dynload")]

print "sys.path now:",sys.path
#import platform
#print "platform:",platform.system()

import traceback

import radium,keybindingsparser
#import keybindingsparser

from common import *

ra=radium

class KeyHandler:

    def __init__(self):
        self.keyslist=[]
        self.handlers=[]

    def addHandle(self,keys,handle):
        for lokke in range(len(self.keyslist)):
            if self.keyslist[lokke]==keys:
                return false

        self.keyslist.append(keys)
        self.handlers.append(handle)
        return true
            
# keys is a constant!
    def exe(self,windownum,keys):
        for lokke in range(len(self.keyslist)):
            if self.keyslist[lokke]==keys:
                try:
                    eval(self.handlers[lokke])
                except:
                    traceback.print_exc(file=sys.stdout)
                return True
                break
        return False


def getKeyHandler(num):
    return KeyHandler()

keyhandles=map(getKeyHandler,range(len(keybindingsparser.keysub)))

# key and keys are constants!
def gotKey(windownum,key,keys):
    #print "*********** key: " + keybindingsparser.keysub[key] + ". keys: " + str(map(lambda k:keybindingsparser.keysub[k], keys))
    #    key=keys.pop(0)
    return keyhandles[key].exe(windownum,keys);    


try:
    infilehandle=open(sys.argv[1],'r')
except:
    print "Cant open %s" % sys.argv[1]
    sys.exit(1)

try:
    infilehandle2=open(sys.argv[2],'r')
except:
    print "Cant open %s" % sys.argv[2]
<<<<<<< HEAD
    #sys.exit(-1)
=======
>>>>>>> a105554ea110e1111e6c71b8791b067e7119b37d
    infilehandle2 = None

    #print "Cant open %s" % sys.argv[2], infilehandle2
    #sys.exit(-1)

try:
    outfilehandle=open("eventreceiverparser_generated.py",'w+')
    outfilehandle.write("#Do not edit. This file is automatically generated from keybindings.config.\n")
except:
    print "Cant open eventreceiverparser_generated.py for writing"
    outfilehandle = False
    pass

print "Parsing keybindings.conf..."
#profile.run("KeyConfer(infilehandle,outfilehandle)","fooprof")
if keybindingsparser.start(keyhandles,infilehandle,infilehandle2,outfilehandle,)==false:
    sys.exit(5)

try:
    outfilehandle.close()
except:
    print "Could not close file. Out of disk space?"
    #sys.exit(3)

import eventreceiverparser_generated

#import os
#os.system("/usr/bin/givertcap")


pid = os.getpid()

if hasattr(os,'fork') and os.fork()==0:
    import signal,time
    def signal_handler(signalnum, frame):
        print "You pressed Ctrl+C. Sending SIGINT signal to radium."
        os.kill(pid,signal.SIGINT)
        sys.exit(0)
    signal.signal(signal.SIGINT, signal_handler)
    while True:
        #print os.getppid(),pid,"hmm"
        #sys.stdout.flush()
        if os.getppid() != pid:
            #print "Seems like parent process died. Exiting Ctrl+C process"
            sys.exit(0)
        time.sleep(1)
    #signal.pause()


if len(sys.argv)>3:
    ra.init_radium(sys.argv[3],gotKey)
else:
    ra.init_radium("",gotKey)
    
