# xstore
> nodejs进程级缓存，用于存储简单数据，避免全局变量
## 编译
> node-gyp configure build
## 调用方式
```
var xstore = require("./build/Release/xstore");
var r1 = xstore.set("key1", 123);
var r2 = xstore.get("key1");
var r3 = xstore.del("key1");
var r4 = xstore.get("key1");
console.log(r1, r2, r3, r4);
```
