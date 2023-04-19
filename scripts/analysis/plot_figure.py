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
sns.set_style("whitegrid")
plt.rcParams["font.family"] = "Arial"
plt.rcParams["font.size"] = 12
plt.rcParams["font.weight"] = 'bold'
sns.set_context("paper", font_scale=1.5, rc={"lines.linewidth": 2.5})

# Set global matplotlib color palette
color_name = "tab10"

SAVE_FIG = False

# Show color palette
def show_color_palette():
    color_name = "tab10"
    fig, ax = plt.subplots(figsize=(10, 0.5))
    fig.colorbar(plt.cm.ScalarMappable(cmap=color_name), cax=ax, orientation='horizontal')
    plt.show()

# Motivation figures
def plot_time_utilization_fps():
    df = pd.read_csv("./data/time_utilization_fps.csv")
    df = df.iloc[1:]
    df['e2e'] = 1000 / df['fps']

    fig= plt.figure()
    ax = fig.add_subplot(111)
    ax.set_ylim((0, 100))
    ax.set_xlim((0, 2000))
    # sns.barplot(data=df, x='timestamp', y='utilization', width=100, color=next_color())
    ax.bar(x = df['timestamp'], height = df['utilization'], width=100, color='gray')

    ax2 = ax.twinx()
    ax2.set_ylim((0, 15))
    sns.lineplot(data=df, x='timestamp', y='fps', color=next_color(), linewidth=3,  marker='o', markersize=10, ax=ax2)
    # ax2.plot(df['timestamp'], df['fps'], color = next_color())

    ax.set_xlabel("Time (ms)")
    ax.set_ylabel("Utilization (%)")
    ax2.set_ylabel("Response rate (Hz)")

    if SAVE_FIG:
        plt.savefig("./result_img/time_utilization_fps.pdf", bbox_inches='tight', dpi=300)
        plt.savefig("./result_img/time_utilization_fps.png", bbox_inches='tight', dpi=300)
    else:
        plt.show()
    plt.clf()

def plot_bf_adding_active_tasks():
    # Declaring a figure "gnt"
    fig, gnt = plt.subplots()
    
    # Setting Y-axis limits
    gnt.set_ylim(0, 50)
    
    # Setting X-axis limits
    gnt.set_xlim(0, 160)
    
    # Setting labels for x-axis and y-axis
    gnt.set_xlabel('Seconds since start')
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
    
    if SAVE_FIG:
        plt.savefig("./result_img/bf_adding_active_tasks.pdf", bbox_inches='tight', dpi=300)
        plt.savefig("./result_img/bf_adding_active_tasks.png", bbox_inches='tight', dpi=300)
    else:
        plt.show()
    plt.clf()

def plot_utilization_wait_fps_thermal():
    df = pd.read_csv("./data/time_utilization_fps.csv")
    df = df.iloc[1:]
    df['e2e'] = 100 / df['fps']
    # Do map to every element in the column
    df['e2e'] = df['e2e'] ** 3 / 100

    plt.scatter(data=df, x='utilization', y='wait', s='e2e')

    plt.xlabel("Utilization (%)")
    plt.ylabel("Sum of wait time (ms)")
    plt.text(0.05, 0.85, 'Dot radius:\nResponse interval (ms)', ha='left', va='top', transform=plt.gca().transAxes)

    if SAVE_FIG:
        plt.savefig("./result_img/utilization_wait_fps_thermal.pdf", bbox_inches='tight', dpi=300)
        plt.savefig("./result_img/utilization_wait_fps_thermal.png", bbox_inches='tight', dpi=300)
    else:
        plt.show()

    # Clear figure
    plt.clf()

def plot_speed_sight_distance():
    df = pd.DataFrame(columns=['speed', 'sight_distance', 'e2e'])
    friction = 0.5
    SPEED = {'120': 120, '100': 100, '80': 80, '60': 60, '40': 40}
    for speed_type, v in SPEED.items():
        for e2e in range(100, 520, 20):
            s_distance = (0.278 * (e2e / 1000) * v) + v * v / (254 * friction)
            # df = df.append({'speed': speed_type, 'sight_distance': s_distance, 'e2e': e2e}, ignore_index=True)
            df.loc[len(df)] = [speed_type, s_distance, e2e]
    sns.lineplot(data=df, x='e2e', y='sight_distance', hue='speed', markers=True, style='speed', markersize=8, linewidth=3)
    plt.xlabel("Response time (ms)")
    plt.ylabel("Sight distance (m)")
    plt.tight_layout()

    if SAVE_FIG:
        plt.savefig("./result_img/speed_sight_distance.pdf", bbox_inches='tight', dpi=300)
        plt.savefig("./result_img/speed_sight_distance.png", bbox_inches='tight', dpi=300)

    else:
        plt.show()
    plt.clf()

def plot_cache_miss_execution_time():
    df = pd.read_csv("./data/cache_miss_fps.csv")
    df['num_core'] = df['num_core'].astype(str)
    # sns.lineplot(data=df, x='timestamp', y='execution_time', hue='num_core', markers=True, style='num_core', markersize=8, linewidth=3)
    sns.boxplot(data=df, x='num_core', y='cache_miss')
    plt.xlabel("Number of cores")
    plt.ylabel("Cache miss rate (%)")
    plt.tight_layout()

    if SAVE_FIG:
        plt.savefig("./result_img/cache_miss_execution_time.pdf", bbox_inches='tight', dpi=300)
        plt.savefig("./result_img/cache_miss_execution_time.png", bbox_inches='tight', dpi=300)
    else:
        plt.show()
    plt.clf()

# Evaluation figures
def plot_core_utilization_rate():
    # Load data to pandas
    df = pd.read_csv("./data/core_rrate_utilization.csv")

    ddf = pd.melt(df, id_vars=['core', 'risk_level'], value_vars=['utilization', 'reaction_rate'],
        var_name='type', value_name='val')

    print(ddf)
    
    sns.lineplot(data=ddf, x="core", y="val", hue="risk_level", style='type', markers=True, markersize=8, linewidth=3)
    # plt.scatter(data=df, x='core', y='', s='utilization')

    plt.xlabel("Number of cores")
    plt.ylabel("Percentage (%)")

    plt.tight_layout()

    if SAVE_FIG:
        plt.savefig("./result_img/core_utilization_rate.pdf", bbox_inches='tight', dpi=300)
        plt.savefig("./result_img/core_utilization_rate.png", bbox_inches='tight', dpi=300)
    else:
        plt.show()
    plt.clf()

def plot_core_risk_fps_baseline():
    pass

def plot_nt_delayed_by_rt_baseline():
    df = pd.read_csv("./data/nt_delay.csv")
    df = df.iloc[1:]

    f = plt.figure()
    f.set_figwidth(8)
    f.set_figheight(5)

    sns.boxplot(data=df, x="baseline", y="delay")
    plt.xlabel("Baseline")
    plt.ylabel("Delayed time (ms)")

    plt.tight_layout()

    if SAVE_FIG:
        plt.savefig("./result_img/nt_delayed_by_rt_baseline.pdf", bbox_inches='tight', dpi=300)
        plt.savefig("./result_img/nt_delayed_by_rt_baseline.png", bbox_inches='tight', dpi=300)
    else:
        plt.show()
    plt.clf()

def plot_ablation_study():
    df = pd.read_csv("./data/ablation_study.csv")
    # Plot horizontal stacked bar chart
    df['e2e'] = 1000 / df['fps']

    # Keep only vertical grids
    plt.grid(axis='y')

    names = ['EDF\nno risk', 'SEER\nno risk', 'SEER\nlow risk', 'SEER\nmid risk', 'SEER\nhigh risk']
    colors = {'scheduling': 'tab:blue', 'normal': 'tab:orange', 'active': 'tab:green', 'idle': 'tab:red'}
    hatch = {'scheduling': '///', 'normal': 'xxx', 'active': '+++', 'idle': '...'}
    df_bottom = [0, 0, 0, 0, 0]
    for field in ['normal', 'active', 'scheduling', 'idle']:
        plt.barh(names, df[field] * 100, left=df_bottom, label=field, color=colors[field], hatch=hatch[field])
        df_bottom += df[field] * 100

    plt.legend(loc='center left')
    plt.xlabel("Percentage (%)")
    # plt.ylabel("Baseline")

    plt.tight_layout()

    if SAVE_FIG:
        plt.savefig("./result_img/ablation_study.pdf", bbox_inches='tight', dpi=300)
        plt.savefig("./result_img/ablation_study.png", bbox_inches='tight', dpi=300)
    else:
        plt.show()
    plt.clf()

def plot_compare_hist_fps():
    df = pd.read_csv("./data/compare_hist_fps.csv")

    sns.histplot(data=df, x='fps', hue='risk_level', stat='count', common_norm=False, bins=30, kde=True, alpha=0.1)
    plt.xlabel("Response rate (Hz)")

    # Add mean and variance lines
    df_mean = df.groupby('risk_level').mean()
    print(df_mean)
    # plt.axvline(data=df_mean, x='fps', hue='risk_level', linestyle='--')

    plt.tight_layout()

    if SAVE_FIG:
        plt.savefig("./result_img/compare_hist_fps.pdf", bbox_inches='tight', dpi=300)
        plt.savefig("./result_img/compare_hist_fps.png", bbox_inches='tight', dpi=300)
    else:
        plt.show()
    plt.clf()

def test():
    x = [[1, 2, 3], [3, 2, 1], [2, 1, 2]]
    y = ['A', 'B', 'C']

    # Create horizontal stacked bar chart
    plt.barh(y, x[0], label='Category 1')
    plt.barh(y, x[1], left=x[0], label='Category 2')
    # plt.barh(y, x[2], left=[sum(x[:2])] * len(y), label='Category 3')

    # Add legend
    plt.legend()

    # Show plot
    plt.show()
    plt.clf()

    
if __name__ == "__main__":
    SAVE_FIG = True
    # show_color_palette()
    # test()
    plot_utilization_wait_fps_thermal()
    # plot_time_utilization_fps()
    # plot_core_utilization_rate()
    # plot_bf_adding_active_tasks()
    # plot_nt_delayed_by_rt_baseline()
    # plot_speed_sight_distance()
    # plot_cache_miss_execution_time()
    # plot_ablation_study()
    # plot_compare_hist_fps()
    # pass

    
    