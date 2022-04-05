# 竞赛题目

## **初赛题目**
- 所有题目不需要生成bit文件，完成co-sim仿真即可

### 基础题 

#### DSP （2题必做1题）

1.CORDIC

  > CORDIC (for COordinate Rotation DIgital Computer) is a simple and efficient algorithm to calculate trigonometric functions, hyperbolic functions, square roots, multiplications, divisions, and exponentials and logarithms with arbitrary base, typically converging with one digit (or bit) per iteration.

  >
  > [**Details**](https://github.com/xupsh/ccc/tree/main/problems/cordic)


2.DFT

  > DFT is a common operation in signal processing which generates a discrete frequency domain representation of the discrete input signal. The input signal is a vector of samples and the matrix is a set of basis functions corresponding to discrete cosine and sine waveforms of different frequencies. The multiplication of the input signal with these basis functions describes how well the input signal correlates with those waveforms, which is the value of the Fourier series at that frequency.


  >
  > [**Details**](https://github.com/xupsh/ccc/tree/main/problems/DFT)

#### CV （3题必做1题）

1.FAST

  > Features from accelerated segment test (FAST) is a corner detection method, which could be used to extract feature points and later used to track and map objects in many computer vision tasks. FAST corner detector uses a circle of 16 pixels (a Bresenham circle of radius 3) to classify whether a candidate point p is actually a corner. 
  >
  > [**Details**](https://github.com/xupsh/ccc/tree/main/problems/fast)

2.Harris

  > The Harris affine detector can identify similar regions between images that are related through affine transformations and have different illuminations. At a corner, the image intensity will change largely in multiple directions. Harris examines the changes of intensity due to shifts in a local window and uses the second moment matrix as the basis of its corner decisions.
  >
  > [**Details**](https://github.com/xupsh/ccc/tree/main/problems/harris)

3.Canny

  > The Canny edge detector is an edge detection operator that uses a multi-stage algorithm to detect a wide range of edges in images. 
  > + Apply Gaussian filter to smooth the image in order to remove the noise. Find the intensity gradients of the image
  > + Apply gradient magnitude thresholding or lower bound cut-off suppression to get rid of spurious response to edge detection
  > + Apply double threshold to determine potential edges
  > + Track edge by hysteresis: Finalize the detection of edges by suppressing all the other edges that are weak and not connected to strong edges.
  >
  > [**Details**](https://github.com/xupsh/ccc/tree/main/problems/canny)


### 高级题 2题选1（不提供参考代码，5月15日后发布评测细节）

1.**Graph:** Betweenness
  
  > Betweenness is a graph analysis algorithm. It represents the degree to which nodes stand between each other. For every pair of vertices in a connected graph, there exists at least one shortest path between the vertices such that either the number of edges that the path passes through (for unweighted graphs) or the sum of the weights of the edges (for weighted graphs) is minimized. The betweenness centrality for each vertex is the number of these shortest paths that pass through the vertex.
  > A reference implementation of the betweenness calculation can be found at: [**reference**](https://gitenterprise.xilinx.com/qianqiao/ccc2021/blob/main/problems/betweenness/test.cpp#L84)
  >
  > [**Details**](https://github.com/xupsh/ccc/tree/main/problems/betweenness)

2.**Big Data:** Serialization

  > The serialization is usually used in the Big Data analysis to convert data into portable structure. It is the process of converting data object into a stream of bytes or binary format that is easy to transmit or store. The target serialized format is Arrow IPC defined in open-sourced Apache Arrow project and can be found in the following link: [**reference**](https://github.com/apache/arrow/blob/master/docs/source/format/Columnar.rst#serialization-and-interprocess-communication-ipc)
  >
  > [**Details**](https://github.com/xupsh/ccc/tree/main/problems/serialization)



<!--

!-->  

