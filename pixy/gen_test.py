import numpy as np
import json

num_objects = 5

object_gen = lambda: {
    'sync': hex(0xAA55),
    'checksum': hex(0xFFFF),
    'id': hex(np.random.randint(2, dtype=np.uint16)),
    'x': hex(np.random.randint(0xFFFF, dtype=np.uint16)),
    'y': hex(np.random.randint(0xFFFF, dtype=np.uint16)),
    'width': hex(np.random.randint(0xFFFF, dtype=np.uint16)),
    'height': hex(np.random.randint(0xFFFF, dtype=np.uint16))
}

split_hex = lambda code: '0x{1}, 0x{0}'.format(code[:2], code[-2:])

objects = [object_gen() for i in range(num_objects)]

lines = [', '.join([split_hex(v[2:].zfill(4)) for _, v in ob.items()]) + ',\n' for ob in objects]

with open('test_data.json', 'w') as f:
    json.dump(objects, f, indent=2)

with open('test_data', 'w') as f:
    f.writelines(lines)
