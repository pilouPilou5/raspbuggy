#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
echo $SCRIPT_DIR

# Fonction pour installer la première partie
install_LED() {
    echo "Installation of LED"
    #update
    yes | sudo apt update
    yes | sudo apt upgrade

    yes | sudo apt install meson
    yes | sudo apt install ninja-build
    yes | sudo apt install libyaml-cpp-dev
    yes | sudo apt install libsndfile-dev


    cd $SCRIPT_DIR/LED/NeoSPI

    BUILDDIR="NeoSPI_obj"
    meson setup "$BUILDDIR"
    ninja -C "$BUILDDIR"
    echo "Build lib LED successful!"
    cd $SCRIPT_DIR

    #setup lib 
    export LD_LIBRARY_PATH=$SCRIPT_DIR/LED/NeoSPI/NeoSPI_obj/subprojects/yacppl/src
    export LD_LIBRARY_PATH=$SCRIPT_DIR/sound/play_sound/src/lib

    #activate spi peripherals
    echo "dtparam=spi=on" >> /boot/firmware/config.txt
    echo "dtoverlay=spi1-1cs" >> /boot/firmware/config.txt
    echo "dtoverlay=spi0-1cs" >> /boot/firmware/config.txt
}

# Fonction pour installer la deuxième partie
install_sound() {
    echo "Installation of sound"
    #update
    yes | sudo apt update
    yes | sudo apt upgrade
    #install gpiod library
    yes | sudo apt install libgpiod-dev
}


# Fonction pour tout installer
install_all() {
    echo "Installation complète..."
    install_LED
    install_sound
    #install gstream and camera drivers
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

# Fonction d'affichage de l'aide
afficher_aide() {
    echo "Utilisation : $0 [-a] [-l] [-s] [-h]"
    echo "Options :"
    echo "  -a    install all"
    echo "  -l    only install LED"
    echo "  -s    only install sound"
    echo "  -h    help"
}

# Lecture des options
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
            afficher_aide
            exit 0
            ;;
        *)
            afficher_aide
            exit 1
            ;;
    esac
done

# Si aucune option n'est passée, afficher l'aide
if [ $OPTIND -eq 1 ]; then
    afficher_aide
fi






