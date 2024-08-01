.386
.model flat

PUBLIC _DrawRect
.code
_DrawRect PROC
	
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

_DrawRect ENDP 

END