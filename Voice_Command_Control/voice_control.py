import speech_recognition as sr
import requests

# NodeMCU IP address
NODEMCU_IP = "http://192.168.1.100"

def send_command(command):
    url = f"{NODEMCU_IP}/voice_command?command={command}"
    try:
        requests.get(url)
        print(f"Command sent: {command}")
    except requests.exceptions.RequestException as e:
        print("Error sending command:", e)

def main():
    recognizer = sr.Recognizer()
    microphone = sr.Microphone()

    print("Voice Control Started. Say 'exit' to quit.")
    while True:
        with microphone as source:
            recognizer.adjust_for_ambient_noise(source)
            print("Listening...")
            audio = recognizer.listen(source)

        try:
            command = recognizer.recognize_google(audio).lower()
            print(f"You said: {command}")

            if command == "exit":
                print("Exiting...")
                break

            if command in ["forward", "backward", "left", "right", "stop"]:
                send_command(command)
            else:
                print("Command not recognized. Try: forward, backward, left, right, stop")

        except sr.UnknownValueError:
            print("Could not understand audio")
        except sr.RequestError as e:
            print("Could not request results; {0}".format(e))

if __name__ == "__main__":
    main()
