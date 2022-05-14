#!/usr/local/bin/python3 -u

import pyaes
import math
import secrets
import sys

def bytes_to_long(x):
    return int.from_bytes(x, "big")

def long_to_bytes(x):
    return x.to_bytes(math.ceil(x.bit_length() / 8), "big")

def get_random_bytes(n):
    return secrets.token_bytes(n)

def pad(data, bs):
    p = bs - len(data) % bs
    return data + bytes([p]) * p

class AES:
    MODE_CBC = None

    def __init__(self, k, iv):
        self.cipher =  pyaes.AESModeOfOperationCBC(k, iv = iv)

    def encrypt(self, pt):
        return b"".join(self.cipher.encrypt(pt[i:i + 16]) for i in range(0, len(pt), 16))

    @staticmethod
    def new(k, _mode, iv):
        return AES(k, iv)

flag = b"actf{the_letters_in_rsa_and_aes_form_aries_if_you_throw_in_the_letter_i_because_that_represents_yourself_or_something_anyway_aries_is_a_zodiac_sign_which_means_that_the_two_cryptosystems_are_mutually_compatble_i_think??}"
assert len(flag) < 256

n = 0xbb7bbd6bb62e0cbbc776f9ceb974eca6f3d30295d31caf456d9bec9b98822de3cb941d3a40a0fba531212f338e7677eb2e3ac05ff28629f248d0bc9f98950ce7e5e637c9764bb7f0b53c2532f3ce47ecbe1205172f8644f28f039cae6f127ccf1137ac88d77605782abe4560ae3473d9fb93886625a6caa7f3a5180836f460c98bbc60df911637fa3f52556fa12a376e3f5f87b5956b705e4e42a30ca38c79e7cd94c9b53a7b4344f2e9de06057da350f3cd9bd84f9af28e137e5190cbe90f046f74ce22f4cd747a1cc9812a1e057b97de39f664ab045700c40c9ce16cf1742d992c99e3537663ede6673f53fbb2f3c28679fb747ab9db9753e692ed353e3551
e = 0x10001
d = 0x9df765a382ad8c370f9a5462f4e9ea97c4eaeecbb443fffddc9bbb2162bf94d0d16febac7055f391bd327ba5d20c498013c496e080e4c0250f6b28f2c3ca1a44adf56addc95985134070fbdad187e61c380dd0d2374dc77c2439e3e3295d076224dca6f7e9aadc72e278ccd5e811f3e71fccafe97921eeb0d25e314082a527825874579dddf87ccbc888a63e8c85a4e96a71a79253d4c51a1600fc71309437100cd93d5dcfff4f6eb3f7634a234140f0a637dc78f035e7a14fec9aaeb3d389a2be70316d7472c76414516a106fe1e88c60c7dda14ae9846d571c202e44f636b96a64e5bb15cb5287466df8a90e3d7ce3cfc904b1bdca4307080a1b579396fbdd
assert pow(2,e*d,n)==2

enc = pow(bytes_to_long(flag),e,n)
print(enc)

k = get_random_bytes(32)
iv = get_random_bytes(16)
cipher = AES.new(k, AES.MODE_CBC, iv)

while 1:
    try:
        i = int(input("Enter message to sign: "))
        assert(0 < i < n)
        print("signed message (encrypted with military-grade aes-256-cbc encryption):")
        print(cipher.encrypt(pad(long_to_bytes(pow(i,d,n)),16)))
    except:
        print("bad input, exiting")
        sys.exit(1)
