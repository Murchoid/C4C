
section .data
	a dd 67


section  .text

global main
main:
	push rbp
	mov rbp,rsp
	sub rsp,16
	mov r11d,[rel a]
	mov DWORD [rbp - 4],r11d
	mov r11d,DWORD [rbp - 4]
	mov DWORD [rbp - 8],r11d
	cmp DWORD [rbp - 8],200
	mov DWORD [rbp - 8],0
	setl BYTE [rbp - 8]
	cmp DWORD [rbp - 8],0
	je __c4_label.2
	mov DWORD [rbp - 4],90
	mov r11d,[rel a]
	mov DWORD [rbp - 12],r11d
	sub DWORD [rbp - 12],50
	mov r11d,DWORD [rbp - 12]
	mov [rel a],r11d
	jmp __c4_label.1
__c4_label.2:
__c4_label.1:
	mov eax,[rel a]
	mov rsp,rbp
	pop rbp
	ret
