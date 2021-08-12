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


#!/usr/bin/python


#Ehm, sorry for the totally unreadable "code". The definition of the
#keybindings.conf file syntax started out to be very simple, but after a while..



"""
Example Syntax:

CTRL_LEFT F5: RAO_SetKeyOctave 24
F3: RAO_SetKeyOctave 12


F5:
CTRL_L CTRL_R SHIFT_L SHIFT_R ALT_L ALT_R META_L META_R MOUSE_L MOUSE_R

  x       x                      x                   x                      RAO_SetKeyOctave 24

F5: RAO_SetKeyOctave 24
CL CR SL SR AL AR ML MR ML MR MM
a  a     a           x  x


CL CR SR MR ML F5: RAO_SetKeyOctave 24

CL CR SR MR ML F5 F6: RAO_SetKeyOctave 24

CL CR SR MR ML F5 F6: *
RAO_SetKeyOctave(24)
*



 


"""

import sys,string,os
import traceback
import platform

try:
    is_mocked = sys.radium_is_mocked
except:
    is_mocked = False


if __name__!="__main__" and not is_mocked:
  import radium as ra
else:
  sys.argv = ["", "keybindings.conf", ""]
  sys.path = [os.path.join(os.path.expanduser("~"), ".radium")] + sys.path
  sys.path = [os.path.abspath(os.path.dirname(sys.argv[0]))] + sys.path
  class Mock:
    def getConfPath(self, key):
      return os.path.join(os.getenv("HOME"),".radium",key)
    def hasConfPath(self, key):
      return True
    def addMessage(self, message):
      print "ra.addMessage: "+message
    def addKeybinding(self, command, something):
        print "ra.addKeybinding: " + command + " - " + something
        
  ra = Mock()
  sys.g_program_path = ""


import protoconfparser
from types import *



true=1
false=0

class NullWriter(object):
    def write(self, value): pass


def makeemptylist(len):
    ret=[]
    for lokke in range(len):
        ret.append([])
    return ret


#import profile


#/* Keyboard Sub IDs: (picked from common/nsmtracker_events.h (must be the same)) */

from keysubids import *




def tuple_has_key(tuple,string):
    for lokke in range(len(tuple)):
        if tuple[lokke]==string:
            return lokke
    return -1


def key_is_qualifier(key):
    #print "key_is_qualifier? "+str(key)+" : "+keysub[key]
    return key!=0 and key<tuple_has_key(keysub,"FIRST_NON_QUALIFIER")


def isSpace(char):
    if char==" " or char=="\t": return true
    return false


class LineParser:
    def __init__(self,line,defines):
        self.parts=[]
        insidestring=false
        partstart=0

        line=string.lstrip(line)

        lokke=0
        while lokke<len(line):
            if line[lokke]==":":
                if lokke+1<len(line) and line[lokke+1]!=" ":
                    line=line[:lokke+1]+" "+line[lokke+1:]
                if line[lokke-1]!=" ":
                    line=line[:lokke]+" "+line[lokke:]
                break
            lokke+=1

        #print "LINE: " + line
        for lokke in range(len(line)):
            dasline=line[lokke]
            if dasline=="\t":
                dasline=" "
            if dasline=="\"" and insidestring==false:
                #self.parts.append(line[lokke:])
                if lokke>0:
                    if insidestring==false:
                        insidestring=true
                        partstart=lokke
                    else:
                        insidestring=false
                else:
                    insidestring=true
                    partstart=lokke

            else:
                if lokke>0 and not insidestring:
                    dasprevline=line[lokke-1]
                    if dasprevline=="\t":
                        dasprevline=" "
                    if dasprevline==" " and dasline!=" ":
                       partstart=lokke
                    if dasprevline!=" " and dasline==" ":
                       self.parts.append(line[partstart:lokke])
        if not isSpace(line[lokke]):
            self.parts.append(line[partstart:lokke+1])

        new_parts = []
        for part in self.parts:
            new_parts.append(part.replace("<string>","\\\""))
        self.parts=new_parts
        
        #print "PARTS:", self.parts
        
        lokke=0
        while lokke<len(self.parts):
            if defines.has_key(self.parts[lokke]):
                defdef=defines[self.parts[lokke]]
                if defdef[0]==0:
                    defadd=LineParser(defdef[2],defines).getParts()
                    self.parts=self.parts[:lokke]+defadd+self.parts[lokke+1:]
                else:
                    defparts=defdef[2][:]
                    defargparts=defdef[1][1]
                    for lokke2 in range(len(defargparts)):
                        for lokke3 in range(len(defargparts[lokke2])):
                            defparts[defargparts[lokke2][lokke3]]=self.parts[lokke+lokke2+1]
                    defstring=""
                    for lokke2 in range(len(defparts)):
                        defstring+=defparts[lokke2]+" "
                    defadd=LineParser(defstring,defines).getParts()
                    self.parts=self.parts[:lokke]+defadd+self.parts[lokke+1+len(defparts):]
            lokke+=1
        return

    def getParts(self):
        return self.parts

    def getLine(self):
        line=""
        for lokke in range(len(self.parts)):
            line+=self.parts[lokke]+" "
        return line

