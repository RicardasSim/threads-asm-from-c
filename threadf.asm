BITS 64

DEFAULT REL

global temp_var

section .tdata write tls
temp_var db 0

section .text

;without temp variable
global TestFnAsm1

TestFnAsm1:

xor rcx,rcx

.label_1:

mov al,[rdi+rcx]
inc al
mov [rdi+rcx],al

inc rcx
cmp rcx,rsi
jl .label_1

xor rax,rax
ret

;with TLS (thread-local storage)
global TestFnAsm2

TestFnAsm2:

push rbx

xor rcx,rcx

.label_1:

mov r8,[rel temp_var wrt ..gottpoff]
mov al,[rdi+rcx]
mov [fs:r8],al
mov bl,[fs:r8]
inc bl
mov [fs:r8],bl
mov al,[fs:r8]
mov [rdi+rcx],al

inc rcx
cmp rcx,rsi
jl .label_1

pop rbx

xor rax,rax
ret

;with temporary variable from C
global TestFnAsm3

TestFnAsm3:

push rbx

xor rcx,rcx

.label_1:

mov al,[rdi+rcx]
mov [rdx],al
mov bl,[rdx]
inc bl
mov [rdx],bl
mov al,[rdx]
mov [rdi+rcx],al

inc rcx
cmp rcx,rsi
jl .label_1

pop rbx

xor rax,rax
ret

;with stack
global TestFnAsm4

TestFnAsm4:

push rbx

xor rcx,rcx

.label_1:

mov al,[rdi+rcx]
push rax
pop rbx
inc bl
push rbx
pop rax
mov [rdi+rcx],al

inc rcx
cmp rcx,rsi
jl .label_1

pop rbx

xor rax,rax
ret
