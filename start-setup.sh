#!/bin/bash
#关闭tracker 和storage
sudo fdfs_trackerd ./conf/tracker.conf stop
sudo fdfs_storaged ./conf/storage.conf stop

#重新启动 tracker storage
sudo fdfs_trackerd ./conf/tracker.conf 
echo "tracker succ!"
sudo fdfs_storaged ./conf/storage.conf 
echo "storage succ!"

