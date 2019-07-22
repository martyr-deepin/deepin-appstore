#!/bin/bash
for f in $(ls src/locale | grep "messages..*.xlf"); do
  export LANGUAGE=`echo $f | sed "s/messages\.\(.*\)\.xlf/\1/g"`
  yarn build-locale >/dev/null
  echo -e `date` '\t' $LANGUAGE '\t' `[ $? == 0 ] && echo -e '\033[32m Success \033[0m' || echo -e '\033[31m Failure \033[0m'`
done