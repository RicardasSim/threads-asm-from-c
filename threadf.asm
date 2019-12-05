BITS 64

DEFAULT REL

section .data
temp_var db 0

section .text
global TestFnAsm

TestFnAsm:

push rbx

xor rcx,rcx

label_1:

;works well
mov al,[rdi+rcx]
inc al
mov [rdi+rcx],al

;does not work
;mov al,[rdi+rcx]
;mov [temp_var],al
;mov bl,[temp_var]
;inc bl
;mov [temp_var],bl
;mov al,[temp_var]
;mov [rdi+rcx],al

;with temporary variable from C works well
;mov al,[rdi+rcx]
;mov [rdx],al
;mov bl,[rdx]
;inc bl
;mov [rdx],bl
;mov al,[rdx]
;mov [rdi+rcx],al

;with stack works well
;mov al,[rdi+rcx]
;push rax
;pop rbx
;inc bl
;push rbx
;pop rax
;mov [rdi+rcx],al

inc rcx
cmp rcx,rsi
jl label_1

pop rbx

xor rax,rax
ret
