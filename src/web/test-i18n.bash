#!/bin/bash
for f in $(ls src/locale | grep "messages..*.xlf"); do
  export LANGUAGE=`echo $f | sed "s/messages\.\(.*\)\.xlf/\1/g"`
  echo -e `date` '\t' $LANGUAGE
  yarn build-locale >/dev/null
done