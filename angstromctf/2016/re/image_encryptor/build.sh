#!/usr/bin/env bash 
if [[ $ANDROID_HOME && ${ANDROID_HOME} ]]
then
cd ImageEncryptor
./gradlew assembleDebug
cp encryptor/build/outputs/apk/encryptor-debug.apk ../encryptor.apk
fi
