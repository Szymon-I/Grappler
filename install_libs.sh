#!/bin/bash
# install libraries for linux
sudo apt-get update -y
sudo apt install -y  build-essential libgl1-mesa-dev libglu1-mesa-dev libxi-dev libXi-devel freeglut3-dev  glew-utils libglm-dev libsoil-dev libglew-dev minicom socat
# install libraries for python
pip install pyserial numpy