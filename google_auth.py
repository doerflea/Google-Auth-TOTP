import pyqrcode
from pyqrcode import QRCode
import png
#pip install pyotp
#pip install pyqrcode
#https://github.com/pyotp/pyotp/
#https://pypi.org/project/PyQRCode/

s = "otpauth://totp/ACME%20Co:cs370authtest2@gmail.com?secret=AAHA4AAOADWAAAAO&issuer=ACME%20Co&algorithm=SHA1&digits=6&period=30"
url = pyqrcode.create(s)

url.svg("myqr.svg", scale = 8)


