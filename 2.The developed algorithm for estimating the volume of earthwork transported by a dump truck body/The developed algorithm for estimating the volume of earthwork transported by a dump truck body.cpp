#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/surface/convex_hull.h>
#include <pcl/common/common.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <pcl/surface/concave_hull.h>



double computeConvexHullArea(const pcl::PointCloud<pcl::PointXYZ>::Ptr& cloud) 
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr hull(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::ConvexHull<pcl::PointXYZ> chull;
    chull.setInputCloud(cloud);
    chull.reconstruct(*hull);

    double area = 0.0;
    for (size_t i = 0; i < hull->points.size(); ++i) 
    {
        size_t next = (i + 1) % hull->points.size();
        area += hull->points[i].x * hull->points[next].y - hull->points[next].x * hull->points[i].y;
    }

    return std::abs(area) * 0.5;
}



int main() 
{
    // 读取点云数据
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    if (pcl::io::loadPCDFile<pcl::PointXYZ>("test.pcd", *cloud) == -1) 
    {
        PCL_ERROR("Couldn't read file.\n");
        return -1;
    }
    // 划分为0.5米间隔的层
    double height_interval ;

    std::cout << "Enter the height interval..." << std::endl;
    std::cin >> height_interval;

    float alpha;
    std::cout << "Enter the alpha value.." << std::endl;
    std::cin >> alpha;

    // 计算点云的z坐标的最小和最大值
    pcl::PointXYZ minPt, maxPt;
    pcl::getMinMax3D(*cloud, minPt, maxPt);

    
    int num_layers = static_cast<int>((maxPt.z - minPt.z) / height_interval) + 1;
    double total_volume = 0;
    double total_volume2 = 0;
    for (int i = 0; i < num_layers; ++i) 
    {
        // 设置当前层的高度范围
        double min_height = minPt.z + i * height_interval;
        double max_height = minPt.z + (i + 1) * height_interval;

        // 提取在当前高度范围内的点云
        pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
        for (size_t j = 0; j < cloud->points.size(); ++j) 
        {
            if (cloud->points[j].z >= min_height && cloud->points[j].z < max_height) 
            {
                inliers->indices.push_back(static_cast<int>(j));
            }
        }
        if (inliers->indices.size()>3)
        {


            // 提取点云
            pcl::ExtractIndices<pcl::PointXYZ> extract;
            extract.setInputCloud(cloud);
            extract.setIndices(inliers);
            pcl::PointCloud<pcl::PointXYZ>::Ptr layer_cloud(new pcl::PointCloud<pcl::PointXYZ>);
            extract.filter(*layer_cloud);

            // 计算二维凸包
            pcl::ConvexHull<pcl::PointXYZ> chull;
            chull.setInputCloud(layer_cloud);
            pcl::PointCloud<pcl::PointXYZ>::Ptr hull(new pcl::PointCloud<pcl::PointXYZ>);
            chull.reconstruct(*hull);


            
               // 创建 Concave Hull 对象
               pcl::ConcaveHull<pcl::PointXYZ> concave_hull;

               // 设置输入点云
               concave_hull.setInputCloud(layer_cloud);

               // 设置 alpha 参数（可以根据需要调整）
               concave_hull.setAlpha(alpha);

               // 计算凹包
               pcl::PointCloud<pcl::PointXYZ>::Ptr hull2(new pcl::PointCloud<pcl::PointXYZ>);
               concave_hull.reconstruct(*hull2);

               

               // 计算凸包的面积
            double area = computeConvexHullArea(hull);
            double area2 = computeConvexHullArea(hull2);
            //    std::cout << i << " layer area: " << area << std::endl;
            total_volume = total_volume + area * height_interval;
            total_volume2 = total_volume2 + area2 * height_interval;
        }
    }
    std::cout << "total volume using Convexhull: " << total_volume<<" m3" << std::endl;
    std::cout << "total volume using Concavehull: " << total_volume2  << " m3" << std::endl;
    
    
    system("pause");
    return 0;
}
