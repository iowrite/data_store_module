## test guide

add a file simulate falsh
```
dd if=/dev/zero bs=1M count=64 | tr '\000' '\377' > flash.img
```