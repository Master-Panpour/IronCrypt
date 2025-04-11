import yaml
import pandas as pd
from train_model import ModelTrainer
from sklearn.model_selection import train_test_split

def load_config(config_path="configs/model_params.yaml"):
    """Load model configuration from YAML file"""
    with open(config_path) as f:
        return yaml.safe_load(f)

def load_and_prepare_data(data_path, features):
    """Load and prepare training data"""
    data = pd.read_csv(data_path)
    return data[features].values, data['label'].values

def main():
    try:
        # Load configuration
        config = load_config()
        model_config = config['model']
        training_config = config['training']
        features = config['features']['columns']

        # 1. Load and prepare data
        print("Loading and preparing data...")
        X, y = load_and_prepare_data("enhanced_training_data.csv", features)

        # 2. Split data using configured parameters
        print("Splitting data...")
        X_train, X_test, y_train, y_test = train_test_split(
            X, y,
            test_size=training_config['test_size'],
            random_state=training_config['random_state']
        )

        # 3. Initialize and train model
        print("Initializing model trainer...")
        trainer = ModelTrainer(model_path="normalization/threat_model.pkl")
        
        # 4. Train and evaluate model
        print("Running training pipeline...")
        trainer.run_pipeline(
            data_path="enhanced_training_data.csv",
            training_config={
                'test_size': training_config['test_size'],
                'random_state': training_config['random_state']
            },
            model_config={
                **{k: v for k, v in model_config.items() if k != 'name'},
                'contamination': 0.1  # Adjusted for better anomaly detection
            }
        )

        print("\nPipeline completed successfully!")

    except Exception as e:
        print(f"\nError in model execution: {str(e)}")
        raise

if __name__ == "__main__":
    main()
