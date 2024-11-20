import pandas
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm
from mpl_toolkits.mplot3d import axes3d
import matplotlib.ticker as ticker

df = pandas.read_csv("RMatrixes/RMatrixDataSmall.csv",delimiter=";",names=["Energy","Photons"])
photons = df["Photons"]
#photons.hist(bins=100,range=[400,100000])
photon_energy = photons / 12.3

#photon_energy.hist(bins=100, range=[100,10000],log='True')
n_energy = df["Energy"]
#lowest_bin = photons [n_energy < 1500]
Bin1_photons = photon_energy [(n_energy > 100) & (n_energy <= 249)]
Bin2_photons = photon_energy [(n_energy > 0) & (n_energy < 1000)]
Bin_photons = np.array(Bin1_photons)
Bin_photons = np.append(Bin_photons,np.array(Bin2_photons))
Bin_photons = pandas.Series(Bin_photons)
#Bin2_photons.hist(bins=100,range=[100,2500],density=False)
first_bin_eff = 0.84952
num_bins = 100
num_particles = 10000000*2
parts_per_bin = int(num_particles/num_bins)
#print(parts_per_bin)
Hist, x_edges, y_edges = np.histogram2d(n_energy,photon_energy,bins=100,range=[[1500,15000],[273,9050]])
Hist = Hist.transpose()
#Hist[Hist <100] = 0
Hist = Hist/parts_per_bin

X,Y = np.meshgrid(x_edges[:-1],y_edges[:-1])

font = {'size':12}
plt.rc('font',**font)
fig,ax = plt.subplots(subplot_kw={"projection":"3d"})
fig.set_size_inches(12,10)

def log_tick_formatter(val,pos=None):
    return f"$10^{{{val:g}}}$"

Plot_Hist = np.copy(Hist)
Plot_Hist[Plot_Hist == 0] = min(Hist[Hist != 0])

ax.plot_surface(X,Y,np.log10(Plot_Hist), cmap = 'coolwarm')
ax.zaxis.set_major_formatter(ticker.FuncFormatter(log_tick_formatter))
ax.zaxis.set_major_locator(ticker.MaxNLocator(integer=True))
ax.set_xlabel("Neutron Energy (keV)",fontsize="16")
ax.set_ylabel("Light Output (keVee)",fontsize="16")
ax.set_zlabel("Intensity",fontsize="16")
ax.set_title("EJ309 Response Matrix",fontsize="18")
ax.view_init(elev=30., azim=60.)


if (min([min(np.sum(Hist,axis=0)),min(np.sum(Hist,axis=1))]) > 0):
    print("No zero energy values, RMatrix written to file")
    np.savetxt("EnergyAxis.csv",x_edges[:-1],"%.0f")
    np.savetxt("EJ301_400keV_15MeV_RMatrix.csv",Hist,"%.4e")
elif min(np.sum(Hist,axis=1)) == 0:
    print("RMatrix bad, zero energy along axis 1")
    if min(np.sum(Hist,axis=0))==0:
        print("RMatrix bad, zero energy along axis 0")
elif min(np.sum(Hist,axis=0))==0:
    print("RMatrix bad, zero energy along axis 0")

plt.show()

