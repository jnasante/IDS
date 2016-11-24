import pandas as pd
import numpy as np
import cPickle as pickle
# from sklearn.model_selection import train_test_split

# Helper Methods
def save_object(obj, filename):
    with open(filename, 'wb') as output:
        pickle.dump(obj, output, pickle.HIGHEST_PROTOCOL)

def get_object(filename):
	with open(filename, 'rb') as obj:
	    return pickle.load(obj)

# Basic funcitonality
print('Reading data...')
data = pd.read_csv('data/kddcup_10_percent_normalized.csv', header=None)

# Disgusting, hardcoded
classification_id = {
	'normal' : 0,
	'pod' : 1,
	'neptune' : 2,
	'guess_passwd' : 3,
	'loadmodule' : 4,
	'teardrop' : 5,
	'buffer_overflow' : 6,
	'perl' : 7,
	'smurf' : 8
}

classification = reverse_map = dict(reversed(item) for item in classification_id.items())

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

