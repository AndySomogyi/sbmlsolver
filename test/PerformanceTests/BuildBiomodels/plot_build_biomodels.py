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

    df_list = list()
    for runID, dataset in data.items():
        for suiteID, arr in dataset.items():
            subdf = pd.DataFrame(arr, columns=[runID])
            df_list.append(subdf)

    df = pd.concat(df_list, axis=1)
    df = df[sorted(df.columns)]
    return df

def plot_bargraph(data: pd.DataFrame):
    # runID = which jit and which conditions
    # suiteID = which model series for measuring performance
    df = pd.DataFrame(data.sum())
    df.index.names = ['runID']
    df.columns = ['sum(time)']
    # df = df.reset_index()
    # for suiteID, df2 in df.groupby(by='suiteID'):
    fig = plt.figure()
    sns.barplot(x='runID', y='sum(time)', data=df.reset_index(),
                edgecolor='black', linewidth=1.0)
    sns.despine(fig)
    plt.xticks(rotation=90)
    plt.title("Building Biomodels")
    fname = os.path.join(dirname(__file__), f'BuildingBiomodels.png')
    plt.savefig(fname, bbox_inches='tight', dpi=150)

        # plt.show()



if __name__ == "__main__":
    pickle_files = get_pickle_files()
    df = read_pickle_into_df(pickle_files)
    plot_bargraph(df)
    print(df)
