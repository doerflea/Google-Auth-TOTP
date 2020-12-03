# Google-Auth-TOTP

1) Run make
   *location of python install needs to be known to compiler, currently configured for flip server
   *yes I did make this 10 times harder than it needs to be by mixing python and c

2) Run doerflea-MP4 --generate-qr
   QR code with be generated into a file called myqr.svg.
   *SVG qr code was tested with: https://www.rapidtables.com/web/tools/svg-viewer-editor.html
   *I was only able to test this IOS and not android

3) Run doerflea-MP4 --get-otp
   This will generate the current otp code once with each run.
