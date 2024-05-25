<?php

function genToken($uname, $pin){

    $pin2 = $pin * 3271;
    return md5($uname . $pin . md5("$pin2"));
}

function validate($uname, $pin, $token){

    if (genToken($uname, $pin) == $token)
        return true;
    return false;

}