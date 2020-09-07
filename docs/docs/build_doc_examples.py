"""
This script will run all of the compiled examples it can find
in the build bin directory as well as the Python files in docs/source/*/*.py. The output
is saved to a folder ExampleOutputFiles under the docs directory where sphinx can find them.
"""

import os, sys, glob
import subprocess
import site
import argparse
import re
import sys

# setup command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("--build-dir", type=str, help="root of the build tree")
parser.add_argument("--python-interpretor", type=str,
                    default=sys.executable,
                    help="Full path to the Python interpretor you want to use to run the Python examples")
parser.add_argument("--pyomexmeta-source-dir", type=str,
                    default=sys.executable,
                    help="Full path to the Python interpretor you want to use to run the Python examples")
args = parser.parse_args()

print("Python interpretor being used: {}".format(sys.executable))
print("pyomexmeta source dir: {}".format(args.pyomexmeta_source_dir))

# add the path to pyomexmeta package to python path for duration of script
sys.path.append(os.path.dirname(args.pyomexmeta_source_dir))


DOCS_DIR = os.path.dirname(__file__)
DOCS_EXAMPLE_OUTPUT_DIR = os.path.join(DOCS_DIR, "ExampleOutputFiles")

if not os.path.isdir(DOCS_EXAMPLE_OUTPUT_DIR):
    os.makedirs(DOCS_EXAMPLE_OUTPUT_DIR)

PROJECT_DIR = os.path.dirname(DOCS_DIR)
DOCS_SOURCE_DIR = os.path.join(DOCS_DIR, "source")
PROJECT_SOURCE_DIR = os.path.join(PROJECT_DIR, "src")
DOCS_PYTHON_EXAMPLES = glob.glob(
    os.path.join(DOCS_SOURCE_DIR, "*/*.py")
)

# set a default build dir
BUILD_DIR = os.path.join(PROJECT_DIR, args.build_dir)
print("BUILD_DIR", BUILD_DIR)
# and check if it exists
if not os.path.isdir(BUILD_DIR):
    raise NotADirectoryError(BUILD_DIR)

BIN_DIR = os.path.join(BUILD_DIR, "bin")

# collect a full list of files in bin dir
exe = glob.glob(os.path.join(BIN_DIR, "*"))

# exclude non executables
exe = [i for i in exe if os.access(i, os.X_OK)]

# exclude some more binaries. These are not doc examples.
exclude_list = ["OmexMetaTests", "redland-wrapper-tests"]
exe = [i for i in exe if os.path.splitext(os.path.split(i)[1])[0] not in exclude_list]


def camel_to_snake(name):
    name = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', name).lower()


def execute_binary_and_capture_output(binary_file):
    """
    Execute binary binary_file and collect its output
    :param binary_file:
    :return:
    """
    if not os.path.isfile(binary_file):
        raise FileNotFoundError(binary_file)
    print(f"running \"{binary_file}\"")
    output = subprocess.Popen([binary_file], stdout=subprocess.PIPE).communicate()[0]
    string = output.decode()

    binary_name = os.path.splitext(os.path.split(binary_file)[1])[0]

    # ensure camel case not used
    binary_name = camel_to_snake(binary_name)

    if not os.path.isdir(DOCS_EXAMPLE_OUTPUT_DIR):
        os.makedirs(DOCS_EXAMPLE_OUTPUT_DIR)
    output_fname = os.path.join(DOCS_EXAMPLE_OUTPUT_DIR, "{}.txt".format(binary_name))
    with open(output_fname, "w") as f:
        f.write(string)
    print("output saved to: {}".format(output_fname))


def execute_python_and_capture_output(python_file):
    """
    Run a python file and collect its output
    :param python_file:
    :return:
    """
    if not os.path.isfile(python_file):
        raise FileNotFoundError(python_file)
    print(f"running \"{python_file}\"")

    e = """import site
site.addsitedir(\"{}\")
""".format(PROJECT_SOURCE_DIR)

    with open(python_file) as f:
        python_code = f.read()

    python_code = e + python_code

    tmp_file = os.path.join(os.getcwd(), "tmp.py")
    with open(tmp_file, "w") as f:
        f.write(python_code)

    # collect the name of the python file
    name = os.path.splitext(os.path.split(python_file)[1])[0]

    # camel case to underscore
    name = camel_to_snake(name)

    # create a filename for storing python output
    fname = os.path.join(DOCS_EXAMPLE_OUTPUT_DIR, "{}.txt".format(name))
    output = subprocess.Popen([args.python_interpretor, tmp_file], stdout=subprocess.PIPE).communicate()[0].decode()

    # now write to file
    with open(fname, "w") as f:
        f.write(output)

    print("Output saved to: {}".format(fname))

    # remove the temp
    if os.path.isfile(tmp_file):
        os.remove(tmp_file)


# run the binary examples
print(exe)
for binary_file in exe:
    print("binary file: {}".format(binary_file))
    execute_binary_and_capture_output(binary_file)

for python_file in DOCS_PYTHON_EXAMPLES:
    execute_python_and_capture_output(python_file)
