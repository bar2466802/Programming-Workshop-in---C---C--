import difflib
import subprocess
import time
import random
import textwrap
import sys
import json

COMPILE = "make compile"
REC = "content recommend"
CF = "cf recommend"
PRED = "cf predict"

USE_VAL = False
F_OUT = "./out"
SF_OUT = "./school_out"

EXE = "./prog"
# SCHOOL = "./school/RecommenderSystem"
SCHOOL = "~labcc2/www/ex5/school_solution/RecommenderSystem"

NA = "NA"
SUCCESS = 0
ITERS = 50

STRICT = True
PRINT = False

BASE_PATH = __file__
ARGS = {"Small": "./data/movies_small.txt ./data/ranks_small.txt", "Medium":
    "./data/movies_features.txt ./data/ranks_matrix.txt",
        "Big": "./data/movies_big.txt ./data/ranks_big.txt"}


def compile_c():
    return subprocess.run(COMPILE, text=True, shell=True).returncode


def cmp_files(cmp_file, out_file):
    with open(cmp_file, 'r') as fCmp:
        with open(out_file, 'r') as fOut:
            diff = difflib.ndiff(fOut.readlines(), fCmp.readlines())

            for i, line in enumerate(diff):
                if line.startswith("- ") or line.startswith(
                        "+ ") or line.startswith("? "):
                    print("\t\tOutput test - FAIL")
                    print(
                        "\t\tDifference found in '" + out_file + "' at line " + str(
                            i))

                    fCmp.seek(0)
                    fOut.seek(0)
                    print("\n\t\t printing files:")
                    wrapper = textwrap.TextWrapper(initial_indent='\t\t\tschool: ', width=80)
                    print(wrapper.fill(fCmp.read()))

                    wrapper = textwrap.TextWrapper(initial_indent='\t\t\tyours: ', width=80)
                    print(wrapper.fill(fOut.read()))

                    return 0

            print("\t\tOutput test - PASS")
            return 1


def load_data(files):
    movies_file, ranks_file = files.split()

    movies = []
    users_ranks = {}
    with open(ranks_file, 'r') as rf:
        movies = rf.readline().split()
        for line in rf:
            values = line.split()
            key = values.pop(0)
            users_ranks[key] = values

    return movies, users_ranks


def test_rec(users_ranks, iterations, movie_data, ranking_data):
    my_time = 0
    school_time = 0
    names = random.choices(list(users_ranks.keys()), k=iterations)
    with open(F_OUT, 'w') as fOut, open(SF_OUT, 'w') as sfOut:
        for name in names:
            cmd = " ".join([EXE, movie_data, ranking_data, REC, name])
            start = time.time()
            ret_code = subprocess.run(cmd, stdout=fOut, stderr=fOut, shell=True).returncode
            if ret_code != 0:
                print(f"** your code crashed** \n"
                      f"command: {cmd}\n "
                      f"return code: {ret_code}\n"
                      f" -- check ./out for more info ---")
                exit()

            my_time += time.time() - start

            cmd = " ".join([SCHOOL, movie_data, ranking_data, REC, name])
            start = time.time()
            s_ret_code = subprocess.run(cmd, stdout=sfOut, stderr=sfOut, shell=True).returncode
            if s_ret_code != 0:
                print(f"** school crashed** \n"
                      f"command: {cmd}\n "
                      f"return code: {s_ret_code}\n"
                      f" -- check ./school_out for more info --")
                exit()
            school_time += time.time() - start

        if cmp_files(SF_OUT, F_OUT) != 1:
            print("\nfailed comparision to school")
            print("input was: ")
            print(names)
            exit()
        return my_time, school_time


def test_pred(movies, users_ranks, iterations, movie_data, ranking_data):
    my_time = 0
    school_time = 0

    inputs = {}
    j = 0
    names = random.choices(list(users_ranks.keys()), k=iterations)
    with open(F_OUT, 'w') as fOut, open(SF_OUT, 'w') as sfOut:
        for name in names:
            i = random.choice(range(len(movies)))
            while users_ranks[name][i] != NA:
                i = random.choice(range(len(movies)))

            count = len(users_ranks[name]) - list(users_ranks[name]).count(NA)
            if STRICT and count == 0:
                continue
            else:
                k = random.choice(range(count))
                k = k + 1 if k == 0 else k
            inputs[j] = (name, movies[i], k)
            j += 1

            cmd = " ".join([EXE, movie_data, ranking_data, PRED, movies[i], name, str(k)])
            if PRINT:
                print(cmd)
            start = time.time()
            ret_code = subprocess.run(cmd, stdout=fOut, stderr=fOut, shell=True).returncode
            if ret_code != 0:
                print(f"** your code crashed** \n"
                      f"command: {cmd}\n "
                      f"return code: {ret_code}\n"
                      f" -- check ./out for more info ---")
                exit()
            my_time += time.time() - start

            cmd = " ".join([SCHOOL, movie_data, ranking_data, PRED, movies[i], name, str(k)])
            start = time.time()
            s_ret_code = subprocess.run(cmd, stdout=sfOut, stderr=sfOut, shell=True).returncode
            if s_ret_code != 0:
                print(f"** school crashed** \n"
                      f"command: {cmd}\n "
                      f"return code: {s_ret_code}\n"
                      f" -- check ./school_out for more info --")
                exit()
            school_time += time.time() - start

        if cmp_files(SF_OUT, F_OUT) != 1:
            print("\nfailed comparision to school")
            print("input was: ")
            print(json.dumps(inputs, indent=4))
            exit()
        return my_time, school_time


def test_cf(users_ranks, iterations, movie_data, ranking_data):
    my_time = 0
    school_time = 0

    inputs = {}
    j = 0
    names = random.choices(list(users_ranks.keys()), k=iterations)
    with open(F_OUT, 'w') as fOut, open(SF_OUT, 'w') as sfOut:
        for name in names:
            count = len(users_ranks[name]) - list(users_ranks[name]).count(NA)
            if STRICT and count == 0:
                continue
            else:
                k = random.choice(range(count))
                k = k+1 if k == 0 else k
            inputs[j] = (name, k)
            j += 1

            cmd = " ".join([EXE, movie_data, ranking_data, CF, name, str(k)])
            start = time.time()
            ret_code = subprocess.run(cmd, stdout=fOut, stderr=fOut, shell=True).returncode
            if ret_code != 0:
                print(f"** your code crashed** \n"
                      f"command: {cmd}\n "
                      f"return code: {ret_code}\n"
                      f" -- check ./out for more info ---")
                exit()
            my_time += time.time() - start

            cmd = " ".join([SCHOOL, movie_data, ranking_data, CF, name, str(k)])
            start = time.time()
            s_ret_code = subprocess.run(cmd, stdout=sfOut, stderr=sfOut, shell=True).returncode
            if s_ret_code != 0:
                print(f"** school crashed** \n"
                      f"command: {cmd}\n "
                      f"return code: {s_ret_code}\n"
                      f" -- check ./school_out for more info --")
                exit()
            school_time += time.time() - start

        if cmp_files(SF_OUT, F_OUT) != 1:
            print("\nfailed comparision to school")
            print("input was: ")
            print(json.dumps(inputs, indent=4))
            exit()
        return my_time, school_time


if __name__ == '__main__':
    print("**compiling**")
    assert compile_c() == SUCCESS, "Failed compiling"

    if len(sys.argv) > 1:
        ITERS = int(sys.argv[1])

    if "-strict" in sys.argv:
        STRICT = False

    if "-print" in sys.argv:
        PRINT = True


    print(" --- SMALL --- ")
    args = ARGS["Small"]
    movies, users_ranks = load_data(args)
    print(f"\t**{REC}:")
    t, st = test_rec(users_ranks, ITERS, *args.split())
    print(f"\t\t your time: {t}")
    print(f"\t\t school time: {st}")

    print(f"\t**{PRED}:")
    t, st = test_pred(movies, users_ranks, ITERS, *args.split())
    print(f"\t\t your time: {t}")
    print(f"\t\t school time: {st}")

    print(f"\t**{CF}:")
    t, st = test_cf(users_ranks, ITERS, *args.split())
    print(f"\t\t your time: {t}")
    print(f"\t\t school time: {st}")

    print()
    print(" --- MEDIUM --- ")
    args = ARGS["Medium"]
    movies, users_ranks = load_data(args)
    print(f"\t**{REC}:")
    t, st = test_rec(users_ranks, ITERS, *args.split())
    print(f"\t\t your time: {t}")
    print(f"\t\t school time: {st}")

    print(f"\t**{PRED}:")
    t, st = test_pred(movies, users_ranks, ITERS, *args.split())
    print(f"\t\t your time: {t}")
    print(f"\t\t school time: {st}")

    print(f"\t**{CF}:")
    t, st = test_cf(users_ranks, ITERS, *args.split())
    print(f"\t\t your time: {t}")
    print(f"\t\t school time: {st}")

    print()
    print(" --- BIG --- ")
    args = ARGS["Big"]
    movies, users_ranks = load_data(args)
    print(f"\t**{REC}:")
    t, st = test_rec(users_ranks, ITERS, *args.split())
    print(f"\t\t your time: {t}")
    print(f"\t\t school time: {st}")

    print(f"\t**{PRED}:")
    t, st = test_pred(movies, users_ranks, ITERS, *args.split())
    print(f"\t\t your time: {t}")
    print(f"\t\t school time: {st}")

    print(f"\t**{CF}:")
    t, st = test_cf(users_ranks, ITERS, *args.split())
    print(f"\t\t your time: {t}")
    print(f"\t\t school time: {st}")
