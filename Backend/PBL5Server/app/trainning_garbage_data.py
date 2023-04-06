
import numpy as np
from keras.applications.mobilenet_v2 import preprocess_input

from keras.models import load_model
import cv2
model = load_model("garbage.model")
image = cv2.imread("bohuc.jpg")
image = cv2.resize(image, (224, 224))
image = np.expand_dims(image, axis=0)
image = image.astype("float32")
image = preprocess_input(image)

result = model.predict(image)

if result[0][0] > result[0][1]:
    print("Không tải chế")
else:
    print("Tải chế")



