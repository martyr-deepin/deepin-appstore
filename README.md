

# Build from Source

First, get submodules required by this repo:
```shell
git submodule update --init --recursive
git submodule update --remote
```

Then, install required deb packages:
```shell
sudo apt install cmake libdtkwidget-dev libqcef-dev libqt5webchannel5-dev \
libsass0 qttools5-dev-tools libqt5sql5-sqlite qttools5-dev
```
