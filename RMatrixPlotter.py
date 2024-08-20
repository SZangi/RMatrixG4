import pandas
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm

df = pandas.read_csv("RMatrixGen3.csv",delimiter=";",names=["Energy","Photons"])
photons = df["Photons"]
photons.hist(bins=100,range=[400,20000])
photon_energy = photons / 12.3
n_energy = df["Energy"]
Hist, x_edges, y_edges = np.histogram2d(n_energy,photon_energy,bins=100,range=[[0,5000],[100,2000]])
Hist = Hist.transpose()
X,Y = np.meshgrid(x_edges[:-1],y_edges[:-1])

fig,ax = plt.subplots(subplot_kw={"projection":"3d"})
ax.plot_surface(X,Y,Hist, vmin=0,cmap=cm.coolwarm)
ax.set_xlabel("Neutron Energy (keV)")
ax.set_ylabel("Light Output (keVee)")
ax.set_zlabel("Intensity")
ax.set_title("EJ309 Response Matrix")
plt.show()