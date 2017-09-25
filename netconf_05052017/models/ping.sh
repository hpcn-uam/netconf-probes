#!/bin/sh

context=$2
i=0
for a in "$@" ; do
  	if [ $i = 5 ]; then
		host=$a
  	fi
    if [ $i = 3 ]; then
    	count=$a
    fi
    i=`expr $i + 1`
done

if ping -q -c $count $host >/dev/null
then
    result_ping="was successful"
else
    result_ping="failed"
fi
mesg="Ping to host ${host} count ${count} ${result_ping}."

echo "ping-result 'Invoked from $2. $mesg'"
