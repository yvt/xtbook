#!/bin/bash

if [ "$1" == "" ]; then
echo "USAGE: distsource.sh DESTPATH"
exit 1
fi

DestPath="$1"

function doMakemake {

pushd "$1"

bash ../doc/makemake.sh ..

popd

}

mkdir -p "$DestPath"

cp -R "assets" "$DestPath/"
cp -R "data" "$DestPath/"
cp -R "doc" "$DestPath/"
cp -R "Help" "$DestPath/"
cp -R "InternalDictionaries" "$DestPath/"
cp -R "Languages" "$DestPath/"
cp -R "OptimizedBranch" "$DestPath/"
cp -R "OptimizedBranchOutput" "$DestPath/"
cp -R "RichgelJpeg" "$DestPath/"
cp -R "Strings" "$DestPath/"
cp -R "tcw" "$DestPath/"
cp -R "TinyXML" "$DestPath/"
cp -R "tools" "$DestPath/"
cp -R "TPList" "$DestPath/"
cp -R "TWiki" "$DestPath/"
cp -R "xpms" "$DestPath/"
cp -R "XZEmbedded" "$DestPath/"
mkdir -p "$DestPath/Dictionaries"
mkdir -p "$DestPath/StartupItems"

cp -R "Dictionaries/Sample.xtbdict" "$DestPath/Dictionaries/"
cp -R "English.lproj" "$DestPath/"
cp -R "Utils" "$DestPath/"
cp * "$DestPath/"

mkdir -p "$DestPath/build.cegcc"
mkdir -p "$DestPath/build.linux"
mkdir -p "$DestPath/build.mingw32"

cp "build.cegcc/Makefile" "$DestPath/build.cegcc/"
cp "build.linux/Makefile" "$DestPath/build.linux/"
cp "build.mingw32/Makefile" "$DestPath/build.mingw32/"

doMakemake "$DestPath/build.cegcc"
doMakemake "$DestPath/build.linux"
doMakemake "$DestPath/build.mingw32"




