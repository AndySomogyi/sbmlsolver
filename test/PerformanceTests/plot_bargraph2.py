import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import pickle
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
    return df

def plot(data):
    print(data)

if __name__ == "__main__":
    data = parse_data()

    # for label, df in data.groupby(by=["RunID", "Model Size", "Model ID"]):
    #     print(label, df)
    df = data.groupby(by=["RunID", "Model Size", "Model ID"]).aggregate([np.mean, np.std])

    # m = data.mean(axis=1)
    # s = data.std(axis=1)
    # df = pd.concat([m, s], axis=1)
    # df.columns = ["mean", "stdev"]
    df.to_csv(r"D:\roadrunner\roadrunner\test\PerformanceTests\simulations\data.csv")
    # df.reset_index(inplace=True)

    print(df)

    # plot(data)








