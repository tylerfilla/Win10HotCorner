#
# Win10HotCorner
#
# This project is released to the public domain per the Unlicense. See the
# accompanying LICENSE file for details.
#

cmake_minimum_required(VERSION 3.4)
project(Win10HotCorner)

add_executable(hotcorner main.c)
set_target_properties(hotcorner PROPERTIES C_STANDARD 99)
