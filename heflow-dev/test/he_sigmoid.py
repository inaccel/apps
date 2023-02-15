import grpc
import heflow.tensorflow.keras.activations
import heflow.tensors

x = [0.5]

result = heflow.tensorflow.keras.activations.he_sigmoid(
    heflow.tensors.ckks_tensor(x)).numpy()

print('result=%s' % result)

heflow.tensorflow.keras.activations.he_sigmoid.connect(
    grpc.insecure_channel('localhost:8080',
                          (('grpc.max_receive_message_length', -1),
                           ('grpc.max_send_message_length', -1))))

result = heflow.tensorflow.keras.activations.he_sigmoid(
    heflow.tensors.ckks_tensor(x)).numpy()

print('result=%s' % result)

he_sigmoid = heflow.load_model('models:/he_sigmoid/1').connect(
    grpc.insecure_channel('localhost:8080',
                          (('grpc.max_receive_message_length', -1),
                           ('grpc.max_send_message_length', -1))))

result = he_sigmoid(heflow.tensors.ckks_tensor(x)).numpy()

print('result=%s' % result)
