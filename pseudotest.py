import vybind11

import skimage.data


coin  = skimage.data.coins()
import pylab
pylab.imshow(coin)
pylab.show()

g = vybind11.withCopy(coin)
print(g.shape)

pylab.imshow(g)
pylab.show()