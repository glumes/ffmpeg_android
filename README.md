## 老生常谈-FFmpeg 的编译问题轻松搞定
前几天发了一篇 [FFmpeg 调用 Android MediaCodec 进行硬解码](https://mp.weixin.qq.com/s/S8NwQnY4uyQulfZnRF7t_A) 的文章，这里面的技术点不算太难，也还是调用 FFmpeg 的常用接口操作，但重点在于 FFmpeg 的版本选择以及编译选项要开启 MediaCodec 才行。

关于 FFmpeg 的编译，是个老生常谈的话题了，很多初学者都会卡在怎么编译动态库 so 的问题上，这其实也是 Android 开发转音视频的一大拦路虎，一行 FFmpeg 代码都没来得及写呢，就得先折腾好久编译问题。


当然了，编译麻烦肯定是 FFmpeg 的锅。因为它的不断升级，从早期 2.x 版本到现在的 4.x 版本，调用接口发生了变化，编译选项也调整了不少，但网上的各种 Android so 动态库编译文章可没有对应更新哦，有的教程还停留在 2.x 版本上，如果你照着去编译了，这里面肯定会有兼容性问题的。

举几个例子：

* FFmpeg 源码里面的文件要修改吗？

早期的编译版本还要在 FFmpeg 里面修改一些源码才行，最常见的就是下面的代码：

```cpp
SLIBNAME_WITH_MAJOR='$(SLIBNAME).$(LIBMAJOR)'
LIB_INSTALL_EXTRA_CMD='?(RANLIB)"$(LIBDIR)/$(LIBNAME)"'
SLIB_INSTALL_NAME='$(SLIBNAME_WITH_VERSION)'
SLIB_INSTALL_LINKS='$(SLIBNAME_WITH_MAJOR)$(SLIBNAME)'
```

以前需要修改编译后库名字的连接方式，但是在最新版本中是不用了(最新版指的是 ffmpeg 的 release/4.4 分支版本)。

另外，在实际编译中也不需要修改任何 FFmpeg 源码的。

* Android NDK 要用哪个版本？ 

这也是个常见的兼容性问题。

选择 NDK 版本实际上是在选择编译器，早先编译可能用的是 GCC 编译，后来 Google 在 NDK r18b 版本移除了 GCC 编译工具，具体可以参考如下链接：

[NDK 修订历史记录](https://developer.android.com/ndk/downloads/revision_history?hl=zh-Cn)

所以现在最新的动态库编译都是用 **Clang** 进行操作的，为了跟上时代步伐，也就不要用之前的 NDK 版本了，直接上最新的。

---

为了避免大家把精力消耗在 FFmpeg 的编译上，直接就给出一个 Github Repo ，将编译脚本都放在这个仓库上了。

地址如下：

> https://github.com/glumes/ffmpeg_android


![FFmpeg 编译](https://image.glumes.com/blog_image/%E6%88%AA%E5%B1%8F2021-10-24%20%E4%B8%8B%E5%8D%885.22.09.png)

其中 FFmpeg 源码是作为该仓库的一个子模块 Submodules 的形式加载进来的，在下载时要注意一下：

```cpp
git clone --recursive https://github.com/glumes/ffmpeg_android
```

下载后，进入到 build_android.sh 文件中，将 NDK 替换成你自己的路径，最好也用 r20b 版本的，保持一致。

```cpp
#!/bin/bash
 
#你的NDK路径
NDK=/Users/glumes/Downloads/android-ndk-r20b
TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/darwin-x86_64
API=21
```

替换后，给脚本可执行权限就能运行了，编译后的动态库在 ffmpeg_library 文件夹中，目前仅编译了 armeabi-v7a 架构的。

android 文件夹内对应加载 so 的 Android 工程，也是 FFmpeg 调用 Android MediaCodec 的源码。

这个工程目录也是不需要修改 FFmpeg 的，并且关于 FFmpeg 的很多编译选项开关放在了 config-env.sh 目录中，有需要可以在文件内做修改，不影响主的编译脚本运行。


顺便贴一下源码：

这里是具体执行编译的函数，函数用到的一些参数要在外面定义好：

```cpp
function build_android
{
echo "Compiling FFmpeg for $CPU"

./configure \
    --prefix=$PREFIX \
    --libdir=$LIB_DIR \
    --enable-shared \
    --disable-static \
    --enable-jni \
    --disable-doc \
    --disable-symver \
    --disable-programs \
    --target-os=android \
    --arch=$ARCH \
    --cpu=$CPU \
    --cc=$CC \
    --cxx=$CXX \
    --enable-cross-compile \
    --sysroot=$SYSROOT \
    --extra-cflags="-Os -fpic $OPTIMIZE_CFLAGS" \
    --extra-ldflags="$ADDI_LDFLAGS" \
    --disable-asm \
    # 这些编译参数在 config-env.sh 文件中定义了
    $COMMON_FF_CFG_FLAGS
make clean
make -j8 # 这里是定义用几个CPU编译
make install
echo "The Compilation of FFmpeg for $CPU is completed"
}
```

接下来定义好相关参数，就可以执行了：

```cpp
// 编译的 configure 可执行文件在 ffmpeg 源码目录中，要先进入到目录里
cd ffmpeg

// 定义好编译的架构
OUTPUT_FOLDER="armeabi-v7a"
ARCH="arm"
CPU="armv7-a"
TOOL_CPU_NAME=armv7a
TOOL_PREFIX="$TOOLCHAIN/bin/arm-linux-androideabi"
 
CC="$TOOLCHAIN/bin/armv7a-linux-androideabi$API-clang"
CXX="$TOOLCHAIN/bin/armv7a-linux-androideabi$API-clang++"
SYSROOT="$NDK/toolchains/llvm/prebuilt/darwin-x86_64/sysroot"
PREFIX="${PWD}/../ffmpeg_library/android/$OUTPUT_FOLDER"
LIB_DIR="${PWD}/../ffmpeg_library/android/libs/$OUTPUT_FOLDER"
OPTIMIZE_CFLAGS="-march=$CPU"
build_android
```

由于我们的编译脚本不放在 FFmpeg 源码目录中，所以要对路径做一些修改，这样可以绝对地不改动任何 FFmpeg 内容了。

通过上述的参数设定，应该就可以编译出正确的 Android 动态库了，如果你在编译过程中有任何问题，欢迎加我微信 ezglumes 联系我，及时调整保证轻松搞定编译过程。

