import unicodedata
# -*- coding: utf-8 -*-

import os
import subprocess
import sys



PATH = '.'
userinput= input("nom ou chemin du dossier a archiver ")
if userinput != '.' and userinput != './' and userinput != '':
    PATH = PATH + '/'+ userinput
a = os.listdir(PATH)
fichierfinal = input("nom de l'archive ? ")

print("archivage par recursion dans : "+PATH)
filelist=[]
filepath=[]
def run(cmd):
    proc = subprocess.Popen(cmd,
        stdout = subprocess.PIPE,
        )
    stdout = proc.communicate()
 
    return stdout

def recursion (l, path, fl):   
    for key in l:
        if os.path.isdir(path+'/'+key): 
            b = os.listdir(path+'/'+key)
            recursion(b, path+'/'+key, fl)
        elif os.path.isfile(path+'/'+key):
            if key != "script.py":
                filepath.append(path+'/'+key)
                fl.append(key)
            


recursion(a, PATH, filelist)

print (filelist)
print (filepath)
fichierfinal = fichierfinal+ '.txt'

with open(fichierfinal, 'w') as outfile:
    for fname in filepath:
        with open(fname) as infile:
            outfile.write(fname)
            outfile.write('\n')
            for line in infile:
                outfile.write(line)
            outfile.write('\n')
huffmanarg=[]
huffmanarg.append('./huf')
huffmanarg.append(fichierfinal)
huffmanarg.append(fichierfinal+'.huf')


dehuffmanarg=[]
dehuffmanarg.append('./dehuf')
dehuffmanarg.append(fichierfinal+'.huf')

huff = subprocess.Popen(huffmanarg)
huff.wait()
huff.terminate()

#dehuff = subprocess.Popen(dehuffmanarg)
#dehuff.wait()
#dehuff.terminate()

