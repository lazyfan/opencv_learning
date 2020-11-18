#!/bin/bash
rm -rf ./build/
mkdir ./build/
cd ./build/
cmake ../
make -j8
#cd ../release_linux
#./deploy ../release_bin/deploy_sample/task_config.xml
