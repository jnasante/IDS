import pandas as pd
import numpy as np
import cPickle as pickle
# from sklearn.model_selection import train_test_split

# Variables
data = None
classification = None
classification_id = None
X = None
y = None
num_samples = None

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
	# print(means, devs)
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
		if (classification_id[y[i][:-1]] != 0):
			num_malicious += 1
			malicious.append(i)

	num_normal = num_samples - num_malicious
	print('Malicious: {0}\tNormal: {1}\tTotal: {2}\tMin Accuracy: {3}').format(num_malicious, num_normal, num_samples, float(num_normal)/num_samples)

	return malicious

def initialize():
	global data
	global classification
	global classification_id
	global X
	global y
	global num_samples

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
	num_samples = len(y)
