import data_processor
import som_ids
import neural_gas_ids
import time
import numpy as np
from matplotlib import pyplot as plt

# Initialize
data_processor.initialize()
# Variables
categories = {}

def run_net(m, heatmap=False):
	global categories

	malicious = data_processor.count()

	form_categories(get_groups(m))
	accuracy = predict_all(m)

	if (heatmap):
		plt.imshow(m.neuron_heatmap())
		# plt.imshow(m.distance_heatmap(X[0]))
		plt.show()

	return categories, accuracy

def get_groups(m):
	groups = {}
	for test_subject in range(len(data_processor.X_train)):
		winner = get_winner(m, data_processor.X_train[test_subject])
		attack_name = data_processor.y_train[test_subject]

		if (winner not in groups):
			groups[winner] = {}
			categories[winner] = 'normal.'
		if (attack_name not in groups[winner]):
			groups[winner][attack_name] = 0

		groups[winner][attack_name] += 1

	return groups

def form_categories(groups):
	for winner in groups:
		# print('WINNING NEURON: {0}'.format(winner))
		max_number = -1
		for classification in groups[winner]:
			if (groups[winner][classification] > max_number):
				max_number = groups[winner][classification]
				categories[winner] = classification[:-1]
			# print('\tClassification: {0}\n\t\tNumber: {1}'.format(classification, groups[winner][classification]))
		# print('')

def get_winner(m, sample):
	return m.flat_to_coords(m.winner(sample))

def predict(m, sample):
	winner = get_winner(m, sample)
	prediction = 'normal.' if winner not in categories else categories[winner]
	# if (prediction != 'smurf'):
	# 	print(prediction)
	return prediction

def predict_all(m):
	start = time.time()
	correct = 0.0
	total_test_samples = len(data_processor.X_test)
	for test_subject in range(total_test_samples):
		prediction = predict(m, data_processor.X_test[test_subject])
		actual = data_processor.y_test[test_subject][:-1]
		if (prediction == actual):
			correct += 1.0

		# print('Prediction: {0}\nActual: {1}\n'.format(prediction, actual)

	accuracy = correct/float(total_test_samples)
	print('Accuracy: {0}\tNumber of samples: {1}\tClassification time: {2}'.format(accuracy, total_test_samples, time.time()-start))
	return accuracy

################## Collect Statistics ##################
metrics_range = range(1, 21, 1)
def get_metrics(m, training_method, get_object_method, suffix):
	times = []
	accuracies = []

	for epoch in metrics_range:
		if (epoch % 10 == 0):
			print('\nEpoch: {0}\n'.format(epoch))

		start = time.time()

		m = training_method(1, m)
		_, accuracy = run_net(m)
		accuracies.append(accuracy)
		times.append(time.time() - start)

	np.savetxt('metrics/accuracies_{0}_{1}-{2}.txt'.format(suffix, metrics_range[0], metrics_range[-1]), accuracies)
	np.savetxt('metrics/times_{0}_{1}-{2}.txt'.format(suffix, metrics_range[0], metrics_range[-1]), times)

def graph_accuracies(title, suffix):
	accuracies = np.loadtxt('metrics/accuracies_{0}_{1}-{2}.txt'.format(suffix, metrics_range[0], metrics_range[-1]))

	plt.plot(metrics_range, accuracies)
	plt.ylim([0.60, 1])
	plt.title(title)
	plt.xlabel('Epochs (Training Set)')
	plt.ylabel('Accuracy (Test Set)')
	plt.show()

def calculate_times(suffix):
	times = np.loadtxt('metrics/times_{0}_{1}-{2}.txt'.format(suffix, metrics_range[0], metrics_range[-1]))
	tpe = np.sum(times)/float(np.sum(metrics_range))
	print('Time Per Epoch: {0}s'.format(tpe))

# som_ids.trainKohonenSOM()
run_net(som_ids.getObject())

# neural_gas_ids.trainKohonenGrowingGas(iterations=5)
run_net(neural_gas_ids.getObject())

# get_metrics(neural_gas_ids.createKohonenGrowingGas(), neural_gas_ids.trainKohonenGrowingGas, neural_gas_ids.getObject, 'ng')
# graph_accuracies('Epochs vs. Accuracy (Growing Neural Gas)', 'ng')
# calculate_times('ng')

# get_metrics(som_ids.createKohonenSOM(), som_ids.trainKohonenSOM, som_ids.getObject, 'som')
# graph_accuracies('Epochs vs. Accuracy (Self-Organizing Map)', 'som')
# calculate_times('som')
