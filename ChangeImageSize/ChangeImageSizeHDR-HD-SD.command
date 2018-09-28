#
# 
# 將原始圖片製作為 HDR HD SD
#
#!/bin/sh

ABSPATH=$(cd "$(dirname "$0")"; pwd) 
echo \"$ABSPATH\"

cd "$ABSPATH"/

mkdir SD
mkdir HD
mkdir HDR

#
# 先將圖片分別copy到HDR HD SD資料夾
# 並修改名稱
#
for file in *.png
do
   name=$(basename -s .png $file)
   target1=./HD/"$name".png
   target2=./HDR/"$name".png
   target3=./SD/"$name".png
   cp $file $target1
   cp $file $target2
   cp $file $target3
done



#
# 再copy原圖至small並縮小之
#
#
cd ./SD
for file in *.png
do
   w=`sips -g pixelWidth $file | tail -1 | sed "s/.* //"`
   let tw=w/4
   sips -Z $tw "$file"
done
/Applications/ImageOptim.app/Contents/MacOS/ImageOptim *.png

cd ../HD
for file in *.png
do
	w=`sips -g pixelWidth $file | tail -1 | sed "s/.* //"`
	let tw=w/2
	sips -Z $tw "$file"
done
/Applications/ImageOptim.app/Contents/MacOS/ImageOptim *.png

cd ../HDR
/Applications/ImageOptim.app/Contents/MacOS/ImageOptim *.png


exit 0