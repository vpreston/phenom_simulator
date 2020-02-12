import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

target_file = '../logs/test.txt'

if __name__ == '__main__':
    df = pd.read_csv(target_file, header=None)
    df.columns = ['x', 'y', 'z', 't']

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot(df['x'], df['y'], df['z'])
    plt.show()
