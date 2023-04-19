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
        et = np.random.randint(100, 200)
        df.iloc[i]['timestamp'] = int(df.iloc[i - 1]['timestamp'] + et)
        df.iloc[i]['fps'] = 1000 / et
        if et in range(100, 130):
            df.iloc[i]['utilization'] = np.random.randint(63, 85) + np.random.randn() * 2.5
        elif et in range(130, 160):
            df.iloc[i]['utilization'] = np.random.randint(82, 95) + np.random.randn() * 3
        else:
            df.iloc[i]['utilization'] = np.random.randint(92, 100) + np.random.randn() * 3
        if df.iloc[i]['utilization'] > 100:
            df.iloc[i]['utilization'] = 100
        if et in range(100, 118):
            df.iloc[i]['wait'] =  np.random.randint(0, 7) + np.random.randn() * 2
        elif et in range(118, 145):
            df.iloc[i]['wait'] = np.random.randint(5, 14) + np.random.randn() * 2
        elif et in range(145, 161):
            df.iloc[i]['wait'] = np.random.randint(11, 21) + np.random.randn() * 2
        elif et in range(161, 185):
            df.iloc[i]['wait'] = np.random.randint(18, 28) + np.random.randn() * 2
        else:
            df.iloc[i]['wait'] = np.random.randint(25, 35) + np.random.randn() * 2

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

def method_comparison():
    df = pd.DataFrame(columns=['method', 'scheduling', 'normal', 'active', 'idle', 'fps', 'rate'])

    # EDF scheduling
    s, n, a = 0.025, 0.69, 0.13
    df.loc[len(df)] = ['EDF', s, n, a, 1 - (s + n + a)]

    # FIFO scheduling
    s, n, a = 0.012, 0.61, 0.23
    df.loc[len(df)] = ['FIFO', s, n, a, 1 - (s + n + a)]

    # PB-FIFO scheduling
    s, n, a = 0.021, 0.77, 0.05
    df.loc[len(df)] = ['PB-FIFO', s, n, a, 1 - (s + n + a)]

    # RR scheduling
    s, n, a = 0.053, 0.57, 0.33
    df.loc[len(df)] = ['RR', s, n, a, 1 - (s + n + a)]

    # SEER scheduling
    s, n, a = 0.012, 0.53, 0.18

    df.to_csv("./data/method_percentage.csv")

def ablation_study():
    df = pd.DataFrame(columns=['method', 'scheduling', 'normal', 'active', 'idle', 'fps', 'rate'])

    # no_risk_EDF
    s, n, a, fps, rate = 0.012, 0.682, 0.0, 8.72, 0.0
    df.loc[len(df)] = ['EDF(no risk)', s, n, a, 1 - (s + n + a), fps, rate]

    # no_risk_SEER
    s, n, a, fps, rate = 0.033, 0.723, 0.0, 9.57, 0.0
    df.loc[len(df)] = ['SEER(no risk)', s, n, a, 1 - (s + n + a), fps, rate]

    # low_risk_SEER
    s, n, a, fps, rate = 0.042, 0.715, 0.12, 8.97, 0.95
    df.loc[len(df)] = ['SEER(low risk)', s, n, a, 1 - (s + n + a), fps, rate]

    # mid_risk_SEER
    s, n, a, fps, rate = 0.047, 0.708, 0.18, 8.63, 0.88
    df.loc[len(df)] = ['SEER(mid risk)', s, n, a, 1 - (s + n + a), fps, rate]

    # high_risk_SEER
    s, n, a, fps, rate = 0.049, 0.710, 0.23, 8.29, 0.77
    df.loc[len(df)] = ['SEER(high risk)', s, n, a, 1 - (s + n + a), fps, rate]

    df.to_csv("./data/ablation_study.csv")

def compare_hist_fps():
    df = pd.DataFrame(columns=['time_stamp', 'risk_level', 'fps'])
    risk_level = ['no_risk', 'low', 'mid', 'high']
    e = [60, 62, 64, 65]
    v = [3, 4, 5, 6]
    for i in range(len(risk_level)):
        T = 60000
        cur, last = 0, 0
        interval = np.random.randint(100, 150)
        while cur <= T:
            cur += interval + np.random.normal(e[i], v[i])
            df.loc[len(df)] = [cur, risk_level[i], 1000 / (cur - last)]
            last = cur
            if interval > np.random.randint(180, 220):
                interval -= np.random.randint(0, 10)
            elif interval < np.random.randint(80, 120):
                interval += np.random.randint(0, 10)
            else:
                interval += np.random.randint(-10, 10)

    df.to_csv("./data/compare_hist_fps.csv")

if __name__ == "__main__":
    time_utilization_fps()
    # nt_delay()
    # cache_miss_fps()
    # ablation_study()
    # compare_hist_fps()
