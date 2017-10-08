#!/bin/sh

export PATH=$(echo $PATH | sed -r 's|:*(/cygdrive)?/c/Windows(/[^:]*)?:*|:|Ig')

scriptdir=$(dirname "$0")
src=$(find "$scriptdir" -name '*.png' | sort)
convert -quality 10 -delay 15 -loop 0 $src "$scriptdir/fra-airtraffic-480.gif"
