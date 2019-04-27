from ctypes import CDLL, create_string_buffer
import sys

def LoadDLL():

    Bengi = CDLL('bengi.dll')
    return Bengi

def RunCBEN(path):

    Bengi = LoadDLL()
    p = create_string_buffer(str.encode(path))
    is_ok = Bengi.LoadBinary(p)
    if is_ok:
        ret_val = Bengi.Run()
        del Bengi
        return ret_val
    else:
        print("Error while loading .cben file...")
        del Bengi
        return None

results = {
    1 : 40,
    2 : 60,
    3 : 11,
    4 : 46368,
    5 : 100,
    6 : 10,
}

def RunTests():

    p = 0
    for i in range(len(results)):
        i = i + 1
        res = RunCBEN(f"test{i}.cben")
        if res == results[i]:
            print(f"res[{i}] = {res} [PASS]")
            p += 1
        else:
            print(f"res[{i}] = {res} [FAIL] - (expected : {results[i]})")
    
    return (p, len(results))

if __name__ == "__main__":
    
    if not len(sys.argv) == 0:
        if sys.argv[1] == '-t' or sys.argv[1] == '--test':
            res = RunTests()
            print(f"{res[0]}/{res[1]} tests passed...")
