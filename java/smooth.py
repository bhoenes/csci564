import sys
from java.awt import *
from java.lang import *
from java.util import *
from java.nio import *
from javax.swing import *

from edu.mines.jtk.awt import *
from edu.mines.jtk.io import *
from edu.mines.jtk.mosaic import *
from edu.mines.jtk.util import *
from edu.mines.jtk.util.ArrayMath import *
from operator import sub

from my_dgemm import *

#############################################################################

def main(args):
  #goSmooth1()
  goSmooth2()
  #goSmooth2S()
  #goSmooth2T()
  #goDetrend()
  #goBenchmark()

def goSmooth1():
  a = 0.9
  x = readImage()
  #x = testImage1()
  #x = testImage2()
  y1 = smooth1(a,x)
  plot(x,"input")
  plot(y1,"smooth1")

def goSmooth2():
  a = 0.9
  #x = readImage()
  #x = testImage1()
  #x = testImage2()
  x=testImage3()
  x = smooth2(a,x)
  plot(x,"input")
  plot(y,"smooth2")

def goSmooth2S():
  a = 0.9
  #x = readImage()
  #x = testImage1()
  x = testImage2()
  y = smooth2S(a,x)
  plot(x,"input")
  plot(y,"smooth2S")

def goSmooth2T():
  a = 0.9
  x = readImage()
  #x = testImage1()
  #x = testImage2()
  y = smooth2T(a,x)
  plot(x,"input")
  plot(y,"smooth2T")

def goDetrend():
	a = 0.9
	x = readImage()
	y = smooth1(a,x)
	y = smooth2(a,y)
	y = csub(x,y)
	plot(x, "input")
	plot(y, "detrend")

def goBenchmark():
	maxtime = 5.0
	a = 0.9
	x = readImage()
	pixels = len(x[0]) * len(x)
	sw = Stopwatch()
	print "Jython-Java"
	print "Benchmark smoothing of a 2D image"		
	for func in (smooth1, smooth2, smooth2S, smooth2T):
		nsmooth = 0
		sw.restart()
		while sw.time()<maxtime:
			y = func(a,x)
			nsmooth += 1
		sw.stop()
		print func.__name__ + ": mflops " + ("%0.2f" % (6.0e-6*pixels*nsmooth/sw.time()))
	
#############################################################################

def like(x):
  return zerofloat(len(x[0]),len(x))

def smooth(a,x):
  y = like(x)
  Dsp.smooth(a,x,y)
  return y

def smooth1(a,x):
  y = like(x)
  Dsp.smooth1(a,x,y)
  return y

def smooth2(a,x):
  y = like(x)
  Dsp.smooth2(a,x,y)
  return y
  #x = readImage()
def smooth2S(a,x):
  y = like(x)
  Dsp.smooth2S(a,x,y)
  return y

def smooth2T(a,x):
  y = like(x)
  Dsp.smooth2T(a,x,y)
  return y

def testImage1():
  n1,n2 = 51,41
  x = zerofloat(n1,n2)
  x[n2/2][n1/2] = 1.0
  return x

def testImage2():
	n1,n2 = 51,41
	x = zerofloat(n1,n2)
	x[0   ][0   ] = 1.0
	x[0   ][n1-1] = 1.0
	x[n2-1][0   ] = 1.0
	x[n2-1][n1-1] = 1.0
	return x

def testImage3():
	n1,n2 = 51,41
	x = zerofloat(n1,n2)
	for i1 in range(n1):
		x[n2-1][i1] = 1.0
	return x

def readImage():
  n1,n2 = 750,750
  x = zerofloat(n1,n2)
  ais = ArrayInputStream("brian.gray",ByteOrder.LITTLE_ENDIAN)
  ais.readFloats(x)
  ais.close()
  return x

def plot(x,title):
  sp = SimplePlot.asPixels(x)
  sp.setTitle(title)
  sp.addColorBar()
  sp.setSize(900,800)
  sp.plotPanel.setColorBarWidthMinimum(100)

#############################################################################
# Do everything on Swing thread.

class RunMain(Runnable):
  def run(self):
    main(sys.argv)
SwingUtilities.invokeLater(RunMain())

