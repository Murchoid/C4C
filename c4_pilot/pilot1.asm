global main
main:
	push rbp
	mov rbp,rsp
	sub rsp,32
	mov DWORD [rbp - 4],30
	mov r11d,DWORD [rbp - 4]
	mov DWORD [rbp - 8],r11d
	cmp DWORD [rbp - 8],18
	mov DWORD [rbp - 8],0
	setl BYTE [rbp - 8]
	cmp DWORD [rbp - 8],0
	je __c4_label.4
	mov eax,1
	mov rsp,rbp
	pop rbp
	ret
	jmp __c4_label.3
__c4_label.4:
	mov r11d,DWORD [rbp - 4]
	mov DWORD [rbp - 12],r11d
	cmp DWORD [rbp - 12],23
	mov DWORD [rbp - 12],0
	setl BYTE [rbp - 12]
	cmp DWORD [rbp - 12],0
	je __c4_label.7
	mov eax,2
	mov rsp,rbp
	pop rbp
	ret
	jmp __c4_label.3
__c4_label.7:
	mov r11d,DWORD [rbp - 4]
	mov DWORD [rbp - 16],r11d
	cmp DWORD [rbp - 16],25
	mov DWORD [rbp - 16],0
	setle BYTE [rbp - 16]
	cmp DWORD [rbp - 16],0
	je __c4_label.9
	mov eax,3
	mov rsp,rbp
	pop rbp
	ret
	jmp __c4_label.3
__c4_label.9:
	mov DWORD [rbp - 20],0
continue__c4_label.2:
	mov r11d,1
	cmp r11d,0
	je break__c4_label.2
	mov r11d,DWORD [rbp - 20]
	mov DWORD [rbp - 24],r11d
	cmp DWORD [rbp - 24],100
	mov DWORD [rbp - 24],0
	setl BYTE [rbp - 24]
	cmp DWORD [rbp - 24],0
	je __c4_label.11
	mov r11d,DWORD [rbp - 20]
	mov DWORD [rbp - 28],r11d
	add DWORD [rbp - 28],1
	mov r11d,DWORD [rbp - 28]
	mov DWORD [rbp - 20],r11d
	jmp continue__c4_label.2
	jmp __c4_label.10
__c4_label.11:
__c4_label.10:
	mov eax,DWORD [rbp - 20]
	mov rsp,rbp
	pop rbp
	ret
	jmp continue__c4_label.2
break__c4_label.2:
__c4_label.3:
