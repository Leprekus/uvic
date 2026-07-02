#!/usr/bin/env bash
rawify() {
  mkdir -p ./tests/raw
  for entry in "./tests/video_samples"/*
  do
    filename=$(basename $entry)
    filename="${filename%%.*}"
    ffmpeg -i "$entry" -f rawvideo -pixel_format yuv420p - > "./tests/raw/$filename.raw"
  done

}

debug() {
  mkdir -p ./out/uvi
  mkdir -p ./out/raw
  filename=$(basename $1)
  filename="${filename%%.*}"
  make
  # store compressed output
  echo "compressing $1"
  ./uvid_compress 720 480 "medium" < $1 > "./out/uvi/$filename.uvi"
  # decompress video
  dpath="./out/uvi/$filename.uvi"
  echo "decompressing $dpath"
  ./uvid_decompress < "$dpath" > "./out/raw/$filename.raw"

  # store final output 
  fpath="./out/$filename.y4m"
  rm -f "$fpath" # delete if exists
  echo "writing $fpath"
  ffmpeg -f rawvideo -pixel_format yuv420p -framerate 30 -video_size 352x288 -i - -f yuv4mpegpipe "$fpath" < "./out/raw/$filename.raw" 

  mpv "$fpath"
}

while getopts 'hrd:' opt;
do
  case "$opt" in
    r) rawify;;
    h) echo "help option still not implemented";;
    d) debug $OPTARG;;
  esac
done






