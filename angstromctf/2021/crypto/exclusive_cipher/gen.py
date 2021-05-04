import random

message = b"Congratulations on decrypting the message! The flag is actf{who_needs_aes_when_you_have_xor}. Good luck on the other crypto!"
key = random.randbytes(5)

print(bytes(message[i] ^ key[i % len(key)] for i in range(len(message))).hex())
