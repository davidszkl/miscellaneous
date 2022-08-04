# Vigenere_decrypter

This program can decrypt files that were encrypted using a variation of the Caesar cypher that uses
keys of multiple characters, letters of the original text are offset differently based on their index:
example:

text=	   "aaaaa"
key=	   "key"
encrypted= "keyke"

It works with probabilities: the letter 'e' in the french language is expected to appear 17.110% of the
time in a long text, we can use this to our advantage and compare what our encrypted text's most common
letter is, calculate the offset and decrypt the code, the problem is it was encrypted using multiple
characters. The solution is to split the encoded text into 'key_length' smaller texts and analyse them
separately.

Since the key is unknown, it's length is also unknown, this program will test all key lengths up to a
nbr given in the parameters and select the most one that has the smallest error margin. The longer the
text the more accurate it will be.

If you want this to work for your language you need to change the macro COMMON_FREQ to be the most common
letter in your alphabet.

to use the decrypter, launch the makefile, then enter the following command:
./decrypt 'encrypted_file' 'max_key_length' 'output_file'

to encrypt your text you can use:
./encrypt 'your_file' 'key'