.if empty(MACHINE_ARCH:M*eb)
.PATH.S: ${.PARSEDIR}

#AES_SRCS = aes-mips${"${COPTS:M*-mabi=64*}" == "":?:64}.S
#AESCPPFLAGS = -DAES_ASM
.endif

.include "../../aes.inc"
