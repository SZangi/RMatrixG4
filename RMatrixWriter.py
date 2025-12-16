import pandas
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm
from mpl_toolkits.mplot3d import axes3d
import matplotlib.ticker as ticker
from matplotlib import font_manager

plt.rcParams['font.family'] = 'Liberation Serif'
plt.rcParams['font.size'] = 14

df = pandas.read_csv("RMatrixes/RMatrixDataSmall.csv",delimiter=";",names=["Energy","Photons"])
photons = df["Photons"]
#photons.hist(bins=100,range=[400,100000])
photon_energy = photons / 12.3

#photon_energy.hist(bins=100, range=[100,10000],log='True')
n_energy = df["Energy"]
## Plotting total photons out
#lowest_bin = photons [n_energy < 1500]
Bin1_photons = photon_energy [(n_energy > 100) & (n_energy <= 249)]
Bin2_photons = photon_energy [(n_energy > 0) & (n_energy < 1000)]
Bin_photons = np.array(Bin2_photons)
#Bin_photons = np.append(Bin_photons,np.array(Bin2_photons))
Bin_photons = pandas.Series(Bin_photons)
#Bin2_photons.hist(bins=100,range=[100,2500],density=False)

## Response Matrix Plotting
num_bins = 100
num_particles = 10000000*2
parts_per_bin = int(num_particles/num_bins)
#print(parts_per_bin)
min_E = 1000
max_E = 6000
min_light_pro = 110
max_light_pro = 2500
#max_light_pro = (photon_energy [(n_energy < max_E) & (n_energy > (max_E-50))]).max()
#photon_energy [(n_energy < max_E) & (n_energy > (max_E-50))].hist()
#print(max_light_pro)
Hist, x_edges, y_edges = np.histogram2d(n_energy,photon_energy,bins=num_bins,range=[[min_E,max_E],[min_light_pro,max_light_pro]])
Hist = Hist.transpose()
#Hist[Hist <100] = 0
Hist = Hist/parts_per_bin

X,Y = np.meshgrid(x_edges[:-1],y_edges[:-1])


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
#ax.set_title("EJ309 Response Matrix",fontsize="18")
ax.view_init(elev=30., azim=60.)


if (min([min(np.sum(Hist,axis=0)),min(np.sum(Hist,axis=1))]) > 0):
    print("No zero energy values, RMatrix written to file")
    np.savetxt("EnergyAxis2.csv",x_edges[:-1],"%.0f")
    np.savetxt("EJ309_110keV_6MeV_RMatrix.csv",Hist,"%.4e")
elif min(np.sum(Hist,axis=1)) == 0:
    print("RMatrix bad, zero energy along axis 1")
    if min(np.sum(Hist,axis=0))==0:
        print("RMatrix bad, zero energy along axis 0")
elif min(np.sum(Hist,axis=0))==0:
    print("RMatrix bad, zero energy along axis 0")

fig.set_layout_engine('tight')
fig.savefig("../../pics/EJ309_RMatrix.pdf",format="pdf")

plt.show()