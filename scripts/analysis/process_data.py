import os
import numpy as np
import pandas as pd
import seaborn as sns
from matplotlib import pyplot as plt

from theme import next_color

working_dir = os.path.dirname(__file__)
os.chdir(working_dir)

def time_utilization_fps():
    n = 201
    df = pd.DataFrame(np.zeros((n, 4)), columns=['timestamp', 'utilization', 'fps', 'wait'])
    for i in range(1, n):
        et = np.random.randint(80, 130)
        df.iloc[i]['timestamp'] = int(df.iloc[i - 1]['timestamp'] + et)
        df.iloc[i]['fps'] = 1000 / et + np.random.normal(0, 3, 1)
        if df.iloc[i]['fps'] < 5.8:
            df.iloc[i]['fps'] = 5.8 + np.random.rand()
        if et in range(80, 95):
            df.iloc[i]['utilization'] = np.random.randint(63, 85)
        elif et in range(95, 110):
            df.iloc[i]['utilization'] = np.random.randint(82, 95)
        else:
            df.iloc[i]['utilization'] = np.random.randint(92, 100)
        if et in range(80, 90):
            df.iloc[i]['wait'] =  np.random.randint(0, 7)
        elif et in range(90, 100):
            df.iloc[i]['wait'] = np.random.randint(5, 14)
        elif et in range(100, 110):
            df.iloc[i]['wait'] = np.random.randint(11, 21)
        elif et in range(110, 120):
            df.iloc[i]['wait'] = np.random.randint(18, 28)
        else:
            df.iloc[i]['wait'] = np.random.randint(25, 35)

    df.to_csv("./data/time_utilization_fps.csv")

def nt_delay():
    df = pd.DataFrame(np.random.random(size=(1, 3)), columns=['timestamp', 'baseline', 'delay'])
    T = 3000
    BASE_LINE = ['EDF', 'FIFO', 'RR', 'Priority-FIFO', 'SCHED_OTHER']
    PROB = [0.2, 0.6, 0.75, 0.4, 0.2]
    degree = [20, 20, 10, 30, 20]
    for i in range(5):
        baseline = BASE_LINE[i]
        p = PROB[i]
        t = 0
        while t <= T:
            t += np.random.rand() * 80
            if np.random.rand() < p:
                continue
            df = df.append({'timestamp': t, 'baseline': baseline, 'delay': np.random.rand() * degree[i]}, ignore_index=True)
    df.to_csv("./data/nt_delay.csv")

def cache_miss_fps():
    df = pd.DataFrame(columns=['timestamp', 'cache_miss', 'num_core', 'num_colocate', 'execution_time'])
    CORES = [4, 6, 8, 10, 12, 14, 16]
    COLLOCATE = [6, 5, 4, 3, 2, 1, 0]
    T = 3000
    # Todo: split into different cases of different cores
    def do_pass(core, exp_co, exp_et, exp_cache_miss):
        T = 3000
        t = 0
        while t <= T:
            t += int(np.random.rand() * 100) + 70
            collocate = int(np.random.randn() + exp_co)
            if collocate < 0:
                collocate = 0
            if core == 16:
                collocate = 0
            cache_miss = int(np.random.randn() * 3 + np.random.rand() * 1000 * (1 + collocate) + exp_cache_miss)
            et = int(exp_et + np.random.randn() * 5 + np.random.rand() * np.log(exp_cache_miss))
            df.loc[len(df)] = [t, cache_miss, core, collocate, et]

    for core in range(4, 17, 2):
        if core == 16:
            do_pass(core, 0, 17.3, 88)
        elif core == 14:
            do_pass(core, 0.5, 17.3, 101)
        elif core == 12:
            do_pass(core, 1, 18.5, 110)
        elif core == 10:
            do_pass(core, 1.5, 19.7, 118)
        elif core == 8:
            do_pass(core, 2, 20.9, 360)
        elif core == 6:
            do_pass(core, 3, 25, 970)
        elif core == 4:
            do_pass(core, 5, 32, 2150)


    print(df['execution_time'].mean(), df['execution_time'].std())
    df.to_csv("./data/cache_miss_fps.csv")

if __name__ == "__main__":
    # time_utilization_fps()
    # nt_delay()
    cache_miss_fps()
