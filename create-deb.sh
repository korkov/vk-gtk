#! /bin/sh
rm -rf vk-gtk
make install DESTDIR=vk-gtk
cp -R DEBIAN vk-gtk/
dpkg-deb --build vk-gtk
mv vk-gtk.deb vk-gtk_0.3-1_amd64.deb
echo vk-gtk_0.3-1_amd64.deb
