# HardwareCourseDesign
Hardware course design for HUST EIC, grouped by Liu Kaixin, Mu Difan, Li Bingling, Li Yuanzhe.

> Huazhong University of Science and Technology, </br>
School of Electronic Information and Communications, </br>
Advanced Class 2016.</br>

### 目录介绍
- HardWare 是硬件工程，设计完毕基本不改动。
- SoftWare中为上层软件工程。其中 .sof 文件是硬件工程比特流文件；.sopcinfo 是硬件系统的描述文件，用于生成板级支持包bsp. 
- SoftWare2 是触摸驱动软件工程，其中 .sof 文件是硬件工程比特流文件；.sopcinfo 是硬件系统的描述文件，用于生成板级支持包bsp. 
- SoftWare3 是SD卡读取文件工程，其中 .sof 文件是硬件工程比特流文件；.sopcinfo 是硬件系统的描述文件，用于生成板级支持包bsp. 
- SoftWare4 是屏幕显示驱动工程，其中 .sof 文件是硬件工程比特流文件；.sopcinfo 是硬件系统的描述文件，用于生成板级支持包bsp. 



- “matlab测试图片显示效果”，由黎冰凌实现，用于在软件层面测试SoftWare屏幕显示函数的效果，可直接以图片格式显示。使用时，image.txt和C代码在同一目录下，.m文件里的路径要对应修改

- Hardware中的硬件错误已经修复，而且添加了一个pen_intr低速率采样器，可去抖动。

- ScreenTest1是软件层面的测试程序，测试屏幕信息初始化和屏幕显示函数，不依赖于硬件平台。




- Software中版本介绍：
  ebook1为基本页面切换工程
  ebook2添加GBK的翻页
  ebook3添加UCS2的显示，长文件名的显示，时间响应函数也添加了。
  ebook4使用改进版翻页函数，并对设置页面的操作进行优化。