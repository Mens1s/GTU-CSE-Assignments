import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier, export_text

# load dataset
data = pd.read_csv('./iris/iris.data', header=None, names=['sepal_length', 'sepal_width', 'petal_length', 'petal_width', 'class'])

# assuming last column is the target
X = data.iloc[:, :-1]
y = data.iloc[:, -1]

# split data into train and test sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = 0.25, random_state = 40)

# Create a Decision Tree model and train it
clf = DecisionTreeClassifier()
clf.fit(X, y)

# Apply the model to the test data and export it to console
rules = export_text(clf, feature_names=list(X.columns))
print(rules)