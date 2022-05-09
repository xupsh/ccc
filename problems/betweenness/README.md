## Repository Tree
```
.
├── test.cpp      //源码框架
├── top.hpp       //头文件
├── top.cpp       //测试源码
├── data          //标准输入
├── run_hls.tcl   //tcl文件
└── README.md     //说明文件
```

## Betweenness （最近一次更新：5月9日）
1. 框架代码说明

    框架代码是算法的C-model，
    您需要自行设计可综合co-sim的设计。  
    但请注意：**您必须保持顶层函数不变。**
    
2. 项目要求  
    ![](../assets/1.png)
  
  
    •numVert: how many Vertices in the graph
  
    •NumEdge: how many edges om the graph
  
    •Offset: offset of the csr graph
  
    •Column: column of the csr graph
  
    •Btwn: the output betweenness for each vertex
    
    

    ### INPUT *.obj
    File in row-based format like csv is interpreted as a series of data in Object struct. We use file in Object struct format as test input.
    Object struct a 88-bits data structure, which has four fields to describe data properties: type&flag, valid, fieldID, and data, defined as following:

    - Object.range(87, 84): type&flag,
        - 0000: boolean
        - 0001: int64
        - 0010: float
        - 0011: double
        - 0100: date
        - 0101: string
        - 1101: end of line
        - 1111: end of file
    - Object.range(83, 80): valid, the number of valid bytes in data field from LSB
    - Object.range(79, 64): fieldID, the col field index, start from 0
    - Object.range(63, 0): data, stores data

    Take a single line csv input as example: "1", "5.0", "example string", with schema defined as int64, double, string. The interpreted objects in series: obj1, obj2, obj3, obj4, obj5, obj5:
    ```
    // col 0
    obj1.type = 1
    obj1.valid = 8
    obj1.fieldid = 0
    obj1.data = 1

    // col 1
    obj2.type = 3
    obj2.valid = 8
    obj2.fieldid = 1
    obj2.data = 5.0

    // col 2
    obj3.type = 5
    obj3.valid = 8
    obj3.fieldid = 2
    obj3.data = "example "

    obj4.type = 5
    obj4.valid = 6
    obj4.fieldid = 2
    obj4.data = "string"

    // end of line
    obj5.type = 13
    obj5.valid = 0
    obj5.fieldid = 0
    obj5.data = 0

    // end of file
    obj6.type = 15
    obj6.valid = 0
    obj6.fieldid = 0
    obj6.data = 0
    
    ```

    ### OUTPUT *.arrow

    Output format is in arrowIPC which is defined in Apache Arrow project. Please refer to [link]       (https://github.com/apache/arrow/blob/master/docs/source/format/Columnar.rst#serialization-and-interprocess-communication-ipc).

    Submodule references:
    - [Arrow](https://arrow.apache.org/docs/format/Columnar.html#format-columnar)
    - [FlatBuffers](https://google.github.io/flatbuffers/)

    ### TOOL
    - Dump binary file: `hexdump -C -V binary_file`
  
3. 提交程序
  
    您仅需提交cpp文件和h文件。
    请将除test.cpp以外的所有cpp和h文件
    压缩为一个压缩包betweenness.zip，
    在页面下方您的解答处点击“**上传代码**”按钮进行提交，
    稍待片刻即可在文本框下方得知您的设计是否通过了基本功能测试。
    延迟、面积等性能指标则可在历史提交记录中点击版本号详细查看。
  
4. 评分规则
  
    将所有的点按照选手计算的betweenness的值从大到小排序
  
    将所有的点按照参考代码计算的betweenness的值从小到大排序
  
    比较上述两个序列从第几个值开始不一样，这个值记为cnt
  
    同时记录程序运行时间 time
    
    小测试用例占40%分数，大测试用例占60%分数，最终得分按小测试用例得分与大测试用例得分加权相加而得
  
    最终得分 mark=[0.6*(cntbig^4)+0.4*(cntsmall^4)]/time，mark越大对应排名得分越高，若程序运行时间超过1个小时则不计分
