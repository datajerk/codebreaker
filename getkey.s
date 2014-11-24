.if CODESEG = 1
.segment "CODE1"
.endif

	.export _getkey
	.export	_count
	.export	_key

KBDCR	=	$D011		; Apple I got key?
KBDDATA	=	$D010		; The key pressed

_count:	.byte	0,0,0,0
_key:	.byte	0

_getkey:
	LDA	KBDCR		; cycles 4	got key?
	BMI	DONE		; cycles 2	if neg, got key goto DONE
	CLC			; cycles 2	clear carry
	LDA	#1		; cycles 2	A = 1
	ADC	_count+0	; cycles 4	LSB += A
	STA	_count+0	; cycles 4
	LDA	#0		; cycles 2	A = 0
	ROL	A		; cycles 2	A = carry, carry = 0
	ADC	_count+1	; cycles 4
	STA	_count+1	; cycles 4
	LDA	#0		; cycles 2	A = 0
	ROL	A		; cycles 2	A = carry, carry = 0
	ADC	_count+2	; cycles 4
	STA	_count+2	; cycles 4
	LDA	#0		; cycles 2	A = 0
	ROL	A		; cycles 2	A = carry, carry = 0
	ADC	_count+3	; cycles 4
	STA	_count+3	; cycles 4
	CLC			; cycles 2	clear carry
	BCC	_getkey		; cycles 2 + 1	back to checking for key
				; total = 59
DONE:	LDA	KBDDATA		; get key value
	AND	#$7F		; 7th bit to 0
	STA	_key		; store in _key
	RTS