class Parser:
    def __init__(self, filehandle1, filehandle2 = None):
        self.linetype="NORMAL"
        self.filehandle1=filehandle1
        self.filehandle2=filehandle2
        self.linenum=0
        self.keys=[]
        self.defines={}
        self.defines_var={}
        self.nextline=""
        self.parts=[]
        self.parser=0
        self.outlinenum=0
        self.mouseEditorKey = tuple_has_key(keysub,"MOUSE_EDITOR")
        self.mouseMixerKey = tuple_has_key(keysub,"MOUSE_MIXER")
        self.mouseMixerStripsKey = tuple_has_key(keysub,"MOUSE_MIXERSTRIPS")
        self.mouseSequencerKey = tuple_has_key(keysub,"MOUSE_SEQUENCER")
        self.focusEditorKey = tuple_has_key(keysub,"FOCUS_EDITOR")
        self.focusMixerKey = tuple_has_key(keysub,"FOCUS_MIXER")
        self.focusMixerStripsKey = tuple_has_key(keysub,"FOCUS_MIXERSTRIPS")
        self.focusSequencerKey = tuple_has_key(keysub,"FOCUS_SEQUENCER")
        
    def readLine(self):
        self.linenum+=1
#        print "----------------line: %d " % self.linenum
        return self.filehandle1.readline()
    
    def readNextLine(self):
        if self.nextline!="":
            self.currline=self.nextline
            self.nextline=""
        else:
            self.currline=self.readLine()

        if self.currline=='':
            self.filehandle1.close()
            
            if self.filehandle1 == self.filehandle2:
                return False

            if self.filehandle2 is None:
                return False

            self.filehandle1 = self.filehandle2
            self.linenum = 0
            return self.readNextLine()

        self.currline=string.rstrip(self.currline)

        while self.currline=="" or self.currline=="\n" or self.currline[0:1]=="#":
            return self.readNextLine()

        if self.currline[0:4]=="?out":
            self.outlinenum+=1
            print "  --->%d. %s" % (self.outlinenum,self.currline[4:])
            return self.readNextLine()

        if len(self.currline)>0 and self.currline[len(self.currline)-1]=="\n":
            self.currline=self.currline[:-1]

#        self.currline+="\n"
#        if len(self.currline)>1 and self.currline[len(self.currline)-1]!="\n":


        if self.currline[:7]=="?define":
            if self.currline[:8]=="?define ":
                parts=string.split(self.currline)
                lokke=0;hits=0
                while hits<2:
                    if self.currline[lokke]==" ":
                        while self.currline[lokke]==" ":
                            lokke+=1
                        hits+=1
                    lokke+=1
                self.defines[parts[1]]=[0,[],self.currline[lokke-1:]]
            else:
                lokke=8
                while self.currline[lokke]!=")":
                    lokke+=1
                args=string.split(self.currline[8:lokke],",")

                self.currline=self.currline[lokke+2:]
                parts=string.split(self.currline)
                key=parts.pop(0)
                argplaces=[]
                for lokke2 in range(len(args)):
                    argplaces.append([])
                    for lokke in range(len(parts)):
                        if parts[lokke]==args[lokke2]:
                            argplaces[lokke2].append(lokke)

                self.defines[key]=[len(args),[args,argplaces],parts]
            return self.readNextLine()

        if self.currline[len(self.currline)-1]=="\n":
            self.currline=self.currline[:-1]

        self.currlineorg=self.currline[:]
        self.parser=LineParser(self.currline,self.defines)
        self.currline=self.parser.getLine()

        for lokke in range(len(self.currline)):
            if self.currline[lokke:lokke+2]=="?n":
                self.nextline=self.currline[lokke+3:]
                self.currline=self.currline[:lokke]
                if self.currline!="":
                    self.parser=LineParser(self.currline,self.defines)
                else:
                    return self.readNextLine()                    
                break

        return true
            
    def nextLine(self):
        if not self.readNextLine():
            return "ENDFILE"
        
        if self.currline[0:1]=='*':
            if self.linetype=="GOINGTOINSERTCODE":
                self.linetype="INSERTCODELAST"
            else:
                if self.linetype=="INSERTCODE":
                    self.linetype="INSERTCODELAST"
                else:
                    if self.linetype=="INSERTCLEANCODE":
                        self.linetype="INSERTCLEANCODELAST"
                    else:
                        self.linetype="GOINGTOINSERTCLEANCODE"
                
        else:
            if self.linetype=="GOINGTOINSERTCODE":
                self.linetype="INSERTCODE"
            if self.linetype=="GOINGTOINSERTCLEANCODE":
                self.linetype="INSERTCLEANCODE"
            if self.linetype=="INSERTCODELAST" or self.linetype=="INSERTCLEANCODELAST":
                self.linetype="NORMAL"
            if self.linetype=="NORMAL" or self.linetype=="SKIP":
                if self.currline[-2:]=="* ":
                    self.linetype="GOINGTOINSERTCODE"

                parts=self.parser.getParts()

                keys=[]
                self.command=[]
                for lokke in range(len(parts)):
                    if parts[lokke]==":":
                        keys=parts[:lokke]
                        self.command=parts[lokke+1:]
                        break
                lokke=0
                while lokke<len(keys):
                    key=tuple_has_key(keysub,keys[lokke])
                    if key==-1:
                        key=tuple_has_key(qualsub,keys[lokke])
                        if(key!=-1):
                            key+=tuple_has_key(keysub,"CTRL_L")
                    if key==-1:
                        message = "Unknown key \""+keys[lokke] +"\" in line %d in keyconfig file." % self.linenum
                        print message
                        ra.addMessage(message)
                        del keys[lokke]
                        self.linetype="ERROR"
                        return "OK"
                    else:
                        keys[lokke]=key
                        lokke+=1

                keys.sort()
                new_keys = []
                self.qualifiers = []
                
                for key in keys:
                    if key_is_qualifier(key):
                        self.qualifiers.append(key)
                    else:
                        new_keys.append(key)

                new_keys.sort()
                self.qualifiers.sort()
                
                if len(keys)>len(self.qualifiers): # ????????
                    self.keys=new_keys
                        
                if self.linetype!="GOINGTOINSERTCODE":
                    if len(self.command)==0:
                        self.linetype="SKIP"
                    else:
                        self.linetype="NORMAL"

                #print "------------------------------------------>"
                #print "command: %s" % self.command
                #print "self.qualifiers: %s" % str(map(lambda k:keysub[k], self.qualifiers))
                #print "self.keys: %s " % str(map(lambda k:keysub[k], self.keys))
                #print "<------------------------------------------"

        return "OK"
    

    def getLineType(self):
        return self.linetype

    def getNumElements(self):
        self.part=string.split(self.currline)
        return len(self.part)

    def getCurrLine(self):
        return self.currlineorg

    def getKeys(self):
        return self.keys[:]

    def mouseInQualifiers(self):
        if self.mouseEditorKey in self.qualifiers:
            return True
        elif self.mouseMixerKey in self.qualifiers:
            return True
        elif self.mouseMixerStripsKey in self.qualifiers:
            return True
        elif self.mouseSequencerKey in self.qualifiers:
            return True
        else:
            return False;
        
    def focusInQualifiers(self):
        if self.focusEditorKey in self.qualifiers:
            return True
        elif self.focusMixerKey in self.qualifiers:
            return True
        elif self.focusMixerStripsKey in self.qualifiers:
            return True
        elif self.focusSequencerKey in self.qualifiers:
            return True
        else:
            return False;

    def getFocusQualifiers(self):
        ret = []
        
        if self.focusEditorKey in self.qualifiers:
            ret += [self.focusEditorKey]
        if self.focusMixerKey in self.qualifiers:
            ret += [self.focusMixerKey]
        if self.focusMixerStripsKey in self.qualifiers:
            ret += [self.focusMixerStripsKey]
        if self.focusSequencerKey in self.qualifiers:
            ret += [self.focusSequencerKey]
            
        if not ret:
            ret += [self.focusEditorKey]
            ret += [self.focusMixerKey]
            ret += [self.focusMixerStripsKey]
            ret += [self.focusSequencerKey]
            
        return ret
    
    def getMouseQualifiers(self):
        ret = []
        
        if self.mouseEditorKey in self.qualifiers:
            ret += [self.mouseEditorKey]
        if self.mouseMixerKey in self.qualifiers:
            ret += [self.mouseMixerKey]
        if self.mouseMixerStripsKey in self.qualifiers:
            ret += [self.mouseMixerStripsKey]
        if self.mouseSequencerKey in self.qualifiers:
            ret += [self.mouseSequencerKey]
            
        if not ret:
            ret += [self.mouseEditorKey]
            ret += [self.mouseMixerKey]
            ret += [self.mouseMixerStripsKey]
            ret += [self.mouseSequencerKey]
            
        return ret
    
    def getQualifiers(self):
        #print "qualifiers:", self.qualifiers
        return self.qualifiers[:]

    def getQualifiersWithoutFocusAndMouse(self):
        #print "qualifiers:", self.qualifiers
        ret = []
        for qualifier in self.qualifiers:
            if qualifier == self.focusEditorKey:
                continue
            if qualifier == self.focusMixerKey:
                continue
            if qualifier == self.focusMixerStripsKey:
                continue
            if qualifier == self.focusSequencerKey:
                continue

            if qualifier == self.mouseEditorKey:
                continue
            if qualifier == self.mouseMixerKey:
                continue
            if qualifier == self.mouseMixerStripsKey:
                continue
            if qualifier == self.mouseSequencerKey:
                continue

            ret += [qualifier]
            
        return ret

    def getCommands(self):
        return self.command

    def getCurrLineNum(self):
        return self.linenum
        

