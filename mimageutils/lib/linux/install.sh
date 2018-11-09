#!/bin/bash
# here is desption.
# author : makebing
# version : 1.0


files=`ls *.tar.gz`
tools_dir="imagetools-1.2"

echo "** step 1 depress **"

for f in ${files}; do
	tar -xzvf "$f"
done


echo "** step 2 install dependency packages **"

CUR_DIR=`pwd`
dirs="zlib-1.2.8 libpng-1.6.25 jpeg-9b libwebp-0.4.2"
for f in ${dirs}; do
	echo "install $f"
	cd "$f"
	./configure && make && make install
	cd "$CUR_DIR"
done


echo "** step 3 copy so to /lib64  **"

cp -f /usr/local/lib/libpng* /lib64/
cp -f /usr/local/lib/libjpeg.* /lib64/
cp -f /usr/local/lib/libwebp.* /lib64/
cp -f /usr/local/lib/libz.* /lib64/

echo "** step 4 install $tools_dir **"

cd $tools_dir

make && make install


echo "** step 5 test **"

make test