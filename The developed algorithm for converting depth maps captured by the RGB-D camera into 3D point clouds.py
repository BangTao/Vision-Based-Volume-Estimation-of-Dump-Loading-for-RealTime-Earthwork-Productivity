
import pyrealsense2 as rs
import numpy as np
import open3d as o3d
import cv2
import os

# ========== 配置 ==========
output_path = "output_cloud.ply"
depth_scale = None  # 自动读取标定值
depth_max = 4.0  # 最大有效深度（单位：米）

# ========== 初始化 RealSense ==========
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
profile = pipeline.start(config)

# 获取内参与比例
depth_sensor = profile.get_device().first_depth_sensor()
depth_scale = depth_sensor.get_depth_scale()
print(f"[INFO] 深度比例: {depth_scale} 米/单位")

align_to = rs.stream.color
align = rs.align(align_to)

# ========== 捕获一帧并对齐 ==========
frames = pipeline.wait_for_frames()
aligned_frames = align.process(frames)
depth_frame = aligned_frames.get_depth_frame()
color_frame = aligned_frames.get_color_frame()

if not depth_frame or not color_frame:
    raise RuntimeError("未能获取有效的对齐帧")

color_image = np.asanyarray(color_frame.get_data())
depth_image = np.asanyarray(depth_frame.get_data())

# ========== 获取相机内参 ==========
depth_intrinsics = depth_frame.profile.as_video_stream_profile().intrinsics
fx, fy = depth_intrinsics.fx, depth_intrinsics.fy
cx, cy = depth_intrinsics.ppx, depth_intrinsics.ppy
height, width = depth_image.shape

# ========== 点云构建 ==========
points = []
colors = []

for v in range(height):
    for u in range(width):
        z = depth_image[v, u] * depth_scale
        if z == 0 or z > depth_max:
            continue
        x = (u - cx) * z / fx
        y = (v - cy) * z / fy
        points.append([x, y, z])
        colors.append(color_image[v, u] / 255.0)  # 归一化 RGB

# ========== 转换为 Open3D 格式并保存 ==========
pcl = o3d.geometry.PointCloud()
pcl.points = o3d.utility.Vector3dVector(np.array(points))
pcl.colors = o3d.utility.Vector3dVector(np.array(colors))

print(f"[INFO] 点云点数: {len(points)}")
o3d.io.write_point_cloud(output_path, pcl)
print(f"[INFO] 点云已保存为 {output_path}")

pipeline.stop()
