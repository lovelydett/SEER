PALLET1 = ['#2878b5', '#9ac9db', '#f8ac8c', '#c82423', '#ff8884', '#96c37d', '#c497b2', '#f3d266']
color_idx = 0
def next_color():
    global color_idx
    color_idx += 1
    return PALLET1[color_idx % len(PALLET1)]