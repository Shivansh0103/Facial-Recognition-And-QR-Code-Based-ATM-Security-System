import requests
import cv2
import face_recognition
import tkinter as tk
import time
# Initialize list of known faces
known_faces = []

# Define function to start the server and perform face recognition
def start_server():
    response = requests.get('https://djcollection.pythonanywhere.com/cam/1/')
    data = response.json()
    if 'id' in data:
        id_value = data['cam_access']
        print(data)
        print(id_value)
        if id_value == 1:
            # Open PC camera and take photo
            camera = cv2.VideoCapture(0)
            _, image = camera.read()
            cv2.imwrite('image.jpg', image)
            camera.release()
            
            # Perform facial recognition and make POST request with person face id
            face_encodings = face_recognition.face_encodings(image)
            if len(face_encodings) > 0:
                # Compare face encoding with known faces
                matches = face_recognition.compare_faces(known_faces, face_encodings[0])
                if True in matches:
                    # Get the index of the matched face
                    match_index = matches.index(True)
                    # Make POST request with person face id
                    face_id = f'face{match_index}'
                    print("face matched ")
                    response = requests.put('https://djcollection.pythonanywhere.com/cam/1/', data={'identified_face': str(face_id)})
                    print(response)
                    response = requests.put('https://djcollection.pythonanywhere.com/cam/1/', data={'cam_access': 0})
            
                else:
                    # Register the new face
                    known_faces.append(face_encodings[0])
                    face_id = f'face{len(known_faces) - 1}'
                    print("new face")
                    #response = requests.post('https://djcollection.pythonanywhere.com/cam/1/', data={'face_id': face_id})
            time.sleep(10)
            print("time up 15 sec")
        else:
            time.sleep(2)
            print('cam module checking every 2 sec')      

    window.after(200, start_server)
# Define function to register a new face
def register_face():
    # Open PC camera and take photo
    camera = cv2.VideoCapture(0)
    _, image = camera.read()
    cv2.imwrite('image.jpg', image)
    camera.release()
    
    # Encode the face and add it to the list of known faces
    face_encodings = face_recognition.face_encodings(image)
    if len(face_encodings) > 0:
        known_faces.append(face_encodings[0])
        

# Create the tkinter window
window = tk.Tk()

# Set the window size and make it non-resizable
window.geometry("640x480")
window.resizable(False, False)
window.configure(bg='#82aef5')
# Create the start server button
start_button = tk.Button(window, text="Start Server", bg="#a0a7ad",command=start_server)
start_button.place(x=100, y=120, width=150, height=90)

# Create the register face button
register_button = tk.Button(window, text="Register Face",bg="#a0a7ad", command=register_face)
register_button.place(x=400, y=120, width=150, height=90)


# Start the tkinter mainloop
window.mainloop()
