from data_processor import data, X, y, classification, classification_id
import numpy as np
from matplotlib import pyplot as plt

# Parameters
num_features = len(X[0])
cluster_units_number = (30, 30)
epochs = 100
learning_rate = 0.5
neighborhood_number = 7


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

def kohonenSOM():
	import kohonen

	params = kohonen.Parameters(
		dimension=num_features,
		shape=cluster_units_number,
		learning_rate=learning_rate,
		neighborhood_size=neighborhood_number)

	m = kohonen.Map(params)
	m.reset() # Need to reset after initialization

	# Time for school!
	for e in range(1):
		print('Epoch: {0}'.format(e))
		for sample in X:
			m.learn(sample)

	winner = m.flat_to_coords(m.winner(X[0]))

	print(winner)
	print(m.neuron(winner))
	print(X[0])
	print(y[0])

	# plt.imshow(m.neuron_heatmap())
	plt.imshow(m.distance_heatmap(X[0]))
	plt.show()


kohonenSOM()