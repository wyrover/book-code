setlocal
"..\system32\nasm.exe" -o "..\Debug\startup.com" -f bin main.asm
"..\system32\nasm.exe" -o "..\Debug\ntos.img" -f bin boot.asm
"..\system32\qemu-img.exe" convert "..\Debug\ntos.img" -O vmdk "..\Debug\ntos.vmdk"
endlocal