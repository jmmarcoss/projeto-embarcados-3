# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/jm/esp/v5.3.1/esp-idf/components/bootloader/subproject"
  "/home/jm/Documentos/www/ifpb/embarcados/build/bootloader"
  "/home/jm/Documentos/www/ifpb/embarcados/build/bootloader-prefix"
  "/home/jm/Documentos/www/ifpb/embarcados/build/bootloader-prefix/tmp"
  "/home/jm/Documentos/www/ifpb/embarcados/build/bootloader-prefix/src/bootloader-stamp"
  "/home/jm/Documentos/www/ifpb/embarcados/build/bootloader-prefix/src"
  "/home/jm/Documentos/www/ifpb/embarcados/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/jm/Documentos/www/ifpb/embarcados/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/jm/Documentos/www/ifpb/embarcados/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
