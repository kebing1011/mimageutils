## imgtl
图片处理命令行程序（支持Linux, Windows, MaxOS）


* 缩放图片支持(png jpg)
* 转码图片(png->jpg jpg->png)
* 旋转图片
* 水平反转 & 垂直反转

用法:

```
Usage:
      imgtl [-oqzswhfr] [file ...]
      -o [output] output image file use format with .jpg or .png
      -q [quality] for quality 1~100 eg. 75
      -z [MaxWH] resize aspect to size
      -s [scale] resize aspect to scale
      -w [width] resize to width
      -h [height] resize to height
      -f [flip] h: horizontal flip or v: vertical flip
      -r [rotate] l: left rotate or r: right rotate
```

##imgmk
图片创建命令行程序（支持Linux, Windows, MaxOS）

根据rgba颜色值创建指定大小的图片 png or jpg

用法:

```
Usage:
      imgmk -o 1.jpg -w 100 -h 100 -r ffffff(RGBA)
      -o [output] output image file use format with .jpg or .png
      -w [width] set width
      -h [height] set height
      -r [rgb] rgba or rgb eg. ff00ff00, ff00ff
```
