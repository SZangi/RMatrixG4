import pandas
import matplotlib.pyplot as plt

df = pandas.read_csv("RMatrixTest.csv",delimiter=";",names=["Energy","Photons"])
photons = df["Photons"]
photons.hist(bins=100)
plt.show()