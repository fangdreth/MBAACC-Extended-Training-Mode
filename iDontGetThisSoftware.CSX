
<Structures>
  <Structure Name="whatthefuck" AutoFill="0" AutoCreate="1" DefaultHex="0" AutoDestroy="0" DoNotSaveLocal="0" RLECompression="1" AutoCreateStructsize="4096">
    <Elements>
      <Element Offset="0" Vartype="Byte" Bytesize="1" RLECount="4" OffsetHex="00000000" DisplayMethod="signed integer"/>
      <Element Offset="4" Vartype="4 Bytes" Bytesize="4" OffsetHex="00000004" DisplayMethod="unsigned integer"/>
      <Element Offset="8" Vartype="2 Bytes" Bytesize="2" OffsetHex="00000008" Description="effect type? 0x0101 is a dot, 0x010F is a line?" DisplayMethod="hexadecimal"/>
      <Element Offset="10" Vartype="2 Bytes" Bytesize="2" RLECount="2" OffsetHex="0000000A" Description="scale/stretch/skew?" DisplayMethod="hexadecimal"/>
      <Element Offset="14" Vartype="2 Bytes" Bytesize="2" OffsetHex="0000000E" DisplayMethod="hexadecimal"/>
      <Element Offset="16" Vartype="4 Bytes" Bytesize="4" OffsetHex="00000010" Description="X Pos" DisplayMethod="signed integer"/>
      <Element Offset="20" Vartype="4 Bytes" Bytesize="4" OffsetHex="00000014" Description="Y Pos" DisplayMethod="signed integer"/>
      <Element Offset="24" Vartype="4 Bytes" Bytesize="4" RLECount="8" OffsetHex="00000018" DisplayMethod="unsigned integer"/>
      <Element Offset="56" Vartype="4 Bytes" Bytesize="4" OffsetHex="00000038" Description="angle" DisplayMethod="signed integer"/>
      <Element Offset="60" Vartype="4 Bytes" Bytesize="4" RLECount="2" OffsetHex="0000003C" Description="probs a matrix angle" DisplayMethod="signed integer"/>
      <Element Offset="68" Vartype="4 Bytes" Bytesize="4" RLECount="7" OffsetHex="00000044" DisplayMethod="unsigned integer"/>
    </Elements>
  </Structure>
</Structures>