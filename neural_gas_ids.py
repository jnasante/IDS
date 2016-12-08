import data_processor

# Parameters
cluster_units_number = (50,)
epochs = 1
obj_file_name = 'objects/kohonenGrowingGas.pkl'

############### Kohonen Growing Gas ###############
def createKohonenGrowingGas():
	import kohonen

	params = kohonen.GrowingGasParameters(
		dimension=data_processor.num_features,
		shape=(cluster_units_number))

	m = kohonen.GrowingGas(params)
	m.reset() # Need to reset after initialization

	return m

def trainKohonenGrowingGas(iterations=epochs, m=None):
	if (m is None):
		m = createKohonenGrowingGas()

	# Time for school!
	for e in range(iterations):
		# if (e % 10 == 0):
		# 	print('Epoch: {0}'.format(e))
		for sample in data_processor.X_train:
			m.learn(sample)

	# Save to file
	data_processor.save_object(m, obj_file_name)

	return m

def getObject():
	return data_processor.get_object(obj_file_name)
