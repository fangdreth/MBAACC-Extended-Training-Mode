.386
.model flat

PUBLIC _asmDrawRect
.code
_asmDrawRect PROC
	
	push ebp
	mov ebp,esp
	sub esp,38h
	mov eax,00415450h
	mov edx,[ebp+28h]
	mov [esp+20h],edx
	mov edx,[ebp+24h]
	mov [esp+1Ch],edx
	mov edx,[ebp+20h]
	mov [esp+18h],edx
	mov edx,[ebp+1Ch]
	mov [esp+14h],edx
	mov edx,[ebp+18h]
	mov [esp+10h],edx
	mov edx,[ebp+14h]
	mov [esp+0Ch],edx
	mov edx,[ebp+10h]
	mov [esp+08h],edx
	mov edx,[ebp+0Ch]
	mov [esp+04h],edx
	mov edx,[ebp+08h]
	mov [esp],edx
	mov edx,eax
	xor eax,eax
	call edx
	add esp,24h
	mov eax,00000001h
	leave 
	ret 

_asmDrawRect ENDP 

PUBLIC _asmDrawText
.code
_asmDrawText PROC
	
	push ebp
	mov ebp, esp
	sub esp, 56
	mov eax, 41D340h
	mov edx, [ebp + 48]
	mov [esp + 32], edx
	mov edx, [ebp + 44]
	mov [esp + 28], edx
	mov edx, [ebp + 12]
	mov [esp + 24], edx
	mov edx, [ebp + 40]
	mov [esp + 20], edx
	mov edx, [ebp + 36]
	mov [esp + 16], edx
	mov edx, [ebp + 28]
	mov [esp + 12], edx
	mov edx, [ebp + 32]
	mov [esp + 8], edx
	mov edx, [ebp + 24]
	mov [esp + 4], edx
	mov edx, [ebp + 20]
	mov [esp + 0], edx
	mov edx, eax
	mov eax, [ebp + 8]
	mov ecx, [ebp + 16]
	call edx
	add esp, 48
	mov eax, 1
	leave 

	ret

_asmDrawText ENDP

END