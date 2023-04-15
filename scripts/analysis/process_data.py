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

if __name__ == "__main__":
    # time_utilization_fps()
    nt_delay()
