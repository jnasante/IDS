import data_processor
from data_processor import data, X, y, classification, classification_id
import numpy as np
from matplotlib import pyplot as plt

# Parameters
num_features = len(X[0])
cluster_units_number = (50, 50)
epochs = 100
obj_file_name = 'objects/kohonenGrowingGas.pkl'

############### Kohonen Growing Gas ###############
def trainKohonenGrowingGas():
	import kohonen

	params = kohonen.GrowingGasParameters(
		dimension=num_features,
		shape=(50,))

	m = kohonen.GrowingGas(params)
	m.reset() # Need to reset after initialization

	# Time for school!
	for e in range(1):
		print('Epoch: {0}'.format(e))
		for sample in X:
			m.learn(sample)

	# Save to file
	data_processor.save_object(m, obj_file_name)

def kohonenGrowingGas():
	m = data_processor.get_object(obj_file_name)

	winner = m.flat_to_coords(m.winner(X[0]))
	winning_neuron = m.neuron(winner)
	result = np.dot(winning_neuron, X[0])
	print(winner)
	print(winning_neuron)
	print(X[0])
	print(result)
	print(y[0])

	# m.neuron_heatmap()

	plt.imshow(m.neuron_heatmap())
	plt.show()


trainKohonenGrowingGas()
kohonenGrowingGas()