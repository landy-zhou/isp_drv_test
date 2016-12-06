all:
	ndk-build -B V=1
	cp obj/local/armeabi-v7a/isp-driver-test ../ramdisk_linux/bin/
	cd ../ramdisk_linux && rm -f ../ramdisk.img && find . | cpio -o -H newc | gzip > ../ramdisk.img
