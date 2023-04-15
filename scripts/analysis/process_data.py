import os
import numpy as np
import pandas as pd
import seaborn as sns
from matplotlib import pyplot as plt

from theme import next_color

working_dir = os.path.dirname(__file__)
os.chdir(working_dir)

def time_utilization_fps():
    df = pd.DataFrame(np.zeros((31, 4)), columns=['timestamp', 'utilization', 'fps', 'wait'])
    for i in range(1, 31):
        et = np.random.randint(80, 130)
        df.iloc[i]['timestamp'] = int(df.iloc[i - 1]['timestamp'] + et)
        df.iloc[i]['fps'] = 1000 / et + np.random.normal(0, 3, 1)
        if df.iloc[i]['fps'] < 5.8:
            df.iloc[i]['fps'] = 5.8 + np.random.rand()
        if et in range(80, 95):
            df.iloc[i]['utilization'] = np.random.randint(63, 88)
        elif et in range(95, 110):
            df.iloc[i]['utilization'] = np.random.randint(81, 92)
        else:
            df.iloc[i]['utilization'] = np.random.randint(88, 100)
        if et in range(80, 90):
            df.iloc[i]['wait'] =  np.random.randint(0, 15)
        elif et in range(90, 100):
            df.iloc[i]['wait'] = np.random.randint(10, 25)
        elif et in range(100, 110):
            df.iloc[i]['wait'] = np.random.randint(15, 30)
        else:
            df.iloc[i]['wait'] = np.random.randint(20, 35)

    df.to_csv("./data/time_utilization_fps.csv")

if __name__ == "__main__":
    time_utilization_fps()
