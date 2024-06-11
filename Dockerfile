FROM nvidia/opengl:1.2-glvnd-runtime-ubuntu22.04

ARG RUNWORKSPACE=ros2ws
WORKDIR /root/${RUNWORKSPACE}
RUN apt update && apt install locales
RUN locale-gen en_US en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8 &&  export LANG=en_US.UTF-8
RUN apt-get install software-properties-common -y
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN add-apt-repository universe
RUN apt update && apt install curl -y &&curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg &&echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | tee /etc/apt/sources.list.d/ros2.list > /dev/null
RUN apt update && apt upgrade -y
RUN apt install ros-humble-desktop -y
ENV QT_X11_NO_MITSHM=1
ENV EDITOR=nano
ENV XDG_RUNTIME_DIR=/tmp
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    make \
    curl \
    git \
    gazebo \
    ros-humble-gazebo-ros-pkgs\
    ros-humble-xacro\
    libeigen3-dev \
    libopencv-dev
RUN apt install -y python-is-python3
RUN apt install -y  python3-rosdep
RUN apt install -y python3-colcon-common-extensions

RUN rosdep init && rosdep update && \
    echo "source /opt/ros/humble/setup.bash"  >> ~/.bashrc
# installing casadi
RUN apt-get install -y gfortran liblapack-dev pkg-config --install-recommends
RUN apt install coinor-libipopt-dev -y
RUN apt-get install -y swig
RUN cd /tmp && git clone https://github.com/casadi/casadi.git && mkdir casadi/build && cd casadi/build && cmake -DWITH_PYTHON=ON -DWITH_IPOPT=ON -DWITH_OPENMP=ON -DWITH_THREAD=ON .. && make && make install
RUN cd /root && git clone https://github.com/leonhartyao/gazebo_models_worlds_collection.git

RUN echo "export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:/root/gazebo_models_worlds_collection/models"  >> ~/.bashrc
RUN echo "export GAZEBO_RESOURCE_PATH=$GAZEBO_RESOURCE_PATH:/root/gazebo_models_worlds_collection/worlds"  >> ~/.bashrc
RUN echo "source /usr/share/gazebo/setup.bash" >> ~/.bashrc




