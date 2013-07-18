VK-GTK 0.3
======

Linux desktop notifications for vk.com.

## Download compiled packages
* [Ubuntu (natty) i386](https://github.com/korkov/vk-gtk/raw/master/packages/ubuntu/natty/vk-gtk_0.3-1_i386.deb)
* [Ubuntu (natty) amd64](https://github.com/korkov/vk-gtk/raw/master/packages/ubuntu/natty/vk-gtk_0.3-1_amd64.deb)

## Compilation

#### Building vk-gtk from source requires that several dependencies are installed.
These include:
 * cmake
 * g++
 * libc6-dev 
 * libgtk2.0-dev
 * libglade2-dev
 * libnotify-dev
 * libboost-signals-dev
 * libcurl4-gnutls-dev
 * libwebkitgtk-dev or libwebkit1.0-dev

#### Run the command:
       cmake ./ && make

#### Installation:
       make install

--
Alexander Korkov
e-mail: korkov@yandex.ru
