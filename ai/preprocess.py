import pandas as pd
import numpy as np
from sklearn.preprocessing import StandardScaler, MinMaxScaler

class DataPreprocessor:
    def _init_(self):
        """
        Initialize the preprocessor with scalers for feature normalization and scaling.
        """
        self.standard_scaler = StandardScaler()
        self.minmax_scaler = MinMaxScaler()

    def clean_data(self, data):
        """
        Clean raw data by handling missing values, removing duplicates, and sanitizing inputs.
        :param data: Pandas DataFrame containing raw logs or features.
        :return: Cleaned Pandas DataFrame.
        """
        if not isinstance(data, pd.DataFrame):
            raise ValueError("Input data must be a Pandas DataFrame.")

        # Remove duplicates
        data = data.drop_duplicates()

        # Handle missing values (e.g., fill with median or drop rows)
        data = data.fillna(data.median())

        # Sanitize string fields (if applicable)
        for column in data.select_dtypes(include=['object']).columns:
            data[column] = data[column].str.strip().str.lower()

        return data

    def extract_features(self, raw_logs):
        """
        Extract meaningful features from raw logs (e.g., login frequency, file access rate).
        :param raw_logs: Pandas DataFrame containing raw log entries.
                         Example columns: ['user', 'timestamp', 'action', 'resource']
        :return: Pandas DataFrame with extracted features.
                 Example columns: ['login_freq', 'file_access_rate']
        """
        if not isinstance(raw_logs, pd.DataFrame):
            raise ValueError("Raw logs must be a Pandas DataFrame.")

        # Example feature extraction logic
        features = pd.DataFrame()
        
        # Calculate login frequency per user
        login_counts = raw_logs[raw_logs['action'] == 'login'].groupby('user').size()
        
        # Calculate file access rate per user
        file_access_counts = raw_logs[raw_logs['action'] == 'file_access'].groupby('user').size()
        
        # Merge extracted features into a single DataFrame
        features['user'] = login_counts.index
        features['login_freq'] = login_counts.values
        features['file_access_rate'] = file_access_counts.reindex(login_counts.index, fill_value=0).values

        return features

    def scale_features(self, features):
        """
        Scale numerical features using standardization or normalization.
        :param features: Pandas DataFrame containing numerical feature columns.
                         Example columns: ['login_freq', 'file_access_rate']
        :return: Scaled Pandas DataFrame.
                 Columns are scaled to a range suitable for machine learning models.
        """
        if not isinstance(features, pd.DataFrame):
            raise ValueError("Features must be a Pandas DataFrame.")

        # Apply standard scaling (mean=0, std=1)
        scaled_features = pd.DataFrame(
            self.standard_scaler.fit_transform(features),
            columns=features.columns,
            index=features.index
        )

        return scaled_features

    def preprocess_pipeline(self, raw_logs):
        """
        Full preprocessing pipeline: clean data -> extract features -> scale features.
        :param raw_logs: Pandas DataFrame containing raw log entries.
                         Example columns: ['user', 'timestamp', 'action', 'resource']
                         Raw logs are cleaned and transformed into scaled feature vectors.
        :return: Preprocessed feature matrix (Pandas DataFrame).
                 Example columns: ['login_freq', 'file_access_rate']
                 Ready for training or classification.
        """
        print("Starting preprocessing pipeline...")
        
        # Step 1: Clean the raw logs
        cleaned_data = self.clean_data(raw_logs)
        
        # Step 2: Extract meaningful features
        extracted_features = self.extract_features(cleaned_data)
        
        # Step 3: Scale the extracted features
        scaled_features = self.scale_features(extracted_features[['login_freq', 'file_access_rate']])
        
        print("Preprocessing pipeline completed.")
        
        return scaled_features


# Example usage
if _name_ == "_main_":
    # Sample raw logs (replace with actual log data)
    sample_logs = pd.DataFrame({
        'user': ['john', 'sarah', 'john', 'mike', 'sarah'],
        'timestamp': ['2025-04-12T10:00:00', '2025-04-12T10:05:00', 
                      '2025-04-12T10:10:00', '2025-04-12T10:15:00',
                      '2025-04-12T10:20:00'],
        'action': ['login', 'login', 'file_access', 'login', 'file_access'],
        'resource': ['/home/john/file.txt', '/home/sarah/file.docx',
                     '/home/john/data.csv', '/home/mike/report.pdf',
                     '/home/sarah/data.csv']
    })

    preprocessor = DataPreprocessor()
    
    # Run preprocessing pipeline
    processed_data = preprocessor.preprocess_pipeline(sample_logs)
    
    print("Processed Feature Matrix:")
    print(processed_data)