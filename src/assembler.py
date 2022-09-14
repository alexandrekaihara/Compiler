from sys import argv
class Instructions:
    ADD = 'ADD'
    SUB = 'SUB'
    MULT = 'MULT'
    DIV = 'DIV'
    JMP = 'JMP'
    JMPN = 'JMPN'
    JMPP = 'JMPP'
    JMPZ = 'JMPZ'
    COPY = 'COPY'
    LOAD = 'LOAD'
    STORE = 'STORE'
    INPUT = 'INPUT'
    OUTPUT = 'OUTPUT'
    STOP = 'STOP'
    CONST = 'CONST'
    SPACE = 'SPACE'
    OPCODEADD = 1
    OPCODESUB = 2
    OPCODEMULT = 3
    OPCODEDIV = 4
    OPCODEJMP = 5
    OPCODEJMPN = 6
    OPCODEJMPP = 7
    OPCODEJMPZ = 8
    OPCODECOPY = 9
    OPCODELOAD = 10
    OPCODESTORE = 11
    OPCODEINPUT = 12
    OPCODEOUTPUT = 13
    OPCODESTOP = 14

class Assembler:
    def __init__(self):
        self.spaces = {}
        self.labels = {}
        self.object = []
    def run(self, filepath: str) -> None:
        file = self.read(filepath)
        [self.analyze(line) for line in file]
        self.addSpaces()
        self.resolveForwardReferences()
        self.saveFile(argv[2])
    def read(self, filepath: str) -> str:
        with open(filepath, 'r') as f:
            return f.read().split('\n')
    def saveFile(self, filepath) -> None:
        with open(filepath, 'w') as f:
            f.write(self.objectToString())
    def objectToString(self) -> str:
        return ' '.join([str(val) for val in self.object])
    def addSpaces(self) -> None:
        for key,val in self.spaces.items():
            self.labels[key] = self.__objectPosition()
            self.object.append(val)
    def resolveForwardReferences(self):
        def run(i):
            val = self.object[i]
            if type(val) == str:
                self.object[i] = self.labels[val]
        [run(i) for i in range(len(self.object))]
    def analyze(self, line: str) -> None:
        tokens = line.split(' ')
        if   self.__isLabel  (tokens): self.__runLabel(tokens)
        elif self.__isConst  (tokens): self.__runConst(tokens)
        elif self.__isSpace  (tokens): self.__runSpace(tokens)
        elif self.__isAdd    (tokens): self.__runAdd(tokens)
        elif self.__isSub    (tokens): self.__runSub(tokens)
        elif self.__isMult   (tokens): self.__runMult(tokens)
        elif self.__isDiv    (tokens): self.__runDiv(tokens)
        elif self.__isJmp    (tokens): self.__runJmp(tokens)
        elif self.__isJmpn   (tokens): self.__runJmpn(tokens)
        elif self.__isJmpp   (tokens): self.__runJmpp(tokens)
        elif self.__isCopy   (tokens): self.__runCopy(tokens)
        elif self.__isJmpz   (tokens): self.__runJmpz(tokens)
        elif self.__isLoad   (tokens): self.__runLoad(tokens)
        elif self.__isStore  (tokens): self.__runStore(tokens)
        elif self.__isInput  (tokens): self.__runInput(tokens)
        elif self.__isOutput (tokens): self.__runOutput(tokens)
        elif self.__isStop   (tokens): self.__runStop(tokens)
    def __objectPosition(self) -> int:
        return len(self.object) 
    def __runLabel(self, tokens: list) -> bool:
        token = tokens[0].replace(':', '')
        self.labels[token] = self.__objectPosition()
    def __runConst(self, tokens: list) -> bool:
        token = tokens[0].replace(':', '')
        self.spaces[token] = int(tokens[2])
    def __runSpace(self, tokens: list) -> bool:
        token = tokens[0].replace(':', '')
        self.spaces[token] = 0
    def __runAdd(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODEADD)
        self.object.append(tokens[1])
    def __runSub(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODESUB)
        self.object.append(tokens[1])
    def __runMult(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODEMULT)
        self.object.append(tokens[1])
    def __runDiv(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODEDIV)
        self.object.append(tokens[1])
    def __runJmp(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODEJMP)
        self.object.append(tokens[1])
    def __runJmpn(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODEJMPN)
        self.object.append(tokens[1])
    def __runJmpp(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODEJMPP)
        self.object.append(tokens[1])
    def __runJmpz(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODEJMPZ)
        self.object.append(tokens[1])
    def __runCopy(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODECOPY)
        self.object.append(tokens[1])
        self.object.append(tokens[2])
    def __runLoad(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODELOAD)
        self.object.append(tokens[1])
    def __runStore(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODESTORE)
        self.object.append(tokens[1])
    def __runInput(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODEINPUT)
        self.object.append(tokens[1])
    def __runOutput(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODEOUTPUT)
        self.object.append(tokens[1])
    def __runStop(self, tokens: list) -> bool:
        self.object.append(Instructions.OPCODESTOP)
    def __isLabel(self, tokens: list) -> bool:
        if len(tokens) == 1 and ':' in tokens[0]:
            return True
        return False
    def __isConst(self, tokens: list) -> bool:
        if len(tokens) == 3 and ':' in tokens[0]:
            return True
        return False
    def __isSpace(self, tokens: list) -> bool:
        if len(tokens) == 2 and ':' in tokens[0]:
            return True
        return False
    def __isAdd(self, tokens: list) -> bool:
        if tokens[0] == Instructions.ADD: return True
        return False
    def __isSub(self, tokens: list) -> bool:
        if tokens[0] == Instructions.SUB: return True
        return False
    def __isMult(self, tokens: list) -> bool:
        if tokens[0] == Instructions.MULT: return True
        return False
    def __isDiv(self, tokens: list) -> bool:
        if tokens[0] == Instructions.DIV: return True
        return False
    def __isJmp(self, tokens: list) -> bool:
        if tokens[0] == Instructions.JMP: return True
        return False
    def __isJmpn(self, tokens: list) -> bool:
        if tokens[0] == Instructions.JMPN: return True
        return False
    def __isJmpp(self, tokens: list) -> bool:
        if tokens[0] == Instructions.JMPP: return True
        return False
    def __isJmpz(self, tokens: list) -> bool:
        if tokens[0] == Instructions.JMPZ: return True
        return False
    def __isCopy(self, tokens: list) -> bool:
        if tokens[0] == Instructions.COPY: return True
        return False
    def __isLoad(self, tokens: list) -> bool:
        if tokens[0] == Instructions.LOAD: return True
        return False
    def __isStore(self, tokens: list) -> bool:
        if tokens[0] == Instructions.STORE: return True
        return False
    def __isInput(self, tokens: list) -> bool:
        if tokens[0] == Instructions.INPUT: return True
        return False
    def __isOutput(self, tokens: list) -> bool:
        if tokens[0] == Instructions.OUTPUT: return True
        return False
    def __isStop(self, tokens: list) -> bool:
        if tokens[0] == Instructions.STOP: return True
        return False

a = Assembler()
a.run(argv[1])