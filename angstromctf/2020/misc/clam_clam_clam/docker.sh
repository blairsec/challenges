#!/bin/bash

if [ -n $IMAGE_NAME ]
then
	IMAGE_NAME="clam_clam_clam"
fi
if [ -n $CONTAINER_NAME ]
then
	CONTAINER_NAME="clamclamclam"
fi
if [ -n $CONTAINER_PORT ]
then
	CONTAINER_PORT=3000
fi

ARGS=""

function getPort () {
	local PORT=0
	if [ -z $1 ]
	then
		PORT=$CONTAINER_PORT
	else
		PORT=$1
	fi
	echo $PORT
}

function getDir () {
	local DIR=.
	if [ -z $1 ]
	then
		DIR=.
	else
		DIR=$1
	fi
	echo $DIR
}

function stopInstance () {
	docker stop -t 0 $CONTAINER_NAME
	docker rm $CONTAINER_NAME
}

if [ $UID -ne 0 ]
then
	echo "You must be root to run this script."
	exit 1
fi

if [ -z $ADDITIONAL_ARGS ]
then
	ADDITIONAL_ARGS=""
fi

if [ $1 = "build" ]
then
	DIR=$(getDir $2)
	docker build $2 -t $IMAGE_NAME $DIR
elif [ $1 = "run" ]
then
	PORT=$(getPort $2)
	stopInstance
	docker run -p $PORT:$CONTAINER_PORT $ARGS $ADDITONAL_ARGS $3 --name $CONTAINER_NAME -d $IMAGE_NAME
elif [ $1 = "run_bash" ]
then
	PORT=$(getPort $2)
	stopInstance
	docker run -p $PORT:$CONTAINER_PORT -it $ARGS $ADDITIONAL_ARGS $3 --name $CONTAINER_NAME $IMAGE_NAME bash
elif [ $1 = "run_attached" ]
then
	PORT=$(getPort $2)
	stopInstance
	docker run -p $PORT:$CONTAINER_PORT -it $ARGS $ADDITONAL_ARGS $3 --name $CONTAINER_NAME $IMAGE_NAME
elif [ $1 = "bash" ]
then
	docker exec -it $2 $CONTAINER_NAME bash
elif [ $1 = "stop" ]
then
	stopInstance
else
	echo "Please provide a valid option."
fi
