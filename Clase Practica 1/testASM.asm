
section .data
hello: db 'Hola SO!', 10
hello_len: equ $-hello

section .text
global _start
_start:
    mov eax, 4 ; syscall write
    mov ebx, 1 ; stdout
    mov ecx, hello ; mensaje
    mov edx, hello_len

    int 0x80
    mov eax, 1 ; syscall exit
    mov ebx, 0 ;
    int 0x80

; nasm -f elf32 testASM.asm -o testASM.o && ld -m elf_i386 testASM.o -o testASM && ./testASM
; Usamos elf32 porque usamos registros 32 en este ejemplo

; Y si fuera arm? cambiaria todo, no es portable hacerlo asi, por eso usamo las wrapper, de c
