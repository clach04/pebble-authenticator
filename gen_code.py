#!/usr/bin/env python
# Py2 or Py3

import sys


try:
    number_of_slots = int(sys.argv[1])
except:  # yep anything goes
    number_of_slots = 4  # NOTE 5 is too large with 32-byte secret support, larger than Pebble write persist which is capped at 256


# f-strings does not appear to support escaping {}
# can't be bothered to use template


# NOTE defaults to empty - no defaults wil lbe generated for config
# Clay Config - config.js
template = ''',
            {
                "type": "input",
                "label": "Name 0{num}",
                "description": "Name limited to 16 bytes",
                "messageKey": "S0{index_num}_NAME",
                "defaultValue": "",
                "attributes": {
                    "limit": 16
                }
            },
            {
                "type": "input",
                "label": "Secret 0{num}",
                "description": "Secret limited to 32-bytes (256-bit), base32 52 bytes. NOTE do NOT include padding '\\"'='",
                "messageKey": "S0{index_num}_SECRET",
                "defaultValue": "",
                "attributes": {
                    "limit": 52
                }
            }'''


# messageKeys entries - package.js
build_template = ''',
"S0{index_num}_NAME",
"S0{index_num}_SECRET"'''

# do load settings - settings.inc
configload_template = '''
    DO_SETTINGS_NAME({index_num})
    DO_SETTINGS_SECRET({index_num})'''

output = []
for num in range(1, number_of_slots + 1):
	index_num = str(num - 1)
	num = str(num)
	t = template.replace('{index_num}', index_num).replace('{num}', num)
	output.append(t)
print("")
print("// config.js (extract)")
print("".join(output))
print("")
print('-' * 65)


output = []
for num in range(1, number_of_slots + 1):
	index_num = str(num - 1)
	num = str(num)

	t = build_template.replace('{index_num}', index_num).replace('{num}', num)
	output.append(t)
print("")
print("// package.json (extract) `messageKeys`")
print("".join(output))
print("")
print('-' * 65)
print("")



print('#define NUM_SECRETS %d' % number_of_slots)
output = []
for num in range(1, number_of_slots + 1):
	index_num = str(num - 1)
	num = str(num)


	t = configload_template.replace('{index_num}', index_num).replace('{num}', num)
	output.append(t)
print("")
print("// settings.inc (extract) SETTINGS_NAME_VALUE_MACRO")
s = "".join(output)
s = s.replace('\n', '\\\n')
print(s)
print("")
print('-' * 65)
print("")
