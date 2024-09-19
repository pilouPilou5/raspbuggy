#!/bin/bash
#this file is useless
echo "this file is useless" 
cd NeoSPI
BUILDDIR="NeoSPI_obj"

# Exécution de la commande meson setup
echo "Setting up Meson build in directory: $BUILDDIR"
meson setup "$BUILDDIR"
if [ $? -ne 0 ]; then
  echo "Meson setup failed!"
  exit 1
fi

# Exécution de la commande ninja
echo "Building project using Ninja in directory: $BUILDDIR"
ninja -C "$BUILDDIR"
if [ $? -ne 0 ]; then
  echo "Ninja build failed!"
  exit 1
fi

echo "Build successful!"
#echo " start of main" 
#"$BUILDDIR"/example/standarttest2 


