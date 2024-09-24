#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
echo $SCRIPT_DIR
install_common_package(){
    yes | sudo apt update
    yes | sudo apt upgrade
}

# Function to install the first part (LED)
install_LED() {
    install_common_package

    echo "Installation of LED"
    yes | sudo apt install meson
    yes | sudo apt install ninja-build
    yes | sudo apt install libyaml-cpp-dev
    yes | sudo apt install libsndfile-dev

    #build of lib
    cd $SCRIPT_DIR/LED/NeoSPI

    BUILDDIR="NeoSPI_obj"
    meson setup "$BUILDDIR"
    ninja -C "$BUILDDIR"
    echo "Build lib LED successful!"
    cd $SCRIPT_DIR

    echo "$SCRIPT_DIR/LED/NeoSPI/NeoSPI_obj/subprojects/yacppl/src" | sudo tee /etc/ld.so.conf.d/raspbuggy.conf
    sudo ldconfig

    if ! grep -q "^dtoverlay=spi0-2cs" /boot/firmware/config.txt; then
        sudo echo "dtoverlay=spi0-2cs" >> /boot/firmware/config.txt
    fi

    source ~/.bashrc
    echo -e "\e[32mLib Led sucesfully installed\e[0m"
}

# Function to install the second part (sound)
install_sound() {
    install_common_package
    echo "Installation of sound"
    # Install gpiod library
    yes | sudo apt install libgpiod-dev

    echo "$SCRIPT_DIR/sound/play_sound/src/lib" | sudo tee -a /etc/ld.so.conf.d/raspbuggy.conf
    sudo ldconfig
    echo -e "\e[32mLib sound sucesfully installed\e[0m"
}

# Function to install everything
install_all() {
    echo "Complete installation..."
    install_common_package
    install_LED
    install_sound
    # Install gstreamer and camera drivers
    yes | sudo apt install libx264-dev libjpeg-dev
    yes | sudo apt install libgstreamer1.0-dev
    yes | sudo apt install libgstreamer-plugins-base1.0-dev
    yes | sudo apt install libgstreamer-plugins-bad1.0-dev
    yes | sudo apt install gstreamer1.0-plugins-ugly
    yes | sudo apt install gstreamer1.0-tools
    yes | sudo apt install gstreamer1.0-gl
    yes | sudo apt install gstreamer1.0-gtk3
    yes | sudo apt install libcamera-dev
    yes | sudo apt install gstreamer1.0-libcamera
}

# Function to display help
display_help() {
    echo "Usage: $0 [-a] [-l] [-s] [-h]"
    echo "Options:"
    echo "  -a    install all"
    echo "  -l    only install LED"
    echo "  -s    only install sound"
    echo "  -h    help"
}

# Read options
while getopts "alsh" opt; do
    case $opt in
        a)
            install_all
            ;;
        l)
            install_LED
            ;;
        s)
            install_sound
            ;;
        h)
            display_help
            exit 0
            ;;
        *)
            display_help
            exit 1
            ;;
    esac
done

# If no option is passed, display help
if [ $OPTIND -eq 1 ]; then
    display_help
fi
