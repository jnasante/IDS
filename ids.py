import data_processor
import som_ids
import neural_gas_ids
from matplotlib import pyplot as plt



def run_net(m):
	malicious = data_processor.count()
	groups = {}
	for test_subject in range(data_processor.num_samples):
		winner = m.flat_to_coords(m.winner(data_processor.X[test_subject]))
		attack_name = data_processor.y[test_subject]

		if (winner not in groups):
			groups[winner] = {}
		if (attack_name not in groups[winner]):
			groups[winner][attack_name] = 0

		groups[winner][attack_name] += 1

	for winner in groups:
		print('WINNING NEURON: {0}'.format(winner))
		for classification in groups[winner]:
			print('\tClassification: {0}\n\t\tNumber: {1}'.format(classification, groups[winner][classification]))
		print('')

	# for attack in malicious_types:
	# 	print('Classification: {0}\nWinning neuron: {1}\nNumber: {2}\n'.format(attack[0], attack[1], malicious_types[attack]))

	# print(m.neuron(winner))
	# print(X[744])
	# print(y[744])

	plt.imshow(m.neuron_heatmap())
	# plt.imshow(m.distance_heatmap(X[0]))
	plt.show()

# som_ids.trainKohonenSOM()
# run_net(som_ids.getObject())

neural_gas_ids.trainKohonenGrowingGas()
run_net(neural_gas_ids.getObject())