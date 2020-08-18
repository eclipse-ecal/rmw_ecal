FROM eu.artifactory.conti.de:7007/artifactory/c_chs_ctzs_docker_l/ecal-bionic-amd64:latest

ARG ros_distro

RUN apt update
RUN	DEBIAN_FRONTEND=noninteractive
RUN apt -f install -y --allow-unauthenticated curl gnupg2 lsb-release

RUN echo "deb https://eu.artifactory.conti.de/artifactory/i_adas_ubuntu_ros_r `lsb_release -cs` main" > /etc/apt/sources.list.d/ros2-latest.list
RUN curl -O -s -S -k -tlsv1 https://eu-test.artifactory.conti.de/artifactory/c_chs_ctzs_generic_l/external/ros/ros.asc
RUN apt-key add ros.asc

RUN apt update

RUN apt install -y apt-transport-https
RUN apt install -y ros-${ros_distro}-ros-base
RUN apt install -y python3-colcon-common-extensions

RUN apt -y upgrade

RUN curl -s -S -k -tlsv1 https://github.conti.de/storage/releases/13433/files/10987 -o ecal.deb
RUN dpkg -i ecal.deb