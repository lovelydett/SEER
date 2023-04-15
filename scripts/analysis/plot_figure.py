import os
import pandas as pd
import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt
from theme import next_color

# Set working directory to the directory of this script
working_dir = os.path.dirname(__file__)
os.chdir(working_dir)

# Set plotting theme
sns.set_theme(style="whitegrid", font_scale=1.4)

# Motivation figures
def plot_time_utilization_fps():
    df = pd.read_csv("./data/time_utilization_fps.csv")
    df = df.iloc[1:]
    df['e2e'] = 1000 / df['fps']

    fig= plt.figure()
    ax = fig.add_subplot(111)
    ax.set_ylim((0, 100))
    # sns.barplot(data=df, x='timestamp', y='utilization', width=100, color=next_color())
    ax.bar(x = df['timestamp'], height = df['utilization'], width=100, color='gray')

    ax2 = ax.twinx()
    ax2.set_ylim((0, 180))
    sns.lineplot(data=df, x='timestamp', y='e2e', color=next_color(), linewidth=3,  marker='o', markersize=10, ax=ax2)
    # ax2.plot(df['timestamp'], df['fps'], color = next_color())

    plt.show()


def plot_bf_adding_active_tasks():
    # Declaring a figure "gnt"
    fig, gnt = plt.subplots()
    
    # Setting Y-axis limits
    gnt.set_ylim(0, 50)
    
    # Setting X-axis limits
    gnt.set_xlim(0, 160)
    
    # Setting labels for x-axis and y-axis
    gnt.set_xlabel('seconds since start')
    gnt.set_ylabel('Processor')
    
    # Setting ticks on y-axis
    gnt.set_yticks([15, 25, 35])
    # Labelling tickes of y-axis
    gnt.set_yticklabels(['1', '2', '3'])
    
    # Setting graph attribute
    gnt.grid(True)
    
    # Declaring a bar in schedule
    gnt.broken_barh([(40, 50)], (30, 9), facecolors = next_color())
    
    # Declaring multiple bars in at same level and same width
    gnt.broken_barh([(110, 10), (150, 10)], (10, 9), facecolors = next_color())
    
    gnt.broken_barh([(10, 50), (100, 20), (130, 10)], (20, 9),
                                    facecolors = next_color())
    # gnt.broken_barh([(10, 50), (100, 20), (130, 10)], (20, 9))
    
    plt.show()

def plot_utilization_wait_fps_thermal():
    df = pd.read_csv("./data/time_utilization_fps.csv")
    df = df.iloc[1:]
    df['e2e'] = 600 / df['fps']
    plt.scatter(data=df, x='utilization', y='wait', s='e2e')
    plt.show()

def plot_speed_sight_distance():
    df = pd.DataFrame(columns=['speed', 'sight_distance', 'e2e'])
    friction = 0.5
    SPEED = {'120 km/h': 120, '100 km/h': 100, '80 km/h': 80, '60 km/h': 60, '40 km/h': 40}
    for speed_type, v in SPEED.items():
        for e2e in range(100, 310, 10):
            s_distance = (0.278 * (e2e / 1000) * v) + v * v / (254 * friction)
            print(s_distance)
            df = df.append({'speed': speed_type, 'sight_distance': s_distance, 'e2e': e2e}, ignore_index=True)
    sns.lineplot(data=df, x='e2e', y='sight_distance', hue='speed', marker='o', markersize=10, linewidth=3)
    plt.show()


# Evaluation figures 
def plot_core_utilization_rate():
    # Load data to pandas
    df = pd.read_csv("./data/core_rrate_utilization.csv")

    ddf = pd.melt(df, id_vars=['core', 'num_risk'], value_vars=['utilization', 'reaction_rate'],
        var_name='type', value_name='val')
    
    # sns.lineplot(data=df, x="core", y="utilization", hue="num_risk", linestyle = '--')
    # ax2 = plt.twinx()
    # sns.lineplot(data=df, x="core", y="reaction_rate", hue="num_risk", ax = ax2)
    # plt.show()
    
    sns.lineplot(data=ddf, x="core", y="val", hue="num_risk", style = 'type')
    plt.show()

def plot_core_risk_fps_baseline():
    pass

def plot_nt_delayed_by_rt_baseline():
    df = pd.read_csv("./data/nt_delay.csv")
    df = df.iloc[1:]

    f = plt.figure()
    f.set_figwidth(8)
    f.set_figheight(5)

    sns.boxplot(data=df, x="baseline", y="delay")
    plt.show()
    
if __name__ == "__main__":
    # plot_utilization_wait_fps_thermal()
    # plot_time_utilization_fps()
    # plot_core_utilization_rate()
    # plot_bf_adding_active_tasks()
    # plot_nt_delayed_by_rt_baseline()
    plot_speed_sight_distance()
    
    