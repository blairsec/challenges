.segment "HEADER"
.byte $4E, $45, $53, $1A
.byte $02 ; 16KB PRG
.byte $01 ; 8K CHR
.byte $00
.byte $00
.byte $00
.byte $00
.byte $00
.byte $00
.byte $00
.byte $00
.byte $00
.byte $00

.segment "ZEROPAGE"
temp: .res 1
temp2: .res 1
temp3: .res 1
temp4: .res 1
buttons: .res 1
screenmode: .res 1
tempptr1: .res 2
tempptr2: .res 2
tempptr3: .res 2
character: .res 1
maxchars: .res 1
oamindex: .res 1
playerx: .res 1
playery: .res 1
updatepal: .res 1
updatestats: .res 1
argptr: .res 2
needdrawtext: .res 1
help: .res 1
waitnextnmi: .res 1
updatescr: .res 1
frametimer: .res 1
doflippy: .res 1
needcheckpos: .res 1
software: .res 1
cash: .res 1
flags: .res 1
defaultctrl: .res 1
clearbelow: .res 1
scrollval: .res 1
buffer1: .res 16
temp5: .res 1
temp6: .res 1
buffer2: .res 16
decoded_flag: .res 64
coflag: .res 1
.segment "CODE"
nmi:
	pha
	lda waitnextnmi
	beq @nowait
	pla
	dec waitnextnmi
	rti
@nowait:
	txa
	pha
	tya
	pha
	lda frametimer
	beq @skipft
	dec frametimer
@skipft:
	lda #$00
	sta $2000
	sta $2001
	bit $2002
	lda #$02
	sta $4014
	lda #$01
	sta $4016
	sta buttons
	lsr a
	sta $4016
@loop1:
	lda $4016
	lsr a
	rol buttons
	bcc @loop1
	lda updatepal
	beq @noupdate
	lda #$3f
	sta $2006
	lda #$18
	sta $2006
	lda character
	asl a
	asl a
	tax
	lda #$0f
	sta $2007
	lda #$00
	sta $2007
	lda #$10
	sta $2007
	lda #$20
	sta $2007
	lda character_palettes,x
	sta $2007
	inx
	lda character_palettes,x
	sta $2007
	inx
	lda character_palettes,x
	sta $2007
	inx
	lda character_palettes,x
	sta $2007
	inx
	dec updatepal
@noupdate:
	lda updatescr
	beq @noupdatescr
	jsr draw_screen
	inc waitnextnmi
	dec updatescr
@noupdatescr:
	lda updatestats
	bne @doupdate2
	jmp @noupdate2
@doupdate2:
	ldx character
	lda #$21
	sta $2006
	lda #$53
	sta $2006
	lda #$00
	sta temp
@drawboxes:
	ldy temp
	lda empty_bar,y
	tay
	lda rev_scores,x
	cmp temp
	bcc @skip
	beq @skip
	ldy #$31
@skip:
	sty $2007
	inc temp
	lda temp
	cmp #$06
	bne @drawboxes
	lda #$21
	sta $2006
	lda #$73
	sta $2006
	lda #$00
	sta temp
@drawboxes2:
	ldy temp
	lda empty_bar,y
	tay
	lda pwn_scores,x
	cmp temp
	bcc @skip2
	beq @skip2
	ldy #$31
@skip2:
	sty $2007
	inc temp
	lda temp
	cmp #$06
	bne @drawboxes2
	lda #$21
	sta $2006
	lda #$93
	sta $2006
	lda #$00
	sta temp
@drawboxes3:
	ldy temp
	lda empty_bar,y
	tay
	lda cry_scores,x
	cmp temp
	bcc @skip3
	beq @skip3
	ldy #$31
@skip3:
	sty $2007
	inc temp
	lda temp
	cmp #$06
	bne @drawboxes3
	lda #$21
	sta $2006
	lda #$b3
	sta $2006
	lda #$00
	sta temp
@drawboxes4:
	ldy temp
	lda empty_bar,y
	tay
	lda web_scores,x
	cmp temp
	bcc @skip4
	beq @skip4
	ldy #$31
@skip4:
	sty $2007
	inc temp
	lda temp
	cmp #$06
	bne @drawboxes4
	lda #$21
	sta $2006
	lda #$d3
	sta $2006
	lda #$00
	sta temp
@drawboxes5:
	ldy temp
	lda empty_bar,y
	tay
	lda inf_scores,x
	cmp temp
	bcc @skip5
	beq @skip5
	ldy #$31
@skip5:
	sty $2007
	inc temp
	lda temp
	cmp #$06
	bne @drawboxes5
	dec updatestats
@noupdate2:
	lda needdrawtext
	beq @nodrawtext
	inc waitnextnmi
	lda argptr
	sta tempptr1
	lda argptr+1
	sta tempptr1+1
	jsr draw_text
	dec needdrawtext
@nodrawtext:
	lda clearbelow
	beq @noclearbelow
	dec clearbelow
	inc waitnextnmi
	lda #$28
	sta $2006
	lda #$30
	sta $2006
	ldx #$00
@cloop1:
	sta $2007
	inx
	bne @cloop1
@cloop2:
	sta $2007
	inx
	bne @cloop2
@cloop3:
	sta $2007
	inx
	bne @cloop3
@cloop4:
	sta $2007
	inx
	bne @cloop4
@noclearbelow:
	lda needcheckpos
	bne @docheckpos
	jmp @nocheckpos
@docheckpos:
	dec needcheckpos
	lda playerx
	lsr a
	lsr a
	lsr a
	sta temp
	lda playery
	asl a
	asl a
	and #$e0
	ora temp
	sta tempptr1
	lda playery
	lsr a
	lsr a
	lsr a
	lsr a
	lsr a
	lsr a
	ora #$20
	sta tempptr1+1
	lda tempptr1+1
	sta $2006
	lda tempptr1
	sta $2006
	lda $2007
	ldx $2007
	lda collisions,x
	bne @badpos
	lda tempptr1
	clc
	adc #$02
	sta temp
	lda tempptr1+1
	adc #$00
	sta $2006
	lda temp
	sta $2006
	lda $2007
	ldx $2007
	lda collisions,x
	bne @badpos
	lda tempptr1
	clc
	adc #$40
	sta temp
	lda tempptr1+1
	adc #$00
	sta $2006
	lda temp
	sta $2006
	lda $2007
	ldx $2007
	lda collisions,x
	bne @badpos
	lda tempptr1
	clc
	adc #$42
	sta temp
	lda tempptr1+1
	adc #$00
	sta $2006
	lda temp
	sta $2006
	lda $2007
	ldx $2007
	lda collisions,x
	beq @nocheckpos
@badpos:
	lda temp2
	sta playerx
	lda temp3
	sta playery
@nocheckpos:
	lda coflag
	beq @nocoflag
	dec coflag
	ldx #$00
	lda #$28
	sta $2006
	lda #$00
	sta $2006
@coflagloop:
	lda decoded_flag,x
	tay
	lda text_lookup,y
	sta $2007
	inx
	cpx #$40
	bne @coflagloop
@nocoflag:
	lda #$00
	sta $2005
	lda scrollval
	sta $2005
	lda defaultctrl
	sta $2000
	lda #$00
	sta $2005
	lda scrollval
	sta $2005
@waitnmi:
	lda waitnextnmi
	bne @waitnmi
	lda #$00
	sta $2005
	lda scrollval
	sta $2005
	lda #$1e
	sta $2001
	pla
	tay
	pla
	tax
	pla
	rti
irq:
	rti
reset:
	sei
	cld
	ldx #40
	stx $4017
	ldx #$ff
	txs
	inx
	stx scrollval
	stx $2000
	stx $2001
	stx $4010
	bit $2002
@vblankwait1:
	bit $2002
	bpl @vblankwait1
	ldx #$ff
	txa
	inx
@loop1:
	sta oam,x
	inx
	bne @loop1
@vblankwait2:
	bit $2002
	bpl @vblankwait2
	lda #$00
	sta screenmode
	sta buttons
	sta character
	sta updatestats
	sta needdrawtext
	sta waitnextnmi
	sta updatescr
	sta updatepal
	sta frametimer
	sta needcheckpos
	sta software
	sta flags
	sta scrollval
	sta clearbelow
	sta coflag
	lda #$04
	sta maxchars
	lda #$01
	sta cash
	lda #$88
	sta defaultctrl
	sta $2000
	lda #$1e
	sta $2001
	lda #<title_screen
	sta tempptr3
	lda #>title_screen
	sta tempptr3+1
	lda #<title_palette
	sta tempptr2
	lda #>title_palette
	sta tempptr2+1
	inc updatescr
	jsr titlecode
	lda #<csel_screen
	sta tempptr3
	lda #>csel_screen
	sta tempptr3+1
	inc updatescr
	ldx #88
	stx playerx
	dex
	stx playery
cselloop:
	lda #$00
	sta oamindex
	inc updatepal
	inc updatestats
	jsr draw_player_metasprite
	ldx oamindex
	lda character
	asl a
	asl a
	asl a
	adc #$88
	sta oam,x
	inx
	lda #$10
	sta oam,x
	inx
	lda #$02
	sta oam,x
	inx
	lda #70
	sta oam,x
	inx
	stx oamindex
@waitpress:
@loop1:
	lda buttons
	bne @loop1
@loop2:
	lda buttons
	beq @loop2
	and #$20
	beq @noselect
	inc character
	lda character
	cmp maxchars
	bne @skip
	lda #$00
	sta character
@skip:
	jmp cselloop
@noselect:
	lda buttons
	and #$10
	beq @waitpress
	;lda #$00
	;sta tempptr1
	;sta tempptr1+1
	;jsr draw_screen
	lda #<room1
	sta tempptr3
	lda #>room1
	sta tempptr3+1
	lda #<room1_palette
	sta tempptr2
	lda #>room1_palette
	sta tempptr2+1
	inc updatescr
gameloop:
	lda flags
	cmp #$0f
	bne @notyet
	jmp youwon
@notyet:
	lda #$00
	sta oamindex
	lda doflippy
	beq @noflippy
	jsr draw_player_metasprite_flippy
	jmp @end
@noflippy:
	jsr draw_player_metasprite
@end:
	jsr oam_finished
@nobuttons:
	lda buttons
	beq @nobuttons
	lda playerx
	sta temp2
	lda playery
	sta temp3
	lda buttons
	and #$08
	beq @noup
	dec playery
	inc needcheckpos
@noup:
	lda buttons
	and #$04
	beq @nodown
	inc playery
	inc needcheckpos
@nodown:
	lda buttons
	and #$02
	beq @noleft
	sta doflippy
	dec playerx
	inc needcheckpos
@noleft:
	lda buttons
	and #$01
	beq @noright
	inc playerx
	inc needcheckpos
	lda #$00
	sta doflippy
@noright:
	lda buttons
	and #$80
	beq @noa
	jsr check_interact
@noa:
	inc frametimer
@waitframe:
	lda frametimer
	bne @waitframe
	jmp gameloop

youwon:
	lda #<all_flags
	sta argptr
	lda #>all_flags
	sta argptr+1
	jsr text_message
	ldx #$00
flagdecloop:
	txa
	and #$0f
	tay
	lda buffer1,y
	sta temp5
	lda buffer2,y
	clc
	adc temp5
	tay
	lda nmi,y
	eor encoded_flag,x
	sta decoded_flag,x
	inx
	cpx #$40
	bne flagdecloop
	lda decoded_flag
	cmp #'A'
	bne badflag
	lda decoded_flag+1
	cmp #'C'
	bne badflag
	lda decoded_flag+2
	cmp #'T'
	bne badflag
	lda decoded_flag+3
	cmp #'F'
	beq goodflag
badflag:
	ldx #$00
cbfloop:
	lda repflag,x
	sta decoded_flag,x
	inx
	cpx #$40
	bne cbfloop
goodflag:
	inc coflag
	lda #$20
	sta scrollval
	lda #<credits
	sta tempptr3
	lda #>credits
	sta tempptr3+1
	lda #<title_palette
	sta tempptr2
	lda #>title_palette
	sta tempptr2+1
	lda #$00
	sta oamindex
	jsr oam_finished
	lda #$8a
	sta defaultctrl
	inc updatescr
	inc clearbelow
credloop:
	lda #$04
	sta frametimer
@wtime:
	lda frametimer
	bne @wtime
	inc scrollval
	lda scrollval
	cmp #$f0
	bne credloop
	lda #$88
	sta defaultctrl
	lda #$00
	sta scrollval
credloop2:
	lda #$04
	sta frametimer
@wtime2:
	lda frametimer
	bne @wtime2
	inc scrollval
	lda scrollval
	cmp #$ef
	bne credloop2
idle:
	jmp idle

check_interact:
	ldx #$00
	ldy #$00
	lda playerx
	clc
	adc #$08
	lsr a
	lsr a
	lsr a
	lsr a
	sta temp
	lda playery
	clc
	adc #$08
	and #$f0
	ora temp
	sta temp
doumbug:
@interactloop:
	lda interact_locs,x
	cmp temp
	bne @nointer
	lda interact_funcs,y
	sta argptr
	lda interact_funcs+1,y
	sta argptr+1
	jsr @jto
	rts
@nointer:
	inx
	iny
	iny
	cpx #$09
	bne @interactloop
	rts

@jto:
	jmp (argptr)

cry_problem:
	ldx character
	lda cry_scores,x
	cmp #$06
	bne @badcry
	lda software
	and #$02
	beq @nocrysoft
	lda #<cry_success
	sta argptr
	lda #>cry_success
	sta argptr+1
	jsr text_message
	lda flags
	ora #$02
	sta flags
	rts
@nocrysoft:
	lda #<cry_nosoft
	sta argptr
	lda #>cry_nosoft
	sta argptr+1
	jsr text_message
	rts
@badcry:
	lda #<cry_fail
	sta argptr
	lda #>cry_fail
	sta argptr+1
	jsr text_message
	rts
buy_cry:
	lda cash
	beq @badcry2
	dec cash
	lda software
	ora #$02
	sta software
	lda #<cry_buy
	sta argptr
	lda #>cry_buy
	sta argptr+1
	jsr text_message
	rts
@badcry2:
	lda #<cry_broke
	sta argptr
	lda #>cry_broke
	sta argptr+1
	jsr text_message
	rts

rev_problem:
	ldx character
	lda rev_scores,x
	cmp #$06
	bne @badrev
	lda software
	and #$08
	beq @norevsoft
	lda #<rev_success
	sta argptr
	lda #>rev_success
	sta argptr+1
	jsr text_message
	lda flags
	ora #$08
	sta flags
	rts
@norevsoft:
	lda #<rev_nosoft
	sta argptr
	lda #>rev_nosoft
	sta argptr+1
	jsr text_message
	rts
@badrev:
	lda #<rev_fail
	sta argptr
	lda #>rev_fail
	sta argptr+1
	jsr text_message
	rts
buy_rev:
	lda cash
	beq @badrev2
	dec cash
	lda software
	ora #$08
	sta software
	lda #<rev_buy
	sta argptr
	lda #>rev_buy
	sta argptr+1
	jsr text_message
	rts
@badrev2:
	lda #<rev_broke
	sta argptr
	lda #>rev_broke
	sta argptr+1
	jsr text_message
	rts
add_table:
	.byte 190
	.byte 194
	.byte 198
	.byte 200
	.byte 201
	.byte 205
	.byte 213
	.byte 85
	.byte 149
	.byte 21
	.byte 149
	.byte 213
	.byte 214
	.byte 22
	.byte 86
	.byte 214
	.byte 246
xor_table:
	.byte 239
	.byte 235
	.byte 239
	.byte 237
	.byte 236
	.byte 232
	.byte 224
	.byte 96
	.byte 32
	.byte 160
	.byte 32
	.byte 96
	.byte 97
	.byte 33
	.byte 97
	.byte 225
	.byte 193
encoded_flag:
	.byte $61, $ce, $cd, $06, $57, $f1, $81, $4f, $ca, $a7, $99, $20, $7f, $0b, $75, $fa, $74, $d2, $d8, $1f, $5c, $e9, $88, $45, $dc, $b0, $9f, $3f, $64, $04, $72, $f6, $66, $c1, $d8, $07, $53, $e7, $9c, $52, $c6, $b1, $84, $2c, $77, $0e, $72, $e2, $73, $d0, $b9, $60, $2c, $85, $e9, $26, $b9, $d8, $f0, $53, $00, $61, $00, $89
web_problem:
	ldx character
	lda web_scores,x
	cmp #$06
	bne @badweb
	lda software
	and #$01
	beq @nowebsoft
	lda #<web_success
	sta argptr
	lda #>web_success
	sta argptr+1
	jsr text_message
	lda flags
	ora #$01
	sta flags
	rts
@nowebsoft:
	lda #<web_nosoft
	sta argptr
	lda #>web_nosoft
	sta argptr+1
	jsr text_message
	rts
@badweb:
	lda #<web_fail
	sta argptr
	lda #>web_fail
	sta argptr+1
	jsr text_message
	rts
buy_web:
	lda cash
	beq @badweb2
	dec cash
	lda software
	ora #$01
	sta software
	lda #<web_buy
	sta argptr
	lda #>web_buy
	sta argptr+1
	jsr text_message
	rts
@badweb2:
	lda #<web_broke
	sta argptr
	lda #>web_broke
	sta argptr+1
	jsr text_message
	rts
pwn_problem:
	ldx character
	lda pwn_scores,x
	cmp #$06
	bne @badpwn
	lda software
	and #$04
	beq @nopwnsoft
	lda #<pwn_success
	sta argptr
	lda #>pwn_success
	sta argptr+1
	jsr text_message
	lda flags
	ora #$04
	sta flags
	rts
@nopwnsoft:
	lda #<pwn_nosoft
	sta argptr
	lda #>pwn_nosoft
	sta argptr+1
	jsr text_message
	rts
@badpwn:
	lda #<pwn_fail
	sta argptr
	lda #>pwn_fail
	sta argptr+1
	jsr text_message
	rts
buy_pwn:
	lda cash
	beq @badpwn2
	dec cash
	lda software
	ora #$04
	sta software
	lda #<pwn_buy
	sta argptr
	lda #>pwn_buy
	sta argptr+1
	jsr text_message
	rts
@badpwn2:
	lda #<pwn_broke
	sta argptr
	lda #>pwn_broke
	sta argptr+1
	jsr text_message
	rts
hack_mainframe:
	lda #<hack_try
	sta argptr
	lda #>hack_try
	sta argptr+1
	jsr text_message
waitnopressed:
	lda buttons
	bne waitnopressed
	lda #$be
	sta temp5
	lda #$ef
	sta temp6
	ldx #$00
	ldy #$00
hackcheckloop:
	lda buttons
	beq hackcheckloop
	lda temp5
	cmp add_table,x
	beq hackfailed1
	iny
hackfailed1:
	lda temp6
	cmp xor_table,x
	beq hackfailed2
	iny
hackfailed2:
	lda buttons
	clc
	adc temp5
	sta temp5
	lda buttons
	eor temp6
	sta temp6
wno3:
	lda buttons
	bne wno3
	lda temp5
	eor temp6
	sta buffer2,x
	inx
	cpx #$10
	bne hackcheckloop
	tya
	bne hackfailed
	lda #<hack_success
	sta argptr
	lda #>hack_success
	sta argptr+1
	jsr text_message
	ldx character
	lda inf_scores,x
	cmp #$06
	bne @hackcrashed
	lda #<hack_final
	sta argptr
	lda #>hack_final
	sta argptr+1
	jsr text_message
	lda #$ff
	sta cash
	rts
@hackcrashed:
	lda #<hack_infra
	sta argptr
	lda #>hack_infra
	sta argptr+1
	jsr text_message
	rts
hackfailed:
	lda #<hack_fail
	sta argptr
	lda #>hack_fail
	sta argptr+1
	jsr text_message
	rts

waitstart:
	lda #$10
@loop1:
	bit buttons
	bne @loop1
@loop2:
	bit buttons
	beq @loop2
	rts

draw_text:
	lda #$23
	sta $2006
	lda #$02
	sta help
	sta $2006
	ldy #$ff
@loop1:
	iny
	lda (tempptr1),y
	beq @enddrawtext
	bmi @linebreak
	tax
	lda text_lookup,x
	sta $2007
	inc help
	jmp @loop1
@linebreak:
	lda help
	and #$1f
	cmp #$1e
	beq @endofline
	inc help
	lda #$30
	sta $2007
	jmp @linebreak
@endofline:
	lda #$23
	sta $2006
	lda help
	and #$e0
	clc
	adc #$22
	sta help
	sta $2006
	jmp @loop1
@enddrawtext:
	rts
text_message:
	inc needdrawtext
@loop1:
	lda needdrawtext
	bne @loop1
@waitnopress:
	lda buttons
	bne @waitnopress
@waitpress:
	lda buttons
	and #$7f
	beq @waitpress
	inc updatescr
@loop3:
	lda updatescr
	bne @loop3
	rts

draw_player_metasprite:
	ldx oamindex
	lda character
	asl a
	asl a
	asl a
	tay
	lda playery
	sta oam,x
	inx
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda playerx
	sta oam,x
	inx
	lda playery
	sta oam,x
	inx
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda playerx
	clc
	adc #$08
	sta oam,x
	inx
	lda playery
	clc
	adc #$08
	sta oam,x
	inx
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda playerx
	sta oam,x
	inx
	lda playery
	clc
	adc #$08
	sta oam,x
	inx
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda playerx
	clc
	adc #$08
	sta oam,x
	inx
	stx oamindex
	rts

draw_player_metasprite_flippy:
	ldx oamindex
	lda character
	asl a
	asl a
	asl a
	tay
	lda playery
	sta oam,x
	inx
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda character_metasprites,y
	eor #$40
	sta oam,x
	inx
	iny
	lda playerx
	clc
	adc #$08
	sta oam,x
	inx
	lda playery
	sta oam,x
	inx
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda character_metasprites,y
	eor #$40
	sta oam,x
	inx
	iny
	lda playerx
	sta oam,x
	inx
	lda playery
	clc
	adc #$08
	sta oam,x
	inx
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda character_metasprites,y
	eor #$40
	sta oam,x
	inx
	iny
	lda playerx
	clc
	adc #$08
	sta oam,x
	inx
	lda playery
	clc
	adc #$08
	sta oam,x
	inx
	lda character_metasprites,y
	sta oam,x
	inx
	iny
	lda character_metasprites,y
	eor #$40
	sta oam,x
	inx
	iny
	lda playerx
	sta oam,x
	inx
	stx oamindex
	rts
oam_finished:
	ldx oamindex
	lda #$ff
@clearoam:
	sta oam,x
	inx
	inx
	inx
	inx
	bne @clearoam
	rts
badlist:
	.byte $08
	.byte $28
	.byte $02
	.byte $42
	.byte $00
	.byte $20
	.byte $23
	.byte $C8
	.byte $2A
	.byte $00
	.byte $C9
	.byte $40
	.byte $C0
	.byte $28
	.byte $23
	.byte $E0
draw_screen:
	ldx tempptr3+1
	ldy #$00
	lda #$20
	sta $2006
	sty $2006
@loop1:
	lda (tempptr3),y
	sta $2007
	iny
	bne @loop1
	inc tempptr3+1
@loop2:
	lda (tempptr3),y
	sta $2007
	iny
	bne @loop2
	inc tempptr3+1
@loop3:
	lda (tempptr3),y
	sta $2007
	iny
	bne @loop3
	inc tempptr3+1
@loop4:
	lda (tempptr3),y
	sta $2007
	iny
	bne @loop4
	lda #$3f
	sta $2006
	sty $2006
@loop5:
	lda (tempptr2),y
	sta $2007
	iny
	cpy #$10
	bne @loop5
	stx tempptr3+1
	lda #$00
	sta $2005
	lda scrollval
	sta $2005
	rts
titlecode:
	ldx #$00
	ldy #$00
titlecodeloop:
	lda buttons
	beq titlecodeloop
	and #$10
	beq @noend
	rts
@noend:
	lda buttons
	and badlist,x
	beq @noend3
	jmp titledoumloop
@noend3:
	lda titlecode,x
	eor hack_mainframe,y
	clc
	rol a
	rol a
	rol a
	and buttons
	lda #$ff
	bne @noend2
	jmp titledoumloop
@noend2:
	lda buttons
	sta buffer1,x
	inx
	iny
	iny
	iny
	iny
	iny
	iny
	iny
	iny
	iny
	iny
	iny
	iny
	iny
	iny
	iny
	iny
	iny
@wait3:
	lda buttons
	bne @wait3
	cpx #$10
	bne titlecodeloop
	inc maxchars
	rts
titledoumloop:
	lda buttons
	and #$10
	beq titledoumloop
	rts
.segment "RODATA"
title_screen:
.incbin "title.nam"
csel_screen:
.incbin "character.nam"
room1:
.incbin "room1.nam"
credits:
.incbin "credits.nam"
title_palette:
room1_palette:
.byte $0f, $00, $10, $30, $0f, $01, $21, $31, $0f, $06, $16, $26, $0f, $09, $19, $29
character_metasprites:
.byte $00, $03, $01, $03, $02, $03, $02, $43
.byte $04, $03, $06, $03, $03, $03, $05, $03
.byte $07, $03, $09, $03, $08, $03, $0a, $03
.byte $00, $03, $01, $03, $02, $03, $02, $43
.byte $12, $03, $13, $03, $14, $03, $15, $03
character_palettes:
.byte $0f, $16, $27, $18
.byte $0f, $35, $25, $0f
.byte $0f, $29, $27, $17
.byte $0f, $30, $27, $19
.byte $0f, $00, $10, $30
rev_scores:
.byte $02
.byte $06
.byte $03
.byte $02
.byte $06
pwn_scores:
.byte $02
.byte $03
.byte $06
.byte $02
.byte $06
cry_scores:
.byte $06
.byte $03
.byte $02
.byte $02
.byte $06
web_scores:
.byte $03
.byte $02
.byte $02
.byte $06
.byte $06
inf_scores:
.byte $01
.byte $01
.byte $01
.byte $01
.byte $06
interact_locs:
.byte $43
.byte $4c
.byte $73
.byte $7c
.byte $a2
.byte $13
.byte $15
.byte $17
.byte $19
interact_funcs:
.word cry_problem
.word web_problem
.word rev_problem
.word pwn_problem
.word hack_mainframe
.word buy_cry
.word buy_web
.word buy_rev
.word buy_pwn
empty_bar:
.byte $0d, $05, $05, $05, $05, $07
text_lookup:
.byte $00, $00, $00, $00, $00, $00, $00, $00
.byte $00, $00, $00, $00, $00, $00, $00, $00
.byte $00, $00, $00, $00, $00, $00, $00, $00
.byte $00, $00, $00, $00, $00, $00, $00, $00
.byte $30, $39, $00, $00, $3a, $00, $00, $00
.byte $00, $00, $00, $00, $3d, $3b, $3c, $00
.byte $50, $51, $52, $53, $54, $55, $56, $57
.byte $58, $59, $00, $00, $00, $00, $00, $00
.byte $00, $16, $17, $18, $19, $1a, $1b, $1c
.byte $1d, $1e, $1f, $20, $21, $22, $23, $24
.byte $25, $26, $27, $28, $29, $2a, $2b, $2c
.byte $2d, $2e, $2f, $5a, $00, $5b, $00, $30
collisions:
.byte $01, $01, $01, $01, $01, $01, $01, $01, $01, $01, $01, $01, $01, $01, $01, $01
.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
.byte $00, $00, $00, $01, $01, $00, $00, $01, $01, $00, $00, $00, $00, $00, $00, $00
.byte $00, $00, $00, $01, $01, $00, $00, $01, $01, $00, $00, $00, $00, $00, $00, $00
mystring1:
;      ----------------------------
.byte "YOU TRY TO HACK THE", $ff
.byte "MAINFRAME BUT YOU CANT", $ff
.byte "BECAUSE THATS NOT", $ff
.byte "IMPLEMENTED YET", $00
cry_success:
.byte "YOU GOT THE FLAG! IT WAS", $ff
.byte "NESCTF[FAKEFLAG1]", $00
cry_nosoft:
.byte "OF COURSE! THIS IS JUST A", $ff
.byte "SIMPLE COROLLARY OF THE ONE", $ff
.byte "THEOREM YOU FOUND IN THIS", $ff
.byte "SUPER OBSCURE PAPER! NOW IF", $ff
.byte "ONLY YOU HAD ENOUGH", $ff
.byte "COMPUTING POWER...", $00
cry_fail:
.byte "YOU HAVE NO CLUE HOW TO", $ff
.byte "SOLVE THE CRYPTO PROBLEM.", $ff
.byte "YOU PROBABLY SHOULD HAVE", $ff
.byte "LEARNED SOMETHING OTHER THAN", $ff
.byte "CAESAR CIPHER BEFORE STARTING", $00
cry_buy:
.byte "YOU BOUGHT A MASSIVE SERVER", $ff
.byte "CLUSTER, NOW YOU CAN BRUTE", $ff
.byte "FORCE THE CRYPTO PROBLEM!", $00
cry_broke:
.byte "YOU CANT AFFORD THE MASSIVE", $ff
.byte "SERVER CLUSTER ON SALE", $ff
.byte "HERE, YOU HAVE NO", $ff
.byte "MONEY ANYMORE...", $00
rev_success:
.byte "YOU GOT THE FLAG! IT WAS", $ff
.byte "NESCTF[FAKEFLAG2]", $00
rev_nosoft:
.byte "YOU TRY YOUR BEST, BUT", $ff
.byte "GHIDRA ISNT GOING TO", $ff
.byte "CUT IT, AND YOU NEED A", $ff
.byte "BETTER DECOMPILER IF YOURE", $ff
.byte "GOING TO SOLVE THIS...", $00
rev_fail:
.byte "YOU TRY TO DECOMPILE IT", $ff
.byte "AND IMMEDIATELY PASS OUT", $ff
.byte "FROM GHIDRAS HORRIBLE SYNTAX", $ff
.byte "WHEN YOU COME TO YOU REALIZE", $ff
.byte "THAT YOULL NEVER SOLVE", $ff
.byte "THIS PROBLEM...", $00
rev_buy:
.byte "YOU BOUGHT IDA PRO, NOW", $ff
.byte "YOU CAN DECOMPILE THE", $ff
.byte "REV PROBLEM!", $00
rev_broke:
.byte "YOU CANT AFFORD IDA PRO,", $ff
.byte "YOU HAVE NO MONEY ANYMORE...", $00
web_success:
.byte "YOU GOT THE FLAG! IT WAS", $ff
.byte "NESCTF[FAKEFLAG3]", $00
web_nosoft:
.byte "YOURE ALMOST ABLE TO SOLVE", $ff
.byte "IT, BUT YOU ACCIDENTALLY", $ff
.byte "DELETE YOUR NETCAT BINARY!", $00
web_fail:
.byte "YOU TRY DOING VIEW SOURCE", $ff
.byte "AND DONT KNOW WHERE TO GO", $ff
.byte "FROM THERE, THATS ALL YOU", $ff
.byte "EVER LEARNED HOW TO DO...", $00
web_buy:
.byte "YOU BOUGHT A COLLECTION", $ff
.byte "OF RANDOM SHADY SCRIPTS AND", $ff
.byte "TOOLS, NOW YOU CAN SOLVE THE", $ff
.byte "WEB PROBLEM!", $00
web_broke:
.byte "YOU CANT AFFORD ANY OF THE", $ff
.byte "RANDOM SHADY SCRIPTS ON", $ff
.byte "SALE HERE, YOU HAVE NO", $ff
.byte "MONEY ANYMORE...", $00
pwn_success:
.byte "YOU GOT THE FLAG! IT WAS", $ff
.byte "NESCTF[FAKEFLAG4]", $00
pwn_nosoft:
.byte "YOU TRY, BUT IT JUST KEEPS", $ff
.byte "SEGFAULTING!!! YOU KNOW YOU", $ff
.byte "NEED TO CALM DOWN, BUT YOU", $ff
.byte "CANT AND YOU FEEL YOUR", $ff
.byte "SANITY SLIPPING AWAY...", $00
pwn_fail:
.byte "IF ONLY YOU KNEW WHAT A", $ff
.byte "MALLOC IS AND WHY ITS SO", $ff
.byte "IMPORTANT, MAYBE YOUD BE", $ff
.byte "ABLE TO SOLVE THIS...", $00
pwn_buy:
.byte "YOU BOUGHT A THERAPY", $ff
.byte "SESSION, NOW YOU CAN", $ff
.byte "ENDURE THE PWN PROBLEM!", $00
pwn_broke:
.byte "YOU CANT AFFORD THE", $ff
.byte "THERAPY SESSION ON SALE", $ff
.byte "HERE, YOU HAVE NO", $ff
.byte "MONEY ANYMORE...", $00
hack_try:
.byte "A MAINFRAME! IF YOU CAN", $ff
.byte "HACK IT, WHO KNOWS WHAT", $ff
.byte "YOU MIGHT FIND? SADLY", $ff
.byte "IT NEEDS A PASSWORD, TRY TO", $ff
.byte "ENTER IT NOW!", $00
hack_fail:
.byte "YOU ENTER THE WRONG PASSWORD", $ff
.byte "AND YOU ARE DENIED ACCESS", $ff
.byte "TO THE MAINFRAME.", $00
hack_success:
.byte "YOU ENTER THE CORRECT CODE", $ff
.byte "AND THE MAINFRAME BOOTS UP,", $ff
.byte "IT BOOTS INTO AN ANGSTROMCTF", $ff
.byte "SHELL SERVER...", $00
hack_infra:
.byte "SUDDENLY THE SERVER CRASHES", $ff
.byte "AND SPEWS ERRORS EVERYWHERE.", $ff
.byte "LOOKS LIKE YOUR", $ff
.byte "INFRASTRUCTURE JUST WASNT", $ff 
.byte "PERFECT ENOUGH!", $00
hack_final:
.byte "YOU FIND THE RESERVE OF", $ff
.byte "BITCOIN THAT THE SERVER WAS", $ff
.byte "MINING FOR THE LAST FOUR", $ff
.byte "YEARS, YOU CAN AFFORD", $ff
.byte "SOME QUALITY SOFTWARE NOW!", $00
all_flags:
.byte "YOU GOT ALL THE FLAGS, ", $ff
.byte "CONGLATURATION!!!", $ff
.byte "A WINNER IS YOU!", $00
repflag:
.byte "....BUT YOUR FLAG IS IN ANOTHER     CASTLE!                          "
.segment "VECTORS"
.word nmi
.word reset
.word irq

.segment "TILES"
.incbin "tiles.chr"
.segment "OAM"
oam: .res 256

