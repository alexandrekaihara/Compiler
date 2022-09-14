from logging import captureWarnings
from sys import argv
from subprocess import run
from json import loads

class Compiler():
    def __init__(self):
        self.__tinyProgram = 0
        self.__toAssembly = True
        self.__toOBjectCode = True

    def commandLineArguments(self):
        for arg in argv:
            if '-S' == arg: 
                self.__toOBjectCode = False
            elif '.tiny' in arg:
                self.__tinyProgram = arg
        if self.__toAssembly: self.toAssembly()
        if self.__toOBjectCode and self.semanticAnalysis("symbolstable.json"): self.toObjectCode()

    def toAssembly(self):
        run("./parser {}".format(self.__tinyProgram), shell=True, check=True)

    def toObjectCode(self):
        filename = self.__tinyProgram.replace('.tiny', '.out')
        run("python3 assembler.py program.asm {}".format(filename), shell=True, check=True)

    def semanticAnalysis(self, jsonfile: str) -> bool:
        with open(jsonfile, 'r') as f:
            json = loads(f.read())
        
        compile = True
        for key,val in json.items():
            if val['declared'] == False:
                print(f"Variable {key} not declared") 
                compile = False

        return compile

def main():
    run("rm -f *.json", shell=True, check=True)
    Compiler().commandLineArguments()

if __name__=='__main__':
    main()