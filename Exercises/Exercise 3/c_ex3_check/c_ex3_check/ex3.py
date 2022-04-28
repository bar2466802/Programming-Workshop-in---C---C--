#########################################
############## TEST - EX3 ###############
#########################################
# If you found a bug in the test you can always send an email to me:
# amit.david@mail.huji.ac.il
#
# HOW TO RUN
# 0) Make sure you has gcc and python 3.7 installed
# 1) Place TreeAnalyzer.c in the same folder as ex3.py
# 2) Run in terminal using 'python3 ex.3'
# 3) Results will be shown in terminal
#
# All .cmp file generated from school solution
#########################################
import difflib
import subprocess

import sys

PYTHON_VER = f"If you see this error message, you are using python 2.x instead of 3.7"

PROGRAM_NAME_1 = "RBTree"
PROGRAM_NAME_2 = "Structs"

# If you want to force using valgrind change this to variable to True
USE_VALGRIND = True  # sys.platform.startswith("linux")

USE_SCHOOL_SOLUTION = False

# File types
IN = "in"
OUT = "out"
CMP = "cmp"
VAL = "val"

#########################################
# In file names
IN_FILES = [
    ("Test products as data", "ProductExample"),
    ("Test ints as data", "intsTreeCheck"),
    ("Test strings as data", "stringsTreeCheck"),
    ("Test vectors as data", "vectorsTreeCheck")
]


#########################################
def convert_file_name(name, loc):
    return f"{loc}/{name}.{loc}"


def cmp_files(cmp_file, out_file):
    with open(cmp_file, 'r') as fCmp:
        with open(out_file, 'r') as fOut:
            diff = difflib.ndiff(fOut.readlines(), fCmp.readlines())

            for i, line in enumerate(diff):
                if line.startswith("- ") or line.startswith("+ ") or line.startswith("? "):
                    print("\tOutput test - FAIL")
                    print("\tDifference found in '" + out_file + "' at line " + str(i))
                    return

            print("\tOutput test - PASS")


def check_valgrind_file(val_file):
    check_lines = -1

    with open(val_file, 'r') as fVal:
        for line in fVal.readlines():
            if "LEAK SUMMARY:" in line:
                check_lines = 0

            if 0 <= check_lines < 5:
                check_lines += 1
                if ": 0 bytes in 0 blocks" not in line:
                    print("\tValgrind test - FAIL")
                    print("\tMemory leak found, more info in '" + val_file + "'\n")
                    return

            if "ERROR SUMMARY:" in line and "ERROR SUMMARY: 0 errors from" not in line:
                print("\tValgrind test - FAIL")
                print("\tError found, more info in '" + val_file + "'")
                return

    print("\tValgrind test - PASS")


def t_file(test_name, file_name):
    print(test_name)
    in_file = f"in/{file_name}"
    out_file = convert_file_name(file_name, OUT)
    cmp_file = convert_file_name(file_name, CMP)
    val_file = convert_file_name(file_name, VAL)

    with open(out_file, 'w') as fOut:
        try:
            if USE_SCHOOL_SOLUTION:
                program = "~labcc/www/c_ex2/TreeAnalyzer '" + in_file + "'"

            elif USE_VALGRIND:
                program = "valgrind --leak-check=full --log-file=" + val_file + " " + in_file

            else:
                program = in_file

            subprocess.run(program, stdout=fOut, stderr=subprocess.STDOUT, shell=True, timeout=10)

        except subprocess.TimeoutExpired:
            print(test_name + "\tTest - FAIL")
            print("\tTIMEOUT Reached")
            return

    cmp_files(cmp_file, out_file)
    if not USE_SCHOOL_SOLUTION and USE_VALGRIND:
        check_valgrind_file(val_file)


#########################################
if __name__ == "__main__":
    # if "--no-valgrind" in sys.argv:
    #    USE_VALGRIND = False

    print("\n------- COMPILATION START -------")

    print("Compile your files and place binaries at './in' folder...")

    if subprocess.run("(cd in; make all)", text=True, shell=True).returncode != 0:
        print("\nProgram failed compiling")
        exit(1)

    print("------- COMPILATION END - The section above should have no warnings nor errors -------")
    print("\nProgram compiled successfully")

    print("\n------- TEST START -------")
    # Invalid files
    for file in IN_FILES:
        t_file(file[0], file[1])
        print()
    print()

    print("------- TEST END -------")
