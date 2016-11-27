import data_processor

# Initialize
data_processor.initialize()

# Parameters
num_features = len(data_processor.X[0])
num_samples = len(data_processor.X)
cluster_units_number = (50, 50)
epochs = 100
learning_rate = 0.5
neighborhood_number = 7
obj_file_name = 'objects/kohonenSOM.pkl'

############### Tensorflow SOM ###############
def tensorSOM():
	import numpy as np
	from som import SOM
	import tensorflow as tf

	sess = tf.InteractiveSession()
	map_size = 30	

	s = SOM( input_shape=(num_features,), map_size_n=map_size, num_expected_iterations=epochs, session=sess )

	sess.run(tf.initialize_all_variables())

	#Training inputs
	for i in range(10):
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
		for sample in data_processor.featureScale(data_processor.X):
			m.learn(sample)

	# Save to file
	data_processor.save_object(m, obj_file_name)

def getObject():
	return data_processor.get_object(obj_file_name)
