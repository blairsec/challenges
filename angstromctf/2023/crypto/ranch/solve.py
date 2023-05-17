import string

f = open("flag.txt").read()

encrypted = "rtkw{cf0bj_czbv_nv'cc_y4mv_kf_kip_re0kyvi_uivjj1ex_5vw89s3r44901831}"

for shift in range(26):
    s = ""
    for i in encrypted:
        if i in string.ascii_lowercase:
            s += chr(((ord(i) - 97 + shift) % 26)+97)
        else:
            s += i

    print(s)
