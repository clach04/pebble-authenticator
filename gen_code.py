#!/usr/bin/env python
# Py2 or Py3

number_of_slots = 3


# f-strings does not appear to support escaping {}
# can't be bothered to use template

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
                "description": "Secret limited to 16 bytes. NOTE do NOT include padding '\\"'='",
                "messageKey": "S0{index_num}_SECRET",
                "defaultValue": "",
                "attributes": {
                    "limit": 16
                }
            }
'''


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
print("".join(output))
print("")
print('-' * 65)
print("")
