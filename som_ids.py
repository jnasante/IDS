import data_processor

# Parameters
cluster_units_number = (50, 50)
epochs = 10
learning_rate = 0.5
neighborhood_number = 7
obj_file_name = 'objects/kohonenSOM.pkl'

############### Kohonen SOM ###############
def createKohonenSOM(iterations=epochs):
	import kohonen

	params = kohonen.Parameters(
		dimension=data_processor.num_features,
		shape=cluster_units_number,
		learning_rate=learning_rate,
		neighborhood_size=neighborhood_number)

	m = kohonen.Map(params)
	m.reset() # Need to reset after initialization

	return m

def trainKohonenSOM(iterations=epochs, m=None):
	if (m is None):
		m = createKohonenSOM()

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
