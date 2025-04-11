import joblib
import numpy as np
import pandas as pd

class AnomalyClassifier:
    def _init_(self, model_path):
        """
        Initialize the classifier by loading the pre-trained model.
        :param model_path: Path to the trained model file (e.g., threat_model.pkl).
        """
        try:
            self.model = joblib.load(model_path)
            print(f"Model loaded successfully from {model_path}.")
        except FileNotFoundError:
            raise Exception(f"Model file not found at {model_path}. Please train the model first.")

    def classify(self, features):
        """
        Classify user behavior based on input features.
        :param features: A list or numpy array of feature values (e.g., login frequency, file access rate).
        :return: Prediction (1 for anomaly, 0 for normal behavior).
        """
        if not isinstance(features, (list, np.ndarray)):
            raise ValueError("Features must be a list or numpy array.")
        
        # Reshape input for the model
        features = np.array(features).reshape(1, -1)
        
        # Predict using the trained model
        prediction = self.model.predict(features)
        
        # Isolation Forest returns -1 for anomaly and 1 for normal behavior
        return 1 if prediction[0] == -1 else 0

    def batch_classify(self, data):
        """
        Classify multiple entries in batch mode.
        :param data: Pandas DataFrame with rows of feature values.
                     Each row is classified as normal or anomalous.
        :return: Pandas DataFrame with predictions added as a new column.
        """
        if not isinstance(data, pd.DataFrame):
            raise ValueError("Data must be a Pandas DataFrame.")
        
        # Predict for all rows in the DataFrame
        predictions = self.model.predict(data)
        
        # Convert predictions (-1 for anomaly, 1 for normal) to binary format (1 for anomaly, 0 for normal)
        data['prediction'] = [1 if p == -1 else 0 for p in predictions]
        
        return data


# Example usage
if _name_ == "_main_":
    # Path to the pre-trained model
    MODEL_PATH = "threat_model.pkl"

    # Initialize classifier
    classifier = AnomalyClassifier(MODEL_PATH)

    # Example single classification
    example_features = [12, 3]  # Example feature values: login frequency, file access rate
    prediction = classifier.classify(example_features)
    print(f"Single classification result: {'Anomaly' if prediction == 1 else 'Normal'}")

    # Example batch classification
    sample_data = pd.DataFrame({
        'login_freq': [12, 45, 15],
        'file_access': [3, 78, 5]
    })
    results = classifier.batch_classify(sample_data)
    print("Batch classification results:")
    print(results)