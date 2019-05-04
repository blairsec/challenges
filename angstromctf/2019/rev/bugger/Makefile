bugger: bugger.c
	gcc bugger.c -o _bugger -s
	upx _bugger -o bugger
	sed -i -e 's/UPX!/\x00\x00\x00\x00/g' bugger
