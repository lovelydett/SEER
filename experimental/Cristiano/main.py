import pandas as pd
import matplotlib.pyplot as plt

# 讀取CSV文件並解析日期時間
event1 = pd.read_csv("event_point_2_cores.csv", parse_dates=['time_ms'])
event2 = pd.read_csv("event_point_4_cores.csv", parse_dates=['time_ms'])
event3 = pd.read_csv("event_point_8_cores.csv", parse_dates=['time_ms'])

def gantt(event):
    # 找到第一個exit_task_1事件的行
    event_exit_task_1_row = event.loc[event['event'] == 'exit_task_1'].iloc[0]

    # 取出exit_task日志記錄时间
    end_time = event_exit_task_1_row['time_ms']

    # 計算前500ms的时间
    start_time = str(int(end_time) - 500)

    # 選取時間範圍內的數據
    event_slice = event.loc[(event['time_ms'] >= start_time) & (event['time_ms'] <= end_time)]

    # 提取必要的數據
    tid = event_slice.iloc[:, 0].tolist()
    unique_tid = event_slice.iloc[:, 0].drop_duplicates().tolist()
    time_ms = event_slice.iloc[:, 1].tolist()
    events = event_slice.iloc[:, 2].tolist()
    unique_event = event_slice.iloc[:, 2].drop_duplicates().tolist()
    value = event_slice.iloc[:, 3].tolist()
    event_start_time = []
    for i in range(len(time_ms)):
        event_start_time.append(str(int(time_ms[i]) - int(value[i])))

    # 創建圖形和軸對象
    fig, ax = plt.subplots(figsize=(len(time_ms), len(unique_event)))

    # 設置x軸標籤
    ax.set_xlabel('Time (ms)')
    ax.set_xticks(range(len(event_start_time)))
    ax.set_xticklabels(event_start_time)

    # 設置y軸標籤
    ax.set_ylabel('event')
    ax.set_yticks(range(len(unique_event)))
    ax.set_yticklabels(unique_event)
    # 繪製甘特圖
    for i in range(len(events)):
        start = int(event_start_time[i])
        end = int(time_ms[i])
        duration = int(value[i])
        tids = int(tid[i])

        for j in range(len(unique_event)):
            if events[i] == unique_event[j]:
                if tids == unique_tid[0]:
                    ax.barh(j, duration, left=start, height=0.5, label=unique_tid[0], color='orange', align='center')
                    fig.text(start, i, str(start), ha='center', va='center')
                elif tids == unique_tid[1]:
                    ax.barh(j, duration, left=start, height=0.5, label=unique_tid[1], color='green', align='center')
                    fig.text(start, i, str(start), ha='center', va='center')
                elif tids == unique_tid[2]:
                    ax.barh(j, duration, left=start, height=0.5, label=unique_tid[2], color='blue', align='center')
                    fig.text(start, i, str(start), ha='center', va='center')
                elif tids == unique_tid[3]:
                    ax.barh(j, duration, left=start, height=0.5, label=unique_tid[3], color='red', align='center')
                    fig.text(start, i, str(start), ha='center', va='center')
                elif tids == unique_tid[4]:
                    ax.barh(j, duration, left=start, height=0.5, label=unique_tid[4], color='yellow', align='center')
                    fig.text(start, i, str(start), ha='center', va='center')
                elif tids == unique_tid[5]:
                    ax.barh(j, duration, left=start, height=0.5, label=unique_tid[5], color='black', align='center')
                    fig.text(start, i, str(start), ha='center', va='center')
                elif tids == unique_tid[6]:
                    ax.barh(j, duration, left=start, height=0.5, label=unique_tid[6], color='pink', align='center')
                    fig.text(start, i, str(start), ha='center', va='center')
                elif tids == unique_tid[7]:
                    ax.barh(j, duration, left=start, height=0.5, label=unique_tid[7], color='purple', align='center')
                    fig.text(start, i, str(start), ha='center', va='center')
                elif tid == unique_tid[8]:
                    ax.barh(j, duration, left=start, height=0.5, label=unique_tid[8], color='brown', align='center')
                    fig.text(start, i, str(start), ha='center', va='center')

    # 顯示圖例
    plt.legend(loc='lower right', title='tid', fontsize='small', frameon=False)

    # 顯示圖形
    plt.show()

if __name__ == '__main__':
    event_point = input("Please input the number of event point(2/4/8): ")
    if event_point == '2':
        gantt(event1)
    elif event_point == '4':
        gantt(event2)
    elif event_point == '8':
        gantt(event3)
    else:
        print("Please input the correct number of event point!")
