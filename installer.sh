#! /bin/bash

make

SOURCE=$(pwd)

echo "alias marquertag='$SOURCE/marquertag'"  >> ~/.bashrc
echo "alias supprimertag='$SOURCE/supprimertag'"  >> ~/.bashrc
echo "alias listertag='$SOURCE/listertag'"  >> ~/.bashrc
echo "alias filtrertag='$SOURCE/filtrertag'"  >> ~/.bashrc
echo "alias lientag='$SOURCE/lientag'"  >> ~/.bashrc
echo "alias afficherlien='$SOURCE/afficherlien'"  >> ~/.bashrc

echo "alias cp='cp --preserve=xattr'"  >> ~/.bashrc

#echo "alias ln='$SOURCE/tmp_ln'"  >> ~/.bashrc
#echo "alias cp='$SOURCE/cp'"  >> ~/.bashrc
#echo "alias mv='$SOURCE/mv'"  >> ~/.bashrc

. ~/.bashrc
