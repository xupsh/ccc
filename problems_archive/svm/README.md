## Repository Tree
```
.
├── top.cpp                       //源码示例
├── top.hpp                       //源码示例
├── enums.hpp                     //源码示例
├── stream_local_processing.hpp   //源码示例
├── svm_predict.hpp               //源码示例
├── table_sample.hpp              //源码示例
├── test.cpp                      //测试源码
├── 1000.csv                      //测试输入
├── run_hls.tcl                   //tcl文件
└── README.md                     //说明文件
```
## Prediction of SVM Model
  **5月14日，我们对三道中级题的顶层模块进行了细节调整，主要是为了TB的安全性,烦请5月14日之前下载赛题的选手下载最新版本的赛题。**
  >
  1）	框架代码说明
  >您可以直接在提供代码的基础上进行HLS优化，也可以根据您的设计对其适当的修改。  
  >但请注意：
  >**您必须保持顶层函数不变。**

  2）项目要求  
  >在设计完成后，您可以使用我们提供的test.cpp文件进行基本功能测试。但是，由于在评分时会统一使用评分系统内部的test.cpp，因此请不要将设计写在test.cpp中，test.cpp文件也无需提交。
  
  >**在HLS优化的过程中，请务必将优化指令设置为pragma写入c++代码中。**
  
  >比赛不需要生成最终bit文件，仅考察算法实现功能和综合报告的性能，能完成co-sim即可。

  3）提交程序
  >请将除test.cpp以外的所有cpp和hpp文件压缩为一个压缩包svm.zip，在页面下方您的解答处点击“上传代码”按钮进行提交，稍待片刻即可在文本框下方得知您的设计是否通过了基本功能测试。延迟、面积等性能指标则可在历史提交记录中点击版本号详细查看。
  
  4）评分规则
  >性能公式为：<br>
  >如果最大频率小于100MHz，视为未通过基本功能<br>
  >如果最大频率大于等于100MHz，但小于300MHz，使用公式**RTL simulation time/clock period/fmax**计算性能<br>
  >如果最大频率大于等于300MHz，使用公式**RTL simulation time/clock period/300MHz**计算性能<br>
  >其中RTL simulation time是Vitis HLS软件Cosimulation所用时间，clock period是tcl中设定的时钟频率，fmax是设计的最高运行频率。
  >性能公式计算出的数值越小，性能越好。
  >
  >注意：设计的频率范围建议在100MHz和300MHz之间。如果频率低于100MHz，则未能满足最低性能指标，无法得到基本分10分；如果设计的最高运行频率高于300MHz，则在上方公式中也只会取300MHz作为最高频率。
