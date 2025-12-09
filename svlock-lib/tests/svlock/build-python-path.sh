#!/bin/sh
root=$1

dirs=`echo $root/python/build/*/svlock`
echo dirs are $dirs

answer=
for d in $dirs
do
	answer=`dirname "$answer:$d"`
	answer="$answer:$d"
done
echo $answer
