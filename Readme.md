# Personal Hyperscan Utils

For exprimented man ONLY.

x86_64 RHEL7 GCC4.8.5 VSCode CMake Hyperscan5.0.0

2020总结。

可调试。

除了对基本使用的接口进行了封装外，

还可以实现用于分离序列化与反序列化代码的分离，实现加密DB功能。

用户需要手动提供 `ParseCFGFile` 函数。接口可参考`/demo/worker.h`。

生成具体DB可在`app/`下进行，同时修改`CMakeLists.txt`即可。

`app/`下有业务相关代码，无法公开
