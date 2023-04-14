import os
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Set working directory to the directory of this script
working_dir = os.path.dirname(__file__)
os.chdir(working_dir)

# Set plotting theme
sns.set_theme(style="whitegrid", font_scale=1.4)

PALLET1 = ['#2878b5', '#9ac9db', '#f8ac8c', '#c82423', '#ff8884', '#96c37d', '#c497b2', '#f3d266']
color_idx = 0
def next_color():
    global color_idx
    color_idx += 1
    return PALLET1[color_idx % len(PALLET1)]

# Motivation figures
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
    
if __name__ == "__main__":
    # plot_core_utilization_rate()
    plot_bf_adding_active_tasks()
    
    