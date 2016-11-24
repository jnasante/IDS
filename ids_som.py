from data_processor import data, X, y, classification, classification_id
import numpy as np
from matplotlib import pyplot as plt
import cPickle as pickle

# Parameters
num_features = len(X[0])
num_samples = len(X)
cluster_units_number = (50, 50)
epochs = 100
learning_rate = 0.5
neighborhood_number = 7

def save_object(obj, filename):
    with open(filename, 'wb') as output:
        pickle.dump(obj, output, pickle.HIGHEST_PROTOCOL)

def get_object(filename):
	with open(filename, 'rb') as obj:
	    return pickle.load(obj)

############### Tensorflow SOM ###############

def tensorSOM():
	from som import SOM
	import tensorflow as tf

	sess = tf.InteractiveSession()
	map_size = 30	

	s = SOM( input_shape=(num_features,), map_size_n=map_size, num_expected_iterations=epochs, session=sess )

	sess.run(tf.initialize_all_variables())

	#Training inputs
	for i in range(epochs):
		print('Epoch: {0}'.format(i))
		rnd_ind = np.random.randint(0, len(X))
		s.train(X[rnd_ind,:])

	print(np.reshape(s.get_weights(), [map_size, map_size, num_features]))
	print(np.array(y) * s.get_weights())


############### Kohonen SOM ###############

def trainKohonenSOM():
	import kohonen

	params = kohonen.Parameters(
		dimension=num_features,
		shape=cluster_units_number,
		learning_rate=learning_rate,
		neighborhood_size=neighborhood_number)

	m = kohonen.Map(params)
	m.reset() # Need to reset after initialization

	# Time for school!
	for e in range(epochs):
		print('Epoch: {0}'.format(e))
		for sample in X:
			m.learn(sample)

	# Save to file
	save_object(m, 'kohonenSOM.pkl')


def kohonenSOM():
	m = get_object('kohonenSOM.pkl')

	winner = m.flat_to_coords(m.winner(X[744]))

	print(winner)
	print(m.neuron(winner))
	print(X[744])
	print(y[744])

	plt.imshow(m.neuron_heatmap())
	# plt.imshow(m.distance_heatmap(X[0]))
	plt.show()

def count():
	num_malicious = 0
	for i in range(num_samples):
		if (classification_id[y[i][:-1]] != 0):
			print(i)
			break
			num_malicious += 1

	print('Malicious: {0}\tNormal: {1}\tTotal: {2}\tMin Accuracy: {3}').format(num_malicious, num_samples-num_malicious, num_samples, float((num_samples-num_malicious))/num_samples)

# trainKohonenSOM()
kohonenSOM()



