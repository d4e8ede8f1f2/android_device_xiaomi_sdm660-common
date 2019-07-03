#! /vendor/bin/sh

baseband=`getprop ro.baseband`
if [ "$baseband" = "mdm" ] || [ "$baseband" = "mdm2" ]; then
	start mdm_helper
fi
