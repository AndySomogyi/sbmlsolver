import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np
import os

sns.set_context("talk")

simple_flux_parallel = pd.DataFrame({
    1: 2.5627729892730713,
    10: 2.4990291595458984,
    100: 2.5727479457855225,
    1000: 3.2082808017730713,
    10000: 8.656226396560669,
    100000: 65.82059264183044,
}, index=['parallel'])

simple_flux_serial = pd.DataFrame({
    1: 0.015676021575927734,
    10: 0.031247854232788086,
    100: 0.2812502384185791,
    1000: 2.8941521644592285,
    10000: 31.070906162261963,
    100000: 301.27891850471497,
}, index=['serial'])

brown2004_serial = pd.DataFrame({
    1: 0.06298232078552246,
    10: 0.0625145435333252,
    100: 0.5781285762786865,
    1000: 6.202374696731567,
    10000: 63.128479957580566,
    100000: 647.5555186271667,
}, index=['serial'])


brown2004_parallel = pd.DataFrame({
    1: 2.6799676418304443,
    10: 2.843959093093872,
    100: 3.014779806137085,
    1000: 4.199995040893555,
    10000: 18.352259635925293,
    100000: 160.3436517715454,
}, index=['parallel'])

def concat_data(data):
    df = pd.concat(data)
    df = df.transpose()
    df = pd.DataFrame(df.stack()).reset_index()
    # df.reset_index(inplace=True)
    df.columns = ['Num Simulations', 'Simulation Mode', 'Time (s)']
    df['Num Simulations'] = np.log10(df['Num Simulations'])
    plt.xlabel('Num Simulations (log10)')
    return df

def plot_simple_flux_all_data():
    df = concat_data([simple_flux_serial, simple_flux_parallel])
    par = df[df['Simulation Mode'] == 'parallel']['Time (s)'].to_numpy()
    ser = df[df['Simulation Mode'] == 'serial']['Time (s)'].to_numpy()
    print(par)
    print(ser)
    print(ser / par)
    fig = plt.figure()
    sns.barplot(x='Num Simulations', y='Time (s)', data=df, hue='Simulation Mode',
                palette='bright', edgecolor='black',linewidth=1)
    sns.despine(fig)
    plt.xlabel('Num Simulations (log10)')

    fname = os.path.join(os.path.dirname(__file__), 'SimpleFluxTo1e5.png')
    plt.savefig(fname, dpi=150, bbox_inches='tight')

def plot_simple_flux_to1e3():
    df = concat_data([simple_flux_serial, simple_flux_parallel])
    df = df[df['Num Simulations'] < 4.0]
    print(df)
    fig = plt.figure()
    bp = sns.barplot(x='Num Simulations', y='Time (s)', data=df, hue='Simulation Mode',
                     palette='bright', edgecolor='black',linewidth=1)
    plt.legend([], [], frameon=False)
    plt.xlabel('Num Simulations (log10)')
    sns.despine(fig)
    plt.xlabel('')
    plt.ylabel('')

    fname = os.path.join(os.path.dirname(__file__), 'SimpleFluxTo1e3.png')
    plt.savefig(fname, dpi=150, bbox_inches='tight')

def plot_brown2004_all_data():
    df = concat_data([brown2004_serial, brown2004_parallel])
    par = df[df['Simulation Mode'] == 'parallel']['Time (s)'].to_numpy()
    ser = df[df['Simulation Mode'] == 'serial']['Time (s)'].to_numpy()
    print(par)
    print(ser)
    print(ser / par)
    # print(ser)
    fig = plt.figure()
    sns.barplot(x='Num Simulations', y='Time (s)', data=df, hue='Simulation Mode',
                palette='bright', edgecolor='black',linewidth=1
                )
    plt.xlabel('Num Simulations (log10)')
    sns.despine(fig)

    fname = os.path.join(os.path.dirname(__file__), 'Brown2004To1e5.png')
    plt.savefig(fname, dpi=150, bbox_inches='tight')

def plot_brown2004_to1e3():
    df = concat_data([brown2004_serial, brown2004_parallel])
    df = df[df['Num Simulations'] < 4.0]

    fig = plt.figure()
    bp = sns.barplot(x='Num Simulations', y='Time (s)', data=df, hue='Simulation Mode',
                     palette='bright', edgecolor='black',linewidth=1)
    sns.despine(fig)
    plt.legend([], [], frameon=False)
    plt.xlabel('')
    plt.ylabel('')

    fname = os.path.join(os.path.dirname(__file__), 'Brown2004To1e3.png')
    plt.savefig(fname, dpi=150, bbox_inches='tight')



if __name__ == '__main__':
    plot_simple_flux_all_data()
    # plot_simple_flux_to1e3()
    # plot_brown2004_all_data()
    # plot_brown2004_to1e3()








