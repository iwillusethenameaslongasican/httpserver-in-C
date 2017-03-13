#! /bin/bash

DATE=`date`
echo "Date is ${DATE}"

USER=`who | wc -l`
echo "Logged in user are ${USER}"

UP=`date ; uptime`
echo "Uptime is ${UP}"
