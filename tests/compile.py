# 2019 Emir Erbasan(humanova)
# GPL v2 License, see LICENSE for more details

import subprocess
import glob

def Compile(path):

    cmd = f"basm {path}"
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    process.wait()
    for line in process.stdout:
        print(line)
    print("\n")

def CompileAll():
    
    files = glob.glob("*.basm")
    for file in files:
        print(f"Compiling {file}...")
        Compile(file)
    
    print("Finished compiling...")

if __name__ == "__main__":

    CompileAll()