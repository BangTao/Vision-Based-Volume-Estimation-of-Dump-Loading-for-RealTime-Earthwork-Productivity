# Vision-Based-Volume-Estimation-of-Dump-Loading-for-RealTime-Earthwork-Productivity

This repository contains source code, raw data, and implementation details for the paper:

***Automatic Vision-Based Volume Estimation of Dump Loading for Real-Time Earthwork Productivity Assessment and Management***  

(**Submitted to Automation in Construction**)

---

## 🔧 Repository Structure

The repository is organized into four main directories corresponding to different algorithmic components:

### 1. [The developed algorithm for converting depth maps captured by the RGB-D camera into 3D point clouds](./1.The%20developed%20algorithm%20for%20converting%20depth%20maps%20captured%20by%20the%20RGB-D%20camera%20into%203D%20point%20clouds/)
Code and method description for generating 3D point clouds from depth images obtained using an RGB-D camera.

### 2. [The developed algorithm for estimating the volume of earthwork transported by a dump truck body](./2.The%20developed%20algorithm%20for%20estimating%20the%20volume%20of%20earthwork%20transported%20by%20a%20dump%20truck%20body/)
C++ implementation of the slicing-based volume estimation algorithm based on PCL Convex Hull.

### 3. [Fig17_RawData_and_SourceCode](./3.Fig17_RawData_and_SourceCode/)
Python source code, CSV data, and output image related to **Figure 17**, which evaluates the performance of the proposed method in terms of earthwork productivity.

### 4. [TruckDimensions_and_Specifications](./4.TruckDimensions_and_Specifications/)
PDF documentation providing detailed dimensions and specifications of the 42 dump trucks used in the field experiments.

---

## 📄 License

This project is licensed under the **Apache License 2.0**. See the [LICENSE](./LICENSE) file for details.

---

## 🔗 Citation

If you find this repository useful in your research, please consider citing the original paper (citation info will be updated after publication).

---

## 🔍 Contact

For any questions or collaboration inquiries, please contact the authors via GitHub Issues or the corresponding author's email listed in the paper.
