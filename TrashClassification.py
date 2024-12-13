# from keras.models import load_model
from tensorflow.keras.models import load_model

import cv2
import numpy as np
import serial
import time

# Disable scientific notation for clarity
np.set_printoptions(suppress=True)

# Load the model
model = load_model("keras_model.h5", compile=False)

# Load the labels
class_names = open("labels.txt", "r").readlines()

# CAMERA can be 0 or 1 based on the default camera of your computer
camera = cv2.VideoCapture(0)

arduino = serial.Serial('COM3', 9600)  # Replace 'COM3' with the correct port name

while True:
    # Grab the webcam's image.
    ret, image = camera.read()

    # Resize the raw image into (224-height,224-width) pixels
    image = cv2.resize(image, (224, 224), interpolation=cv2.INTER_AREA)


    # Add Text
    text = "Press P to capture image"
    font = cv2.FONT_HERSHEY_SIMPLEX
    org = (0,30)
    font_scale = .50
    color = (0,255,255)
    thickness = 2
    image = cv2.putText(image, text, org, font, font_scale, color, thickness, cv2.LINE_AA)

    # Show the image in a window
    cv2.imshow("Webcam Image", image)

    # Listen to the keyboard for key presses.
    keyboard_input = cv2.waitKey(1)

    # 27 is the ASCII code for the 'esc' key.
    if keyboard_input == 27:
        break
    #112 is the ASCII code for the 'p' key.
    elif keyboard_input == 112:
        #Convert the image to the format expected by the model

        pro_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        pro_image = pro_image / 255.0
        pro_image = np.expand_dims(pro_image, axis=0)

        # Predict the model
        prediction = model.predict(pro_image)
        index = np.argmax(prediction)
        class_name = class_names[index]
        confidence_score = prediction[0][index]

        ###########################
        if confidence_score > 0.95:
            data = class_name
            arduino.write(data.encode())
            print(confidence_score)

            # Print prediction and confidence score
            print("Class:", class_name)
            print("Confidence Score:", str(np.round(confidence_score * 100, decimals=2)) + "%")
            time.sleep(0.9)

# Close the serial connection
arduino.close()
camera.release()
cv2.destroyAllWindows()
