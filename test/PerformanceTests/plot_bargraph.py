import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import pickle
import os
from os.path import (
    split, join, dirname, abspath,
    isdir, isabs, isfile
)
import glob


def get_pickle_files(root=dirname(__file__)):
    pickle_files = glob.glob(join(root, "*/*.pickle"))
    pickle_files = {split(split(os.path.splitext(k)[0])[0])[1]: abspath(k) for k in pickle_files}
    return pickle_files

def read_pickle_into_df(pickle_files: dict):
    data = dict()
    for k, pic in pickle_files.items():
        with open(pic, 'rb') as f:
            data[k] = pickle.load(f)

    # sometimes entries in data are not all the same length
    # discard extras so all datasets have equal number of entries
    minArrSize = 1e6
    for runID, dataset in data.items():
        for suiteID, arr in dataset.items():
            if len(arr) < minArrSize:
                minArrSize = len(arr)

    df_list = list()
    for runID, dataset in data.items():
        for suiteID, arr in dataset.items():
            subdf = pd.DataFrame(arr[:minArrSize], columns=[suiteID])
            subdf.columns = pd.MultiIndex.from_product([subdf.columns, [runID]])
            df_list.append(subdf)

    df = pd.concat(df_list, axis=1)
    df = df[sorted(df.columns)]
    return df

def plot_bargraph(data: pd.DataFrame):
    # runID = which jit and which conditions
    # suiteID = which model series for measuring performance
    df = pd.DataFrame(data.sum())
    df.index.names = ['suiteID', 'runID']
    df.columns = ['sum(time)']
    # df = df.reset_index()
    for suiteID, df2 in df.groupby(by='suiteID'):
        fig = plt.figure()
        sns.barplot(x='runID', y='sum(time)', data=df2.reset_index(),
                    edgecolor='black', linewidth=1.0)
        sns.despine(fig)
        plt.xticks(rotation=90)
        plt.title(suiteID)
        fname = os.path.join(dirname(__file__), f'{suiteID}.png')
        plt.savefig(fname, bbox_inches='tight', dpi=150)

        # plt.show()


if __name__ == "__main__":
    pickle_files = get_pickle_files()
    df = read_pickle_into_df(pickle_files)
    plot_bargraph(df)
    # print(df)
    #
    # print(df.sum(axis=0))







