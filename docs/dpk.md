# dpk 链接
dpk 链接用于记录应用包名称与包格式.

以 gedit 为例, 下面的链接表示的是 deb 包格式的 gedit.
```
dpk://deb/gedit
```

而对应的 flatpak 格式, 需要这样写.
```
dpk://flatpak/gedit
```

dpk 链接分为三部分:
* `dpk` 作为协议名称
* `deb` 或者 `flatpak` 作为包格式名
* `gedit` 是应用名称

## misc
* dpk 链接里的字符都是小写的 ASCII 码.
* 同样一个应用, 可以有多个 dpk 链接.