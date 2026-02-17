

section  .text

global main
main:
	push rbp
	mov rbp,rsp
	sub rsp,40
	mov DWORD [rbp - 4],89
	lea r11,DWORD [rbp - 4]
	mov QWORD [rbp - 12],r11
	mov r11,QWORD [rbp - 12]
	mov QWORD [rbp - 20],r11
	mov r11,QWORD [rbp - 20]
	mov QWORD [rbp - 28],r11
	add QWORD [rbp - 28],1
	mov r11,QWORD [rbp - 20]
	mov QWORD [rbp - 36],r11
	add QWORD [rbp - 36],2
	mov rax,QWORD [rbp - 20]
	mov r11,QWORD [rax - 0]
	mov DWORD [rbp - 40],r11d
	mov eax,DWORD [rbp - 40]
	mov rsp,rbp
	pop rbp
	ret
