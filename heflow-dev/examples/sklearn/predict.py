import click
import grpc
import heflow
import sklearn.datasets
import sklearn.metrics
import sklearn.model_selection


@click.command()
@click.argument('model_uri')
def predict(model_uri):
    X, y = sklearn.datasets.fetch_openml('iris', return_X_y=True)

    _, X_test, _, y_test = sklearn.model_selection.train_test_split(
        X, y, test_size=3, random_state=42)

    model = heflow.load_model(model_uri).connect(
        grpc.insecure_channel('localhost:8080',
                              (('grpc.max_receive_message_length', -1),
                               ('grpc.max_send_message_length', -1))))

    predictions = model.predict(X_test)

    print('predictions=%s' % predictions)
    print('accuracy=%.3f' %
          sklearn.metrics.accuracy_score(y_test, predictions))


if __name__ == '__main__':
    predict()
