import data_processor
from data_processor import data, X, y

# Parameters
num_features = len(X[0])
cluster_units_number = (50,)
epochs = 200
obj_file_name = 'objects/kohonenGrowingGas.pkl'

############### Kohonen Growing Gas ###############
def trainKohonenGrowingGas():
	import kohonen

	params = kohonen.GrowingGasParameters(
		dimension=num_features,
		shape=(cluster_units_number))

	m = kohonen.GrowingGas(params)
	m.reset() # Need to reset after initialization

	# Time for school!
	for e in range(epochs):
		print('Epoch: {0}'.format(e))
		for sample in X:
			m.learn(sample)

	# Save to file
	data_processor.save_object(m, obj_file_name)

def getObject():
	return data_processor.get_object(obj_file_name)
