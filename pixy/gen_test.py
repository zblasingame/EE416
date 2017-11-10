import numpy as np
import json

num_objects = 5
random_garabage = 6

object_gen = lambda: {
    'sync': hex(0xAA55),
    'checksum': hex(0xFFFF),
    'id': hex(np.random.choice([0x13, 0x12, 0x4])),
    'x': hex(np.random.randint(0x0FFF, dtype=np.uint16)),
    'y': hex(np.random.randint(0x0FFF, dtype=np.uint16)),
    'width': hex(np.random.randint(0x0FFF, dtype=np.uint16)),
    'height': hex(np.random.randint(0x0FFF, dtype=np.uint16))
}

split_hex = lambda code: '0x{1}, 0x{0}'.format(code[:2], code[-2:])

objects = [object_gen() for i in range(num_objects)]

# Update checksum
for o in objects:
    o['checksum'] = hex(np.sum([int(v, 16) for k, v in o.items()][2:]))

lines = ['uint8_t bytes[{}] = {{\n'.format(
    num_objects * 14 + (2*random_garabage) + 2
)]
lines += ['\t' + ', '.join(
    [split_hex(hex(v)[2:].zfill(4)) for v in np.random.randint(
        0xFFFF,
        size=random_garabage,
        dtype=np.uint16
    )]
) + ', 0x55, 0xaa,\n']
lines += ['\t' + ', '.join(
    [split_hex(v[2:].zfill(4)) for _, v in ob.items()]
) + ',\n' for ob in objects]
lines += ['};']

with open('test_data.json', 'w') as f:
    json.dump(objects, f, indent=2)

with open('test_data', 'w') as f:
    f.writelines(lines)
