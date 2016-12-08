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

