xhost +local:docker
docker run --gpus all -ti --rm -e "DISPLAY" -e "QT_X11_NO_MITSHM=1" -e XAUTHORITY --mount type=bind,source=./src,target=/root/ros2ws/src --mount type=bind,source=./build_packages.bash,target=/root/ros2ws/build_packages.bash --net=host --privileged -m=8g --name test  ros2_pr:latest
