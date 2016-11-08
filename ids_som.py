from som import SOM
import tensorflow as tf
from matplotlib import pyplot as plot
import numpy as np
from data_processor import data, X, y

sess = tf.InteractiveSession()
epochs = 10
map_size = 30
num_features = len(X[0])

s = SOM( input_shape=(num_features,), map_size_n=map_size, num_expected_iterations=epochs, session=sess )

sess.run(tf.initialize_all_variables())


#Training inputs
for i in range(epochs):
	print('Epoch: {0}'.format(i))
	rnd_ind = np.random.randint(0, len(X))
	s.train(X[rnd_ind,:])

print(s.get_weights())

# plot.imshow( np.reshape(s.get_weights(), [map_size, map_size, num_features]) )

# plot.show()