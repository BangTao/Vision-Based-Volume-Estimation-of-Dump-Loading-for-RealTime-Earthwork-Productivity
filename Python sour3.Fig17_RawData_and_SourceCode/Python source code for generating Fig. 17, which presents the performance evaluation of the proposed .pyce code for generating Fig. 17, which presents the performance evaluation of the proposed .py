# Author: DBT
# Date: 2025/4/1
# Description: Styled comparison plot for truck cycle time and volume with dynamic y-axis range and 20s tick spacing

import pandas as pd
import matplotlib.pyplot as plt

# ========== Step 1: Load Data ==========
df = pd.read_csv("35_Dump_Trucks_Volume and CycleTime Source Data.csv")

# 替换 Truck ID 中的问号为星号
df["Truck ID"] = df["Truck ID"].str.replace("?", "*", regex=False)

# ========== Step 2: Extract Data ==========
x = df["Truck ID"]
x_indices = range(len(x))
bar_width = 0.35

# 使用文件中的真实列名
obs = df["Ground-truth Cycle Time (s)"]
pred = df["Estimated Cycle Time by Proposed Method (s)"]
vol_true = df["Ground-truth Volume (m3)"]
vol_pred = df["Estimated Volume by Proposed Method (m3)"]

# ========== Step 3: Plot ==========
fig, ax1 = plt.subplots(figsize=(14, 6))
ax2 = ax1.twinx()

# 设置折线图优先显示在柱状图之上
ax1.set_zorder(ax2.get_zorder() + 1)
ax1.patch.set_visible(False)

# ---------- 柱状图（体积） ----------
ax2.bar(
    [i - bar_width / 2 for i in x_indices],
    vol_true,
    width=bar_width,
    color="lightgray",
    zorder=1,
    label="Ground-truth Volume",
)
ax2.bar(
    [i + bar_width / 2 for i in x_indices],
    vol_pred,
    width=bar_width,
    color="#96C37D",
    zorder=1,
    label="Estimated Volume",
)

# # ---------- 在预测柱上显示体积相对误差（百分比，保留两位小数，加粗） ----------
# for i, (v_true, v_pred) in enumerate(zip(vol_true, vol_pred)):
#     error = abs(v_pred - v_true) / v_true
#     ax2.text(
#         i + bar_width / 2,  # X 位置：预测柱顶中心
#         v_pred + 0.2,  # Y 位置：略高于柱顶
#         f"{error*100:.2f}%",  # 格式为百分比，保留两位小数
#         ha="center",
#         va="bottom",
#         fontsize=9,
#         color="black",  # 加粗文字
#     )


# ---------- 折线图（循环时间） ----------
ax1.plot(
    x,
    obs,
    linestyle="--",
    linewidth=1.5,
    color="black",
    zorder=10,
    label="Ground-truth Cycle Time",
)  # Ground-truth：黑色虚线
ax1.plot(
    x,
    pred,
    linestyle="-",
    linewidth=1.5,
    color="#EF7A6D",
    zorder=11,
    label="Estimated Cycle Time",
)  # Prediction：浅绿色实线
ax1.scatter(x, pred, color="#d73027", s=30, zorder=12)  # 红点标注预测值

ax1.legend(loc="upper right", fontsize=11, frameon=False)

ax1.legend(loc="upper right", bbox_to_anchor=(1.0, 1.0), fontsize=11, frameon=False)

# 设置 Y 轴范围和刻度
combined = pd.concat([obs, pred])
data_min = combined.min()
data_max = combined.max()

tick_interval = 60
extra_margin = 120  # 顶部额外多出两个刻度单位

y_min = int((data_min // 20 - 1) * 20)
y_max = int((data_max // 20 + 2) * 20) + extra_margin

ax1.set_ylim(y_min, y_max)
ax1.set_yticks(list(range(y_min, y_max + 1, tick_interval)))

# 设置 ax2（右）Volume 范围，给柱状图顶部留空间
vol_combined = pd.concat([vol_true, vol_pred])
vol_max = vol_combined.max()
vol_margin = vol_max * 0.15
ax2.set_ylim(0, vol_max + vol_margin)

# 在保存或显示前增加顶部空间
plt.subplots_adjust(top=0.90)


# ========== Step 4: Label Settings ==========
ax1.set_ylabel("Cycle Time (s)", fontsize=13, fontweight="bold")
ax2.set_ylabel("Earthmoving Volume (m³)", fontsize=13, fontweight="bold")
ax1.set_xlabel("Truck’s license plate number", fontsize=13, fontweight="bold")
ax1.tick_params(axis="x", rotation=90)

# 控制左右边界距离
ax1.set_xlim(-0.6, len(x) - 0.4)

# 设置刻度数字颜色为黑色（默认值）
ax1.tick_params(axis="both", colors="black")
ax2.tick_params(axis="y", colors="black")

# # 设置上下左右边框颜色为浅灰，并加粗
# for spine in ["top", "bottom", "left", "right"]:
#     ax1.spines[spine].set_color("lightgray")
#     ax1.spines[spine].set_linewidth(1.8)
#
#     ax2.spines[spine].set_color("lightgray")
#     ax2.spines[spine].set_linewidth(1.8)


# 不显示图例（避免报错）
# 若需图例，请为每个 plot 添加 label 并使用 ax1.legend()

# ========== Step 5: Save & Show ==========
# plt.title("Cycle Time and Volume Comparison per Truck (Styled)")

# 合并 ax1 和 ax2 的图例项
lines_1, labels_1 = ax1.get_legend_handles_labels()
lines_2, labels_2 = ax2.get_legend_handles_labels()

ax1.legend(
    handles=lines_1 + lines_2,
    labels=labels_1 + labels_2,
    loc="upper right",
    bbox_to_anchor=(1.0, 1.0),
    fontsize=12,
    frameon=False,
    ncol=2,  # 两列显示
    columnspacing=2.0,  # 控制两列之间的间距
)

plt.tight_layout()
plt.savefig("2025.04.02-02.jpg", dpi=300, bbox_inches=None)
plt.show()
