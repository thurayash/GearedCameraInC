import re
import sys

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def verification(file_name):
    file = open(file_name, "r")
    found = False
    for index,line in enumerate(file):
        if re.search(" $", line):
            print(f"file {file_name} \nline : " + bcolors.WARNING + \
                    "{} => {}".format(index+1, line) + bcolors.ENDC)
            found = True
    return found


if __name__ == "__main__":
    failed = False
    for files in sys.argv[1:]:
        if not verification(files):
            print(bcolors.OKGREEN + "Success no white spaces found in " + \
                    bcolors.ENDC + f"{files}")
        else:
            print(bcolors.FAIL + f"{files} : Go fixe the whitespaces using \
                    the command 'make remove_space' !" + bcolors.ENDC + \
                    "\n===========================")
            failed = True

    if failed:
        sys.exit(127)
    sys.exit(0)
