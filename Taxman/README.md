# Taxman
Simple tax calculator, with a bit of customisation.
How to use:
1) Create a file called "brackets.txt" and fill in your country tax brackets , one bracket on each line:
bracket_low bracket_high percentage_tax

there can be any amount of spaces between the items.
You can also add an exoneration amount if applicable (fixed amount not taken into your taxable amount) like this:
exonerated amount

on any line, last line will override previous exonerated lines.

2) Alternatively launch the script, it will prompt you to input brackets by hand, input each bracket,
with items separated by spaces, enter an empty line to finish input and enter exoneration if applicable.

3) Enter gross amount when prompted, net amount will be given back.