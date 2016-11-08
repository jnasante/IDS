import pandas as pd
import numpy as np
# from sklearn.model_selection import train_test_split

print('Reading data...')
data = pd.read_csv('data/kddcup_10_percent.csv', header=None)

print(set(data[1]))
print(set(data[2]))
print(set(data[3]))

# X and y for our learning (as matrices)
X = data.as_matrix(columns=data.columns[:-1])
y = data[data.columns[-1]].values

print('Number of samples: {0}'.format(len(X)))

