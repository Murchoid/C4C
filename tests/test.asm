

section  .text

add_i64:
	push rbp
	mov rbp,rsp
	sub rsp,24
	mov QWORD [rbp - 8],rdi
	mov QWORD [rbp - 16],rsi
	mov r11,QWORD [rbp - 8]
	mov QWORD [rbp - 24],r11
	mov r11,QWORD [rbp - 16]
	add QWORD [rbp - 24],r11
	mov rax,QWORD [rbp - 24]
	mov rsp,rbp
	pop rbp
	ret
global main
main:
	push rbp
	mov rbp,rsp
	sub rsp,88
	mov QWORD [rbp - 8],65
continue__c4_label.6:
	cmp QWORD [rbp - 8],120
	mov QWORD [rbp - 16],0
	setle BYTE [rbp - 16]
	cmp QWORD [rbp - 16],0
	je break__c4_label.6
	mov r11,QWORD [rbp - 8]
	mov QWORD [rbp - 24],r11
	add QWORD [rbp - 24],1
	mov r11,QWORD [rbp - 24]
	mov QWORD [rbp - 8],r11
	jmp continue__c4_label.6
break__c4_label.6:
	mov r11,QWORD [rbp - 8]
	mov QWORD [rbp - 32],r11
	add QWORD [rbp - 32],17
	mov r11,QWORD [rbp - 32]
	mov QWORD [rbp - 40],r11
	mov r11,QWORD [rbp - 40]
	mov QWORD [rbp - 48],r11
	add QWORD [rbp - 48],89
	mov r11,QWORD [rbp - 48]
	mov QWORD [rbp - 40],r11
	mov r11,QWORD [rbp - 8]
	mov DWORD [rbp - 52],r11d
	mov r11d,DWORD [rbp - 52]
	mov DWORD [rbp - 56],r11d
	mov edi,200
	mov esi,106
	call add_i64
	mov QWORD [rbp - 64],rax
	mov r11,QWORD [rbp - 64]
	mov QWORD [rbp - 72],r11
	mov r11,QWORD [rbp - 72]
	mov DWORD [rbp - 76],r11d
	mov r11,QWORD [rbp - 40]
	mov DWORD [rbp - 80],r11d
	mov r11d,DWORD [rbp - 80]
	mov DWORD [rbp - 84],r11d
	mov r11d,DWORD [rbp - 56]
	sub DWORD [rbp - 84],r11d
	mov r11d,DWORD [rbp - 76]
	mov DWORD [rbp - 88],r11d
	mov r11d,DWORD [rbp - 84]
	sub DWORD [rbp - 88],r11d
	mov eax,DWORD [rbp - 88]
	mov rsp,rbp
	pop rbp
	ret
