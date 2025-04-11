import pandas as pd
from sklearn.ensemble import IsolationForest
from sklearn.model_selection import ParameterGrid, train_test_split
from sklearn.metrics import precision_score, recall_score, f1_score

# Load preprocessed data
data = pd.DataFrame({
    'login_freq': [12, 45, 15, 30, 50],
    'file_access_rate': [3, 78, 5, 20, 100],
    'label': [0, 1, 0, 0, 1]
})
X = data[['login_freq', 'file_access_rate']]
y = data['label']

# Split data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# Define hyperparameters to tune
param_grid = {
    'n_estimators': [50, 100, 200],
    'max_samples': [0.5, 0.75, 1.0],
    'contamination': [0.01, 0.05, 0.1],
    'random_state': [42]
}

# Initialize results storage
results = []

# Grid Search for Hyperparameter Tuning
for params in ParameterGrid(param_grid):
    model = IsolationForest(**params)
    model.fit(X_train)

    # Predict anomalies on the test set
    y_pred = model.predict(X_test)
    y_pred = [1 if x == -1 else 0 for x in y_pred]

    # Evaluate model performance with proper handling of undefined metrics
    precision = precision_score(y_test, y_pred, zero_division=1)  # Avoid ill-defined warnings
    recall = recall_score(y_test, y_pred, zero_division=1)
    f1 = f1_score(y_test, y_pred, zero_division=1)

    # Store results
    results.append({
        'params': params,
        'precision': precision,
        'recall': recall,
        'f1_score': f1,
    })

# Save results to CSV for analysis
results_df = pd.DataFrame(results)
results_df.to_csv('grid_search_results.csv', index=False)

print("Hyperparameter tuning completed. Results saved to grid_search_results.csv.")
