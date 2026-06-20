import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("E:\\Users\\rohaa\\Downloads\\LOG.CSV", header=None);
plt.plot(df[0], df[7]);
plt.xlabel("Timestamp")
plt.ylabel("Roll (deg)")
plt.show();