import tkinter as tk
from tkinter import ttk
from PIL import Image, ImageTk
import requests
from io import BytesIO
import time

# Replace with your phone's IP (from IP Webcam)
PHONE_IP = "172.31.76.44:8080/"
prefix = f"http://{PHONE_IP}"
SNAPSHOT_URL = prefix + "shot.jpg"
set_as_portrait = prefix + "settings/orientation?set=portrait"
turn_on_torch = prefix + "enabletorch"
turn_off_torch = prefix + "disabletorch"


class TriplePhotoGUI:
    def __init__(self, root):
        # Set orientation to portrait once at startup
        try:
            requests.get(set_as_portrait, timeout=3)
        except:
            pass

        self.root = root
        self.root.title("Triple Photo Capture")
        self.root.geometry("1300x700")

        self.frame = ttk.Frame(root, padding=10)
        self.frame.pack(fill="both", expand=True)

        self.capture_button = ttk.Button(
            self.frame, text="📸 Take Three Photos", command=self.take_three_photos
        )
        self.capture_button.pack(pady=10)

        self.status_label = ttk.Label(self.frame, text="Ready", anchor="center")
        self.status_label.pack(pady=10)

        self.display_frame = ttk.Frame(self.frame)
        self.display_frame.pack(pady=10)

    def take_three_photos(self):
        try:
            self.status_label.config(text="Capturing 3 photos...")
            self.root.update_idletasks()

            # Capture 1: no torch
            requests.get(turn_off_torch)
            time.sleep(0.4)
            img1 = self.fetch_image()

            #--------------------------------------------------------------------------
            #modify the image with bensons code
            #-----------------------------------------------------------------------

            # Capture 2: lighted
            requests.get(turn_on_torch)
            time.sleep(0.4)
            img2 = self.fetch_image()
            time.sleep(0.4)
            requests.get(turn_off_torch)

            # Convert to Tk-compatible PhotoImages
            tk_imgs = []
            for img in [img1, img2, img2]:
                img_copy = img.copy()
                img_copy.thumbnail((700, 500))
                tk_imgs.append(ImageTk.PhotoImage(img_copy))

            self.display_three_photos(tk_imgs)
            self.status_label.config(text="✅ Three photos captured!")

        except Exception as e:
            self.status_label.config(text=f"❌ Error: {e}")

    def fetch_image(self):
        """Grab one snapshot from IP Webcam."""
        r = requests.get(SNAPSHOT_URL, timeout=5)
        r.raise_for_status()
        return Image.open(BytesIO(r.content))

    def display_three_photos(self, photos):
        labels = ["original image", "modified image (lighted as placeholder)", "image with light"]
        """Display three images side-by-side with labels."""
        # Clear previous photos
        for widget in self.display_frame.winfo_children():
            widget.destroy()

        for i, photo in enumerate(photos):
            sub_frame = ttk.Frame(self.display_frame, padding=5)
            sub_frame.pack(side="left", padx=10)

            img_label = ttk.Label(sub_frame, image=photo)
            img_label.image = photo  # keep reference
            img_label.pack()

            text_label = ttk.Label(sub_frame, text=labels[i])
            text_label.pack()


if __name__ == "__main__":
    root = tk.Tk()
    app = TriplePhotoGUI(root)
    root.mainloop()
