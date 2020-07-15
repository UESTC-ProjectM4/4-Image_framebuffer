# 4-Image_framebuffer

>实训作业，4图片分屏

基于ITOP4412精英版

环境 Redhat 6

GCC版本 4.3.2

使用 arm-linux-gcc 进行交叉编译

## 7-15

实现了四张图片在同一LCD显示屏上分屏显示的功能

编译方法如下

~~~
  # arm-linux-gcc -o [编译后的文件名] [需要编译的文件]
  eg：
  # arm-linux-gcc -o test framebuffer_color04.cpp
~~~
