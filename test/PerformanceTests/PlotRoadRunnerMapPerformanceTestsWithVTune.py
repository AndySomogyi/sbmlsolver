"""Plot results from VTune performance tests

The results need to be generated before they can be plotted. For this,
use the script RunRoadRunnerMapPerformanceTestsWithVTune.py.
"""
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np
import re
import os, glob

sns.set_context("paper", font_scale=1.5)


def extract_times(from_directory: str):
    if not os.path.isdir(from_directory):
        raise NotADirectoryError(from_directory)
    csv_files = glob.glob(os.path.join(from_directory, "*/*.csv"))
    # csv files are badly formatted, so we use regex to extract what we need
    times = {}
    for csv_file in csv_files:
        with open(csv_file, 'r') as f:
            csv_text = f.read()
        elapsed_time = re.findall("Elapsed Time,(.*)", csv_text)
        if not len(elapsed_time) == 1:
            raise ValueError(f"csv_file {csv_file} does not contain Elapsed Time")
        times[csv_file] = float(elapsed_time[0])
    return times


def to_df(times: dict):
    results_list = []
    for k, v in times.items():
        dire, fname = os.path.split(k)
        fname, ext = os.path.splitext(fname)
        jit_engine, n_models, n_threads, label, report_type = fname.split("_")
        results_list.append([jit_engine, int(n_models), int(n_threads), label, report_type, float(v)])

    df = pd.DataFrame(results_list)
    df.columns = ["Jit Engine", "Num Models", "Num Threads", "Run Label", "Report Type", "Time (s)"]
    df = df.sort_values(by=["Jit Engine", "Num Threads"])
    df = df.set_index(list(df.columns[:-1]))
    return df


def plot_bar(data: pd.DataFrame, title: str, fname=None):
    fig = plt.figure()
    sns.barplot(
        data=data.reset_index(), y="Time (s)", x="Num Threads", hue="Jit Engine",
        linewidth=1.0, edgecolor="black"
    )
    sns.despine(fig=fig)
    plt.title(title)

    if not fname:
        plt.show()
    else:
        plt.savefig(fname, dpi=250, bbox_inches="tight")


if __name__ == "__main__":
    # threading analysis
    times = extract_times(r"D:\roadrunner\Profiler\RoadRunnerMapProfiling")
    times = to_df(times)
    title = "Time taken for RoadRunnerMap to load first 100 models\n from the sbml test suite (debug build)."
    fname = os.path.join(os.path.dirname(__file__), "RoadRunnerMapLoad100ModelsDbgMode.png")
    plot_bar(times, title=title, fname=fname)
    print(times)

    # hotspot analysis dbg
    times = extract_times(r"D:\roadrunner\Profiler\RoadRunnerMapProfilingHotspots")
    times = to_df(times)
    title = "Time taken for RoadRunnerMap to load first 100 models\n from the sbml test suite (debug build)."
    fname = os.path.join(os.path.dirname(__file__), "RoadRunnerMapHotspotsLoad100ModelsDbgMode.png")
    plot_bar(times, title=title, fname=fname)
    print(times)

    # hotspot analysis rel
    times = extract_times(r"D:\roadrunner\Profiler\RoadRunnerMapProfilingRelHotspots")
    times = to_df(times)
    title = "Time taken for RoadRunnerMap to load first 100 models\n from the sbml test suite (release build)."
    fname = os.path.join(os.path.dirname(__file__), "RoadRunnerMapHotspotsLoad100ModelsRelMode.png")
    plot_bar(times, title=title, fname=fname)
    print(times)
