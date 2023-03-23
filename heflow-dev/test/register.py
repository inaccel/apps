import heflow.tensorflow.keras.activations
import mlflow

with mlflow.start_run():
    heflow.log_model(heflow.tensorflow.keras.activations.he_sigmoid,
                     registered_model_name='he_sigmoid')
