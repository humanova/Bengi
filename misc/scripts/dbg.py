# 2019 Emir Erbasan(humanova)
# GPL v2 License, see LICENSE for more details

# debugging script for bengi programs

import sys
import time
import ctypes

class Bdbg:

    def __init__(self, shared_lib_path, bytecode_path, step_interval):
        self.vm = self.load_lib(shared_lib_path)
        self.bytecode_file = ctypes.create_string_buffer(str.encode(bytecode_path)) 
        self.step = 0
        self.tos = -1
        
        self.step_interval = step_interval
    
    def load_lib(self, path):
        try:
            vm = ctypes.CDLL(path)
        except Exception as e:
            print(f"error while loading shared library : {e}")
            exit(1)
        return vm

    def get_vm_status(self):
        stack = []
        regs = []

        stack_ptr = self.vm._get_register_value(3)
        for i in range(stack_ptr + 1):
            stack.append(self.vm._get_stack_element_value(i))

        for i in range(1, 6):
            regs.append(self.vm._get_register_value(i))

        status = {"stack" : stack, 
                  "registers" : regs, 
                  "func_depth": self.vm._get_func_depth()}
             
        return status

    def print_step(self):
        status = self.get_vm_status()
        regs = status['registers']
        stack = status['stack']
        func_depth = status['func_depth']

        print(f"(step {self.step} PC:{regs[4]} funcDepth:{func_depth} : tos = {tos}")
        print(f"[REGS] : AX = {regs[0]} BX = {regs[1]} SP = {regs[2]} BP = {regs[3]} PC = {regs[4]}")
        for i in range(len(stack)):
            print(f"[{i}] : {stack[i]}")

    def run(self):
        if self.vm.load_bytecode(self.bytecode_file):
            self.step = 1
            while self.vm.is_running():
                self.tos = self.vm._run_step()
                self.print_step()
                if not self.step_interval == 0:
                    time.sleep(step_interval)
                self.step += 1                
            return tos
        else:
            print("Error while loading bytecode file...")
            return None

if __name__ == "__main__":
    step_interval = 0
    if len(sys.argv) >= 2 and len(sys.argv) <=3:
        if len(sys.argv) == 3:
            step_interval = float(sys.argv[2])

        bytecode_file = sys.argv[1]
        b = Bdbg('../../lib/libbengi_lib.a', bytecode_file, step_interval)
        tos = b.run()
        print(f"vm result : {tos}")