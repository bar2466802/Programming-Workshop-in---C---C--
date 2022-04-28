import os
import sys
import subprocess as sp
import time

SCHOOL_SOL = "~labcc2/www/ex5/school_solution/RecommenderSystem"
PRESUBMIT = "~labcc2/www/ex5/presubmit_ex5"
CODE_STYLE = "~labcc2/www/codingStyleCheck"


def compile():
    os.system("g++ -Wall -Wvla -Wextra -Werror -g -std=c++17 RecommenderSystem.h RecommenderSystem.cpp main.cpp -o test")


def run_test():
    print("coding style check:")
    os.system(CODE_STYLE + " RecommenderSystem.h")
    os.system(CODE_STYLE + " RecommenderSystem.cpp")
    print("\n------\ntesting your sol.")
    t = time.time()
    sp.run("test")
    t2 = time.time()
    print("\n------\ntime took for your solution: {}".format(t2 - t))
    print("school sol:")
    UNAMES = ["Miles", "Michaela"]
    MOVIES = ["TopHat", "TropicalMalady"]
    UNAMES_SEEN = ["Reina", "Celine"]
    print("Unable to open file iasvf")
    n = "a523sgf"
    os.system("~labcc2/www/ex5/school_solution/RecommenderSystem ./movies_big.txt ./ranks_big.txt content recommend " + n)
    os.system("~labcc2/www/ex5/school_solution/RecommenderSystem ./movies_big.txt ./ranks_big.txt cf recommend " + n + " 1")
    os.system("~labcc2/www/ex5/school_solution/RecommenderSystem ./movies_big.txt ./ranks_big.txt cf predict TheKid " + n + " 1")
    os.system("~labcc2/www/ex5/school_solution/RecommenderSystem ./movies_big.txt ./ranks_big.txt cf predict " + n + " Reina 1")
    time.sleep(2)
    print("\nlegal vals:")
    for m in UNAMES:
        os.system("~labcc2/www/ex5/school_solution/RecommenderSystem ./movies_big.txt ./ranks_big.txt content recommend " + m)
        for k in range(1,4):
            os.system("~labcc2/www/ex5/school_solution/RecommenderSystem ./movies_big.txt ./ranks_big.txt cf recommend {} {}".format(m, k))
            for mov in MOVIES:
                os.system("~labcc2/www/ex5/school_solution/RecommenderSystem ./movies_big.txt ./ranks_big.txt cf predict {} {} {}".format(mov, m, k))
    time.sleep(10)
    print("\nillegal vals:")
    for m in UNAMES_SEEN:
        os.system("~labcc2/www/ex5/school_solution/RecommenderSystem ./movies_big.txt ./ranks_big.txt content recommend " + m)
        for k in range(1, 2):
            os.system("~labcc2/www/ex5/school_solution/RecommenderSystem ./movies_big.txt ./ranks_big.txt cf recommend {} {}".format(m, k))
            for mov in MOVIES:
                os.system("~labcc2/www/ex5/school_solution/RecommenderSystem ./movies_big.txt ./ranks_big.txt cf predict {} {} {}".format(mov, m, k))

    print("\n------\ncreating tar and running pre-submit:")
    os.system("tar -cvf ex5.tar RecommenderSystem.cpp RecommenderSystem.h")
    os.system(PRESUBMIT + " ex5.tar")


if __name__ == "__main__":
    compile()
    run_test()