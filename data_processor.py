import pandas as pd
import numpy as np
import cPickle as pickle
from sklearn.model_selection import train_test_split

# Variables
data = None
classification = None
classification_id = None
X = None
y = None
num_samples = None
num_features = None

means = []
devs = []

# Helper Methods
def featureScale(x):
	global means
	global devs
	means = np.mean(x)
	devs = np.std(x)
	return np.array((x - means) / devs)

def invertScale(val):
	return val * devs + means

def save_object(obj, filename):
	with open(filename, 'wb') as output:
		pickle.dump(obj, output, pickle.HIGHEST_PROTOCOL)

def get_object(filename):
	with open(filename, 'rb') as obj:
	    return pickle.load(obj)

def count():
	num_samples = len(y)
	num_malicious = 0
	malicious = []
	for i in range(num_samples):
		if (y[i] != 'normal.'):
			num_malicious += 1
			malicious.append(i)

	num_normal = num_samples - num_malicious
	print('Total: {2}\tMalicious: {0}\tNormal: {1}').format(num_malicious, num_normal, num_samples)

	return malicious

def cross_validation(_X=X, _y=y):
	X_train, X_test, y_train, y_test = train_test_split(_X, _y, test_size=0.2, random_state=42)
	return X_train, X_test, y_train, y_test

def initialize():
	global data
	global classification
	global classification_id
	global X
	global y
	global num_samples
	global num_features
	global X_train
	global X_test
	global y_train
	global y_test

	# Basic funcitonality
	print('Reading data...')
	data = pd.read_csv('data/kddcup_normalized_50K.csv', header=None)

	# Disgusting, hardcoded
	# classification_id = {
	# 	'normal' : 0,
	# 	'pod' : 1,
	# 	'neptune' : 2,
	# 	'guess_passwd' : 3,
	# 	'loadmodule' : 4,
	# 	'teardrop' : 5,
	# 	'buffer_overflow' : 6,
	# 	'perl' : 7,
	# 	'smurf' : 8
	# }

	# classification = reverse_map = dict(reversed(item) for item in classification_id.items())

	# for i in range(1, 4):
	# 	keys = list(set(data[i]))
	# 	print(keys)
		# for j in range(len(data[i])):
		# 	print(i, j)
		# 	data[i][j] = keys.index(data[i][j])

	# X and y for our learning (as matrices)
	X = data.as_matrix(columns=data.columns[:-1])
	y = data[data.columns[-1]].values
	print(list(set(y)))

	num_samples = len(X)
	num_features = len(X[0])

	X_train, X_test, y_train, y_test = cross_validation(featureScale(X), y)

