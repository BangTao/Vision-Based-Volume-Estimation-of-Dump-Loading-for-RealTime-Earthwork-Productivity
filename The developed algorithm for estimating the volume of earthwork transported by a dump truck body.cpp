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
    // ��ȡ��������
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    if (pcl::io::loadPCDFile<pcl::PointXYZ>("test.pcd", *cloud) == -1) 
    {
        PCL_ERROR("Couldn't read file.\n");
        return -1;
    }
    // ����Ϊ0.5�׼���Ĳ�
    double height_interval ;

    std::cout << "Enter the height interval..." << std::endl;
    std::cin >> height_interval;

    float alpha;
    std::cout << "Enter the alpha value.." << std::endl;
    std::cin >> alpha;

    // ������Ƶ�z�������С�����ֵ
    pcl::PointXYZ minPt, maxPt;
    pcl::getMinMax3D(*cloud, minPt, maxPt);

    
    int num_layers = static_cast<int>((maxPt.z - minPt.z) / height_interval) + 1;
    double total_volume = 0;
    double total_volume2 = 0;
    for (int i = 0; i < num_layers; ++i) 
    {
        // ���õ�ǰ��ĸ߶ȷ�Χ
        double min_height = minPt.z + i * height_interval;
        double max_height = minPt.z + (i + 1) * height_interval;

        // ��ȡ�ڵ�ǰ�߶ȷ�Χ�ڵĵ���
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


            // ��ȡ����
            pcl::ExtractIndices<pcl::PointXYZ> extract;
            extract.setInputCloud(cloud);
            extract.setIndices(inliers);
            pcl::PointCloud<pcl::PointXYZ>::Ptr layer_cloud(new pcl::PointCloud<pcl::PointXYZ>);
            extract.filter(*layer_cloud);

            // �����ά͹��
            pcl::ConvexHull<pcl::PointXYZ> chull;
            chull.setInputCloud(layer_cloud);
            pcl::PointCloud<pcl::PointXYZ>::Ptr hull(new pcl::PointCloud<pcl::PointXYZ>);
            chull.reconstruct(*hull);


            
               // ���� Concave Hull ����
               pcl::ConcaveHull<pcl::PointXYZ> concave_hull;

               // �����������
               concave_hull.setInputCloud(layer_cloud);

               // ���� alpha ���������Ը�����Ҫ������
               concave_hull.setAlpha(alpha);

               // ���㰼��
               pcl::PointCloud<pcl::PointXYZ>::Ptr hull2(new pcl::PointCloud<pcl::PointXYZ>);
               concave_hull.reconstruct(*hull2);

               

               // ����͹�������
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
