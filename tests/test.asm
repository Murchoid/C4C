

section  .text

global main
main:
	push rbp
	mov rbp,rsp
	sub rsp,32
	mov DWORD [rbp - 4],90
	lea r11,DWORD [rbp - 4]
	mov QWORD [rbp - 12],r11
	mov r11,QWORD [rbp - 12]
	mov QWORD [rbp - 20],r11
	mov rax,QWORD [rbp - 20]
	mov QWORD [rax - 0],78
	mov rax,QWORD [rbp - 20]
	mov r11,QWORD [rax - 0]
	mov DWORD [rbp - 24],r11d
	add DWORD [rbp - 24],7
	mov rax,QWORD [rbp - 20]
	mov QWORD [rax - 0],50
	mov rax,QWORD [rbp - 20]
	mov r11d,DWORD [rbp - 24]
	mov DWORD [rbp - 28],r11d
	mov r11,QWORD [rax - 0]
	add DWORD [rbp - 28],r11d
	mov eax,DWORD [rbp - 28]
	mov rsp,rbp
	pop rbp
	ret
