# 2019 Emir Erbasan(humanova)
# GPL v2 License, see LICENSE for more details

# debugging script for bengi programs

from colorama import init, Fore, Back, Style 
from ctypes import CDLL, create_string_buffer, RTLD_GLOBAL
import sys
import time

##init colorama
init()

def LoadDLL():
    if sys.platform == "win32":
        Bengi = CDLL('./bengi.dll')
    else:
        Bengi = CDLL('./bengi.so')
    return Bengi

def GetStack(vm, stack_p):
    stack = []
    for i in range(stack_p + 1):
        stack.append(vm.GetStackElement(i))
    return stack

def GetRegisters(vm):
    regs = []
    for i in range(1, 6):
        regs.append(vm.GetRegister(i))

    return regs

def PrintStack(stack):
    for i in range(len(stack)):
        print(Fore.CYAN + f"[{i}] : {stack[i]}")
    print(Style.RESET_ALL)

def PrintRegs(regs):
    print(Fore.LIGHTYELLOW_EX + f"[REGS] : AX = {regs[0]} BX = {regs[1]} SP = {regs[2]} BP = {regs[3]} PC = {regs[4]}")

def PrintStep(step, tos, stack, regs, funcDepth, currFuncSymbol):
    print(Fore.LIGHTGREEN_EX + f"(step {step} PC:{regs[4]} funcDepth:{funcDepth} currFunc:{currFuncSymbol}) : tos = {tos}")
    PrintRegs(regs)
    PrintStack(stack)

def RunCBEN(path):
    global step_interval
    Bengi = LoadDLL()
    p = create_string_buffer(str.encode(path))
    is_ok = Bengi.LoadBinary(p)
    tos = -1
    if is_ok:
        stepCounter = 1
        while Bengi.isRunning():
           
            tos = Bengi.RunStep()
            stackPtr = Bengi.GetRegister(3)
            stack = GetStack(Bengi, stackPtr)
            regs = GetRegisters(Bengi)
            funcDepth = Bengi.GetFuncDepth()
            currFuncSymbol = Bengi.GetCurrFuncSymbol()

            PrintStep(stepCounter, tos, stack, regs, funcDepth, currFuncSymbol)

            stepCounter += 1

            if not step_interval == 0:
                time.sleep(step_interval)
        return tos
    else:
        print("Error while loading .cben file...")
        del Bengi
        return None

if __name__ == "__main__":
    step_interval = 0
    
    if len(sys.argv) == 2:
        tos = RunCBEN(sys.argv[1])
        print(f"bengi {sys.argv[1]} result : {tos}")
    
    elif len(sys.argv) == 3:
        step_interval = float(sys.argv[2])
        tos = RunCBEN(sys.argv[1])
        print(f"bengi {sys.argv[1]} result : {tos}")
         
    else:
        exit()
