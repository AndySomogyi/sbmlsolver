"""Run VTune profiler systematically using command line

Make sure to use the debug build.

Example
--------
python .\RunRoadRunnerMapPerformanceTestsWithVTune.py --VTune "C:\Program Files (x86)\Intel\oneAPI\vtune\latest\bin64\vtune.exe" --RoadRunnerMapPerformanceTestsExecutable D:\roadrunner\roadrunner\cmake-build-debug\bin\RoadRunnerMapPerformanceTests.exe --OutputDirectory D:\roadrunner\Profiler\RoadRunnerProfiling
"""

import glob, os, argparse
import sys
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument("--VTune", type=str, help="Absolute path to the VTune executable", nargs="+")
parser.add_argument("--RoadRunnerMapPerformanceTestsExecutable", type=str, nargs="+",
                    help="Absolute path to location of the RoadRunnerMapPerformanceTestsExecutable")
parser.add_argument("--OutputDirectory", type=str, help="Absolute path to the directory for storing data", nargs="+")
args = parser.parse_args()

vtuneExe = args.VTune if isinstance(args.VTune, str) else "".join(args.VTune)
perfExe = args.VTune if isinstance(args.RoadRunnerMapPerformanceTestsExecutable, str) else "".join(
    args.RoadRunnerMapPerformanceTestsExecutable)
outDir = args.VTune if isinstance(args.OutputDirectory, str) else "".join(args.OutputDirectory)


def build_results_dirname(n_models:int, jit:str, n_threads:int, test_name:str):
    return os.path.join(outDir, f"{jit}_{n_models}_{n_threads}_{test_name}")

def buildThreadingAnalysisCommand(n_models: int, jit: str, n_threads: int, test_name: str):
    dirname = build_results_dirname(n_models, jit, n_threads, test_name)
    command = f"\"{vtuneExe}\" -collect threading -knob sampling-and-waits=hw -knob sampling-interval=10 "
    command += f"-app-working-dir \"{outDir}\" --app-working-dir=\"{outDir}\" -no-allow-multiple-runs -result-dir=\"{dirname}\" --data-limit=6000 "
    command += f" -- {perfExe} "
    command += f"--NModels={n_models} --jit={jit} --nThreads={n_threads} --testName={test_name}"
    return command

def buildHotSpotsAnalysisCommand(n_models: int, jit: str, n_threads: int, test_name: str):
    dirname = build_results_dirname(n_models, jit, n_threads, test_name)
    command = f"\"{vtuneExe}\" --collect hotspots "
    command += f"-app-working-dir \"{outDir}\" --app-working-dir=\"{outDir}\" -no-allow-multiple-runs -result-dir=\"{dirname}\" --data-limit=6000 "
    command += f" -- {perfExe} "
    command += f"--NModels={n_models} --jit={jit} --nThreads={n_threads} --testName={test_name}"
    return command


if __name__ == "__main__":
    n_models = [100]
    jit_engines = ["MCJit", "LLJit"]
    # testNames = ["BuildParallel", "BuildSerial"]
    threads = [i for i in range(1, 16)]

    # first run the serial tests, since we do not run these with different number of threads
    for n in n_models:
        for j in jit_engines:
            print("Running BuildSerial ", "n models: ", n, " jit type: ", j)
            dire = build_results_dirname(n, j, 1, "BuildSerial")
            if not os.path.isdir(dire):
                cmd = buildHotSpotsAnalysisCommand(n, j, 1, "BuildSerial")
                print(cmd)
                subprocess.check_output(cmd, shell=True)

    # and now the parallel tests
    for n in n_models:
        for j in jit_engines:
            for t in threads:
                print("Running BuildParallel", "n models: ", n, " jit type: ", j, "num threads: " , t)
                dire = build_results_dirname(n, j, t, "BuildParallel")
                if not os.path.isdir(dire):
                    print("Running BuildParallel ", n, j, t)
                    cmd = buildThreadingAnalysisCommand(n, j, t, "BuildParallel")
                    subprocess.check_call(cmd, shell=True)
