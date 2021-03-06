# (c) 2019, 2020 Emir Erbasan(humanova)
# GPL v2 License, see LICENSE for more details

# generates C++ header (src/tests/data.h) for tests
# original test data is located at : misc/test_data/

import time
import numpy as np
import glob

def convert_to_array(files):
    data = []
    for file in files:
        f = open(file, "rb")
        fn = file[file.rfind('/')+1:file.rfind('.')]
        data.append({"file" : fn, "bytecode" : np.fromfile(f, dtype=np.uint32)})
    return data

def read_assembly(files):
    data = []
    for file in files:
        f = open(file, "r")
        fn = file[file.rfind('/')+1:file.rfind('.')] + "_asm"
        data.append({"file": fn, "asm" : [("\""+line.replace('\n', '') + "\\n\"") for line in f.readlines()]})
    return data

def generate_cpp_header(cben_data, basm_data):
    header_str = str()
    header_str += f"/*\n * Generated : {time.strftime('%d-%m-%Y %H:%M:%S.00000', time.gmtime())}\n"
    header_str += " * ----------------------------------------------------------\n"
    header_str += " * This file is generated by gen_test_header.py script. Don't edit directly.\n*/\n\n"

    header_str += "#include <vector>\n"
    header_str += "#include <string>\n"
    header_str += "#include <cstdint>\n\n"

    for d in cben_data:
        fn = d['file']
        vector_str = f"std::vector<uint32_t> {fn} = {{{', '.join(hex(inst) for inst in d['bytecode'])}}};"
        header_str += vector_str + "\n"
    header_str += "\n//-----basm strings-----\n\n"

    for d in basm_data:
        fn = d['file']
        asm = '\n\t'.join(d['asm'])
        asm_str = f"std::string {fn} = \n\t{asm};"
        header_str += asm_str + "\n"
    
    header_str += "\n"

    for d in cben_data:
        fn = d['file']
        header_str += f"extern std::vector<uint32_t> {fn};\n"
    
    for d in basm_data:
        fn = d['file']
        header_str += f"extern std::string {fn};\n"

    with open("src/tests/data.h", "w+") as f:
        f.write(header_str)

if __name__ == "__main__":
    directory = "misc/test_data"
    cben_files = glob.glob(directory + "/bytecode/*.cben")
    basm_files = glob.glob(directory + "/basm/*.basm")
    
    cben_data = convert_to_array(cben_files)
    basm_data = read_assembly(basm_files)
    generate_cpp_header(cben_data, basm_data)