import pandas as pd
import numpy as np
# from sklearn.model_selection import train_test_split

print('Reading data...')
data = pd.read_csv('data/kddcup_10_percent_normalized.csv', header=None)

# for i in range(1, 4):
# 	keys = list(set(data[i]))
# 	print(keys)
	# for j in range(len(data[i])):
	# 	print(i, j)
	# 	data[i][j] = keys.index(data[i][j])

# X and y for our learning (as matrices)
X = data.as_matrix(columns=data.columns[:-1])
y = data[data.columns[-1]].values

print(set(y))

print('Number of samples: {0}'.format(len(X)))

