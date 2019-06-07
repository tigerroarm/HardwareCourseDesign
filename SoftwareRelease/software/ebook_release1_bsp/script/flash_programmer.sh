#!/bin/sh
#
# This file was automatically generated.
#
# It can be overwritten by nios2-flash-programmer-generate or nios2-flash-programmer-gui.
#

#
# Converting SOF File: G:\altera_ED2_115\HardwareCourseDesign\HardWare\DE2_115.sof to: "..\flash/DE2_115_epcs.flash"
#
sof2flash --input="G:/altera_ED2_115/HardwareCourseDesign/HardWare/DE2_115.sof" --output="../flash/DE2_115_epcs.flash" --epcs 

#
# Programming File: "..\flash/DE2_115_epcs.flash" To Device: epcs
#
nios2-flash-programmer "../flash/DE2_115_epcs.flash" --base=0x0 --epcs --sidp=0x18002070 --id=0x12345678 --timestamp=1559872952 --device=1 --instance=0 '--cable=USB-Blaster on localhost [USB-0]' --program --override=E:/altera/15.0/nios2-flash-override.txt

#
# Converting ELF File: G:\altera_ED2_115\HardwareCourseDesign\SoftwareRelease\software\ebook_release1\ebook_release1.elf to: "..\flash/ebook_release1_epcs.flash"
#
elf2flash --input="G:/altera_ED2_115/HardwareCourseDesign/SoftwareRelease/software/ebook_release1/ebook_release1.elf" --output="../flash/ebook_release1_epcs.flash" --epcs --after="../flash/DE2_115_epcs.flash" 

#
# Programming File: "..\flash/ebook_release1_epcs.flash" To Device: epcs
#
nios2-flash-programmer "../flash/ebook_release1_epcs.flash" --base=0x0 --epcs --sidp=0x18002070 --id=0x12345678 --timestamp=1559872952 --device=1 --instance=0 '--cable=USB-Blaster on localhost [USB-0]' --program --override=E:/altera/15.0/nios2-flash-override.txt