def putCode(keyhandles,parser,commands,added_qualifiers):
    keys=parser.getKeys()+parser.getQualifiersWithoutFocusAndMouse()+added_qualifiers
    firstkey=keys.pop(0)

    command = " ".join(commands)
    pythoncall = commands[0] + "(" + ",".join(commands[1:]) + ")"
    
    #print "addkeybinding:", commands, keysub[firstkey] + " " + " ".join(sorted(map(lambda x:keysub[x],keys))), pythoncall
    ra.addKeybinding(command, keysub[firstkey] + " " + " ".join(sorted(map(lambda x:keysub[x],keys))))

    
    #pythoncall=commands.pop(0)
    #pythoncall+="("
    #while len(commands)>1:
    #    pythoncall+=commands.pop(0)+","
    #    if len(commands)>0:
    #        pythoncall+=commands.pop(0)
    #    pythoncall+=")"

    if pythoncall=="ra.undo()":
        print "adding \"%s\", line: %d, firstkey: %d, keys: %s" % (pythoncall,parser.getCurrLineNum(),firstkey,keys)
        
    if keyhandles[firstkey].addHandle(keys,compile(pythoncall,'<string>','single'))==False:
        # Note. This doesn't happen anymore. Redefining a keybinding is allowed.
        message = "Keybindings for command \"%s\" in line %d is already used" % (pythoncall , parser.getCurrLineNum())
        print message
        ra.addMessage(message)
        return False
        
    return True


def printsak(file,keyhandles,parser,codestring):
    if 1:
        keys=parser.getKeys()+parser.getQualifiers()
        firstkey=keys.pop(0)
        print "Putting code for '"+codestring+"', with key "+keysub[firstkey]
        if len(keys)>0:
            print " And qualifiers: "+keysub[keys[0]]
    print codestring
    print parser.getKeys()
    print parser.getQualifiers()
    print


def addIt2(keyhandles, parser, commands, extra):
    for focusQualifier in parser.getFocusQualifiers():
        putCode(keyhandles, parser, commands, extra + [focusQualifier])
    
    
def addIt(keyhandles, parser, commands):
    for mouseQualifier in parser.getMouseQualifiers():
        addIt2(keyhandles, parser, commands, [mouseQualifier])



class KeyHandler:

    def __init__(self):
        self.keyslist=[]
        self.handlers=[]

    def addHandle(self,keys,handle):
        for lokke in range(len(self.keyslist)):
            if self.keyslist[lokke]==keys:
                if platform.system() != "Linux":
                    print "NOTE. Overriding",self.keyslist,"keybinding."
                    #ra.addMessage("Overriding the " + str(self.keyslist) + " keybinding.")
                    #return False

        keys.sort()
        self.keyslist.insert(0,keys)
        self.handlers.insert(0,handle)
        return True
            
    # keys is a constant
    def exe(self,windownum,keys):
        #print "keys",keys, ". len(self.keyslist):", len(self.keyslist)
        for lokke in range(len(self.keyslist)):
            #print "keyslist[lokke]",self.keyslist[lokke], map(lambda k:keysub[k], self.keyslist[lokke])
            if self.keyslist[lokke]==keys:
                #print "evaling",self.handlers[lokke]
                eval(self.handlers[lokke])
                return True
        return False


def getKeyHandler(num):
    return KeyHandler()


keyhandles = False

def resetKeyHandles():
    global keyhandles
    keyhandles=map(getKeyHandler,range(len(keysub)))

resetKeyHandles()

# key and keys are constants!
def gotKey(windownum,key,keys):
    try:
        global keyhandles
        #print "*********** key: " + keysub[key] + ". keys: " + str(map(lambda k:keysub[k], keys))
        return keyhandles[key].exe(windownum,keys);    
    except:
        traceback.print_exc(file=sys.stdout)
        ra.addMessage("Keybinding error.\n\nBacktrace:<pre>" + traceback.format_exc() + "</pre>")
        return True

filename1 = ""
filename2 = ""

def init(filename1_2, filename2_2):
    global filename1
    global filename2
    filename1 = filename1_2
    filename2 = filename2_2
    

def get_file_handles():
    try:
        infilehandle=open(sys.argv[1],'r')
    except:
        print "Cant open1 -%s-" % sys.argv[1]
        ra.addMessage("Cant open %s" % sys.argv[1])
        sys.exit(1)

    try:
        infilehandle2=open(sys.argv[2],'r')
    except:
        print "Cant open2 -%s-" % sys.argv[2]
        infilehandle2 = None

    #print "Cant open %s" % sys.argv[2], infilehandle2
    #sys.exit(-1)

    try:
        outfilehandle=open(os.path.join(os.path.expanduser("~"), ".radium", "generated_keybinding_code.py"),'w+')
        outfilehandle.write("#Do not edit. This file is automatically generated from keybindings.config.\n")
    except:
        message = "Could not write to \"" + os.path.join(os.path.expanduser("~"), ".radium", "generated_keybinding_code.py") + "\"."
        print message
        ra.addMessage(message)
        outfilehandle = False

    return infilehandle, infilehandle2, outfilehandle

def clean_generated_keybinding_file():
    filehandle, filehandle2, outfilehandle = get_file_handles()
    if outfilehandle:
        outfilehandle.close()

clean_generated_keybinding_file() # If not, we will run previous keybinding in the next line. If it contained e.g. "*exit()*", radium wouldn't start. But it could also create other types of confusion.

#if not is_mocked: # When running the binaries on ubuntu 20, the import line below didn't work unless the "if not is_mocked:" conditional was removed.
import generated_keybinding_code as keybinding

def parse():
    global keyhandles,radium

    filehandle, filehandle2, outfilehandle = get_file_handles()
    
    parser=Parser(filehandle,filehandle2)
    defnum=0
    reader=protoconfparser.Read()
    
    while parser.nextLine()!="ENDFILE":

        if outfilehandle:
            if parser.getLineType()=="GOINGTOINSERTCODE":
                outfilehandle.write("def radium_generated_keycodedef%d():\n" % defnum)
            if parser.getLineType()=="INSERTCODE":
                outfilehandle.write(parser.getCurrLine()+"\n")
            if parser.getLineType()=="INSERTCODELAST":
                #outfilehandle.write("\treturn\n")
                outfilehandle.write("\n")
                addIt(keyhandles,parser,"keybinding.radium_generated_keycodedef%d()" % defnum)
                defnum+=1
            if parser.getLineType()=="INSERTCLEANCODE":
                outfilehandle.write(parser.getCurrLine()+"\n")

        if parser.getLineType()=="ERROR":
            return False

        if parser.getLineType()=="NORMAL":
            commands=parser.getCommands()


            lokke=2
            while lokke<len(commands):
                if commands[lokke][0]=="+":
                    add=int(commands.pop(lokke)[1:])
                    commands[lokke-1]="%d" % (int(commands[lokke-1])+add)
                elif commands[lokke][0]=="-":
                    add=int(commands.pop(lokke)[1:])
                    commands[lokke-1]="%d" % (int(commands[lokke-1])-add)
                elif commands[lokke][0]=="*":
                    add=int(commands.pop(lokke)[1:])
                    commands[lokke-1]="%d" % (int(commands[lokke-1])*add)
                elif commands[lokke][0]=="/":
                    add=int(commands.pop(lokke)[1:])
                    commands[lokke-1]="%d" % (int(commands[lokke-1])/add)
                lokke+=1

            command = commands[0]
            for c in commands[1:]:
                command += " " + c

            element = [map(lambda x:keysub[x],parser.getKeys()),
                       sorted(map(lambda x:keysub[x],parser.getQualifiers()))]

            addIt(keyhandles, parser, commands)

    try:
        if filehandle:
            filehandle.close()
            
        if filehandle2:
            filehandle2.close()
            
        if outfilehandle:
            outfilehandle.close()
           
    except:
        print "Could not close file. Out of disk space?"
        #sys.exit(3)

    if not is_mocked:
        reload(keybinding) # i.e re-do "import generated_keybinding_code as keybinding"

    #sys.exit(1)
    
    return True





def parse_and_show_errors():
    global keyhandles
    
    old_stdout = sys.stdout
    old_stderr = sys.stderr
    if platform.system() != "Linux": # and os.isatty(sys.stdout.fileno()):
        sys.stdout = NullWriter()
        sys.stderr = NullWriter()

    ret = False
    
    old_keyhandles = keyhandles
    try:
        resetKeyHandles()
        ret = parse()
    except:
        print sys.exc_info()
        #radium.addMessage("Couldn't create keybindings dict. ("+str(sys.exc_info())+")")
        message = traceback.format_exc()
        print message
        #radium.addMessage("Loading "+filename+" failed.") # If this is a valid module file, please send it to k.s.matheussen@notam02.no ("+str(e)+")")
        #        for m in message.split("\n"):
        ra.addMessage("Couldn't parse keybindings file.\n\nBacktrace:<pre>"+message+"</pre>")
        keyhandles = old_keyhandles

    if platform.system() != "Linux": # and os.isatty(sys.stdout.fileno()):
        sys.stdout = old_stdout
        sys.stderr = old_stderr

    return ret

if __name__=="__main__":
    parse_and_show_errors()
    
