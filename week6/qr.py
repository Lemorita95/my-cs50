import qrcode

img = qrcode.make("https://youtube.com")
img.save("qr.png", "PNG")
