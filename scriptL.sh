#! /bin/bash

read -s -p "Password: " PASSWORD
echo

exec 3<>/dev/tcp/localhost/30000
echo $USER >&3

read PSALT <&3
read CSALT <&3

HASH=$(openssl passwd -6 -salt $PSALT $PASSWORD)
REQUEST=$(openssl passwd -6 -salt $CSALT $HASH)
echo $REQUEST >&3
read FIRST_RESPONSE <&3
echo $FIRST_RESPONSE

while true
do
	read COMMAND
	echo $COMMAND >&3
	read RESPONSE <&3
	echo $RESPONSE
done
