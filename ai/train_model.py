import pandas as pd
import numpy as np
from sklearn.ensemble import IsolationForest
from sklearn.model_selection import train_test_split
from sklearn.metrics import precision_score, recall_score, f1_score
import joblib

class ModelTrainer:
    def __init__(self, model_path="threat_model.pkl"):
        """
        Initialize the trainer with the path to save the trained model.
        :param model_path: Path to save the trained model (default: "threat_model.pkl").
        """
        self.model_path = model_path

    def load_data(self, data_path):
        """
        Load preprocessed data from a CSV file.
        :param data_path: Path to the CSV file containing preprocessed features and labels.
        :return: Features (X) and labels (y) as NumPy arrays.
        """
        data = pd.read_csv(data_path)
        X = data[['login_freq', 'file_access_rate']].values
        y = data['label'].values  # Binary labels: 0 (normal), 1 (anomaly)
        return X, y

    def train_model(self, X_train, model_config=None):
        """
        Train an Isolation Forest model using the training data.
        :param X_train: Training feature matrix.
        :param model_config: Dictionary of model parameters (optional)
        :return: Trained Isolation Forest model.
        """
        # Use default parameters if none provided
        if model_config is None:
            model_config = {
                'n_estimators': 100,
                'max_samples': 0.75,
                'contamination': 0.05,
                'random_state': 42
            }

        # Define hyperparameters for Isolation Forest
        model = IsolationForest(**model_config)
        
        # Fit the model on training data
        model.fit(X_train)
        
        return model

    def evaluate_model(self, model, X_test, y_test):
        """
        Evaluate the trained model on test data and calculate precision, recall, and F1 score.
        :param model: Trained Isolation Forest model.
        :param X_test: Test feature matrix.
        :param y_test: Test labels.
        :return: Precision, recall, and F1 score as floats.
        """
        # Predict anomalies on test data
        y_pred = model.predict(X_test)
        
        # Convert predictions (-1 for anomaly, 1 for normal) to binary format (1 for anomaly, 0 for normal)
        y_pred_binary = [1 if x == -1 else 0 for x in y_pred]
        
        # Calculate evaluation metrics
        precision = precision_score(y_test, y_pred_binary)
        recall = recall_score(y_test, y_pred_binary)
        f1 = f1_score(y_test, y_pred_binary)
        
        return precision, recall, f1

    def save_model(self, model):
        """
        Save the trained model to a file using joblib.
        :param model: Trained Isolation Forest model.
        """
        joblib.dump(model, self.model_path)
        print(f"Model saved successfully to {self.model_path}.")

    def run_pipeline(self, data_path, training_config=None, model_config=None):
        """
        Full training pipeline: load data -> split -> train -> evaluate -> save.
        :param data_path: Path to the CSV file containing preprocessed features and labels.
        :param training_config: Dictionary of training parameters (optional)
        """
        try:
            # Use default parameters if none provided
            if training_config is None:
                training_config = {
                    'test_size': 0.3,
                    'random_state': 42
                }

            # Step 1: Load the data
            print("Loading data...")
            X, y = self.load_data(data_path)

            # Step 2: Split the data into training and testing sets
            print("Splitting data into training and testing sets...")
            X_train, X_test, y_train, y_test = train_test_split(
                X, y, 
                test_size=training_config['test_size'],
                random_state=training_config['random_state']
            )

            # Step 3: Train the model
            print("Training the model...")
            model = self.train_model(X_train, model_config=model_config)

            # Step 4: Evaluate the model
            print("Evaluating the model...")
            precision, recall, f1 = self.evaluate_model(model, X_test, y_test)
            print(f"\nModel Evaluation Metrics:")
            print(f"Precision: {precision:.2f}")
            print(f"Recall:    {recall:.2f}") 
            print(f"F1 Score:  {f1:.2f}")

            # Step 5: Save the trained model
            print("\nSaving the model...")
            self.save_model(model)

            print("\nPipeline completed successfully!")

        except Exception as e:
            print(f"\nError in training pipeline: {str(e)}")
            raise