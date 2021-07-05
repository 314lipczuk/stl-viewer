import sys
import re
import struct
def determineType():
    try:
        f = open(sys.argv[1], 'r')
        name = f.readline()
        if 'solid' in name:
            return "ascii"
        else:
            return "bin"
        f.close()
    except:
        print("reading fail")

tris = []
curtri = None
class triangle:
    def __init__(self):
        self.face =[]
        self.vs = []
    def isProper(self):
        if len(self.face) != 3 or len(self.vs) != 3:
            return False
        for v in self.vs:
            if len(v)!= 3:
                return False
        return True

def parseAsciiLine(line):
    global curtri
    if ('normal' in line):
        curtri = triangle()
        crds = re.findall('\-*\d+\.\d+', line)
        for c in crds:
            curtri.face.append(float(c))
        print(curtri.face)
    if 'vertex' in line:
        crds = re.findall('\-*\d+\.\d+', line)
        for c in crds:
            c = float(c)
        curtri.vs.append(crds)
    if 'endfacet' in line:
        if curtri.isProper():
            tris.append(curtri)
            curtri.face = []
            curtri.vs = []
        else:
            raise NameError('not actually nameerror')

def parseAscii():
    f = open(sys.argv[1], 'r')
    for line in f.readlines():
        print(line)
        parseAsciiLine(line)
    f.close()
    print(len(tris))

def parseBin():
    data = open(sys.argv[1], 'rb').read()   
    trinumdat = data[80:85]
    trinum = int.from_bytes(trinumdat, "little", signed=True)
    print(trinum)
    for x in range(trinum)
    
if determineType() == "ascii":
    parseAscii()
else:
    parseBin()
