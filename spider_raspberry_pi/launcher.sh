#!/bin/bash

source /home/pi/Desktop/venv/bin/activate;
python /home/pi/Desktop/client1.py;

RTMP_URL=rtmp://1.23037641.fme.ustream.tv/ustreamVideo/23037641
STREAM_KEY=AkTC6QRG3wyMQZZV8zVhyS4TpyJQ8jaZ
while :
do
  raspivid -n -vf -hf -t 0 -w 960 -h 540 -fps 15 -b 500000 -o - | ffmpeg -i - -vcodec copy -an -metadata title="Streaming from raspberry pi camera" -$
  sleep 2
done 


