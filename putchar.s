.if CODESEG = 1
.segment "CODE1" 
.endif

.export		_putchar

ECHO	=	$FFEF		; print character from A

_putchar:	
	;ORA	$80
	JMP	ECHO
