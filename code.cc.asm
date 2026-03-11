Procedimento para geracao do executavel apos compilacao (em Linux):
(1) compilacao do Assembly com nasm: $ nasm -f elf64 <nome_do_arquivo>
(2) likedicao: $ ld -m elf_x86_64 <nome_arquivo_objeto>

extern printf
extern scanf

	section .text
	global main,_start
main:
_start:

;le valor string
mov edx,16
mov ecx,testando
mov ebx,1
mov eax,3
int 0x80

;escreve valor string
mov edx,16
mov ecx,str0
mov ebx,1
mov eax,4
int 0x80

;le valor inteiro
mov edx,4
mov ecx,valor
mov ebx,1
mov eax,3
int 0x80
label0:
;Amarzenamento de numero
mov rax,4
push rax
;Aplica operador booleano/exp.logica
pop rbx
pop rax
mov rcx,1
cmp eax,ebxjg >0
mov rcx,0
>0:
mov rax, rcx
push rax
;jump condicional
pop rax
cmp rax, 0
jz label1

;escreve valor string
mov edx,16
mov ecx,str1
mov ebx,1
mov eax,4
int 0x80
label2:
;Amarzenamento de numero
mov rax,7
push rax
;Aplica operador booleano/exp.logica
pop rbx
pop rax
mov rcx,1
cmp eax,ebxjg >1
mov rcx,0
>1:
mov rax, rcx
push rax
;jump condicional
pop rax
cmp rax, 0
jz label3
;Amarzenamento de numero
mov rax,4
push rax
mov rax, 4
pop rax
mov [op], rax
;jump incondicional
jmp label2
label3:
;jump incondicional
jmp label0
label1:

;le valor inteiro
mov edx,4
mov ecx,valor
mov ebx,1
mov eax,3
int 0x80
;Aplica operador booleano/exp.logica
pop rbx
pop rax
mov rcx,1
cmp eax,ebxjle 2
mov rcx,0
2:
mov rax, rcx
push rax
;jump condicional
pop rax
cmp rax, 0
jz label4

;escreve valor string
mov edx,16
mov ecx,str2
mov ebx,1
mov eax,4
int 0x80
;jump incondicional
jmp label5
label4:
label5:
jal func0
jal func1

;encerra programa
mov ebx,0
mov eax,1
int 0x80
func0:

;escreve valor string
mov edx,16
mov ecx,str3
mov ebx,1
mov eax,4
int 0x80
jal func1
ra
func1:
jal func0
ra

	section .data
valor: dd "%d", 4
b: dd "%d", 4
op: dd "%d", 4
testei: dd "%lf", 16
valor: dd "%d", 4
testando: db "                " 
g: dd "%lf", 16
a: dd "%d", 4
b: dd "%lf", 16
oi: dd "%lf", 16
oi: dd "%c", 1
g: db "                " 
str0: db "Informe um valor:"
str1: db "\nvalor:"
str2: db "Impressao interna ao IF!\n"
str3: db "Informe um valor:"
