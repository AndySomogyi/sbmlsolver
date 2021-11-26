import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import pickle
from scipy.stats import mode
import os
from os.path import (
    split, join, dirname, abspath,
    isdir, isabs, isfile
)
import numpy as np
import glob
import random_network_performance_tests as rnpt

RUN_LABELS = ["LLJit_AggressiveOpt", "LLJit_NoneOpt", "MCJit", "rr2.1.3"]


def parse_data():
    df_dct = {}
    for run_label in RUN_LABELS:
        df = rnpt.collectPerformanceData(rnpt.NUM_SIM_STEPS, rnpt.NUM_REPEAT_SIMS, run_label)
        df_dct[run_label] = df
    df = pd.concat(df_dct)
    idxNames = list(df.index.names)
    idxNames[0] = "RunID"
    df.index.names = idxNames
    series: pd.Series = df.stack()
    df = pd.DataFrame(series)
    df.columns = ["SimulationTime"]
    return df

def stats(data: pd.DataFrame):
    """Computes mean and std"""
    df = data.groupby(by=["RunID", "ModelSize", "ModelID"]).aggregate([np.mean, np.std])
    colIdxNames = list(df.columns.names)
    colIdxNames[1] = "Statistic"
    df.columns.names = colIdxNames
    series: pd.Series = df.stack().stack()
    df = pd.DataFrame(series, columns=["values"])
    df = df.reorder_levels(["KineticsType", "Statistic", "RunID","ModelSize", "ModelID"])
    df = df.sort_index()
    return df


def plot(data):
    directory = os.path.join(os.path.dirname(__file__), "simulations")
    directory = os.path.join(directory, "barplots")
    if not os.path.isdir(directory):
        os.makedirs(directory)
    groupby = ["KineticsType", "ModelSize", "ModelID"]
    for (kinetics,  model_size, modelID), df in data.groupby(by=groupby):
        print(df)
        fig = plt.figure()
        sns.barplot(
            data=df.reset_index(), x="RunID", y="SimulationTime", units="SimulationRepeat",
            linewidth=1, edgecolor="black"
        )
        plt.ylabel("SimulationTime (ms)")
        plt.title(f"{kinetics} {model_size} {modelID} (n=50)")
        sns.despine(fig)

        fname = os.path.join(directory, f"{modelID}{model_size}{kinetics}.png")
        plt.savefig(fname, dpi=75, bbox_inches="tight")
    # plt.show()
    # print(data)

if __name__ == "__main__":
    data = parse_data()
    print(data)
    # data = stats(data)

    plot(data)








