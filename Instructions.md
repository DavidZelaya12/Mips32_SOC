# Proyecto: Simulador de un System-on-Chip MIPS

## Organización de Computadoras — Q2 2026

## 1. Introducción

En este proyecto desarrollara n un **simulador de un System-on-Chip (SoC) MIPS32** utilizando
**C++20**. El simulador replica un sistema embebido completo que incluye un procesador MIPS32,
una pantalla VGA en modo texto, un temporizador y un teclado de 16 teclas — todos
interconectados mediante un esquema de **Memory-Mapped I/O (MMIO)**.

Su responsabilidad principal es implementar el **módulo de la CPU**. Los dema s mo dulos del
sistema (controlador de teclado, temporizador, controlador VGA y ventana del monitor) les sera n
proporcionados como parte del framework del proyecto.

El objetivo pedago gico es que comprendan, a nivel funcional y de ciclo simple, co mo un
procesador ejecuta instrucciones MIPS32, co mo se comunica con dispositivos de E/S y co mo el
hardware y el software interactu an en un sistema real.


## 2. Descripción General del Sistema

El sistema simulado es un **SoC (System-on-Chip)** compuesto por los siguientes mo dulos:

┌─────────────────────────────────────────────────────┐
│ PROCESSOR CHIP │
│ │
│ ┌───────────────────────────────────────────────┐ │
│ │ CPU │ │
│ │ ┌─────────────┐ ┌─────────────┐ │ │
│ │ │ Instruction │ │ Data │ │ │
│ │ │ Memory │ │ Memory │ │ │
│ │ │ (16 KB) │ │ (8 KB) │ │ │
│ │ └─────────────┘ └─────────────┘ │ │
│ │ │ │
│ │ ┌─────────────┐ ┌───────────────────────┐ │ │
│ │ │ Register │ │ Program Counter │ │ │
│ │ │ File │ │ (PC) │ │ │
│ │ │ (32 regs) │ └───────────────────────┘ │ │
│ │ └─────────────┘ │ │
│ └────────────────────────┬──────────────────────┘ │
│ │ System Bus │
│ ┌────────────────┼────────────────┐ │
│ │ │ │ │
│ ┌───────┴───────┐ ┌──────┴─────┐ ┌────────┴─────┐ │
│ │ VGA Text │ │ Timer │ │ Keypad │ │
│ │ Driver │ │ (ms) │ │ Driver │ │
│ │ 80×30 │ │ │ │ 16 keys │ │
│ └───────┬───────┘ └────────────┘ └──────────────┘ │
└──────────┼──────────────────────────────────────────┘
│
┌──────┴──────┐
│ VGA Monitor │
│ Simulator │
│ (Window) │
└─────────────┘


**2.1. Módulos del Sistema**

# Mo dulo Descripcio n ¿Lo implementan ustedes?

1 **CPU (Core)** Procesador MIPS32 de ciclo simple
con registro de instrucciones, banco
de registros, ALU, memoria de
instrucciones y memoria de datos

```
✅ Sí
```
2 **VGA Text
Driver**

```
Controlador de video en modo texto
80×30 con paleta de 16 colores
```
```
❌ Proporcionado
```
3 **Timer** Temporizador que cuenta
milissegundos desde el inicio del
sistema

```
❌ Proporcionado
```
4 **Keypad
Driver**

```
Controlador para un teclado de 16
teclas
```
```
❌ Proporcionado
```
### 5 VGA

```
Monitor
Simulator
```
```
Ventana gra fica que renderiza el
contenido de la pantalla VGA
```
```
❌ Proporcionado
```
## 3. Especificación de la CPU

**3.1. Arquitectura General**

La CPU implementa un modelo de **ciclo simple (single-cycle)** con la siguiente estructura:

- **Program Counter (PC):** Registro de 32 bits que contiene la direccio n de la instruccio n
    actual.
- **Instruction Memory:** Memoria para almacenar el programa (instrucciones).
- **Data Memory:** Memoria para datos + pila (stack).

Los tamanA os de ambas memorias son **configurables**. Por defecto se usan los siguientes valores:

Memoria TamanA o por defecto

Instruction Memory **16 KB** (0x4000 bytes)

Data Memory (datos) **8 KB** (0x2000 bytes)

Stack **8 KB** (0x2000 bytes)

La Instruction Memory, Data Memory y Stack esta n separadas en el espacio de direcciones segu n
la siguiente tabla de mapa de memoria (ver §5.1).

- **Register File:** Banco de **32 registros** de 32 bits ($zero a $ra), con capacidad de leer dos
    registros y escribir uno por ciclo.

**3.2. Endianness**

El sistema utiliza **little-endian**. Esto significa que en las instrucciones de almacenamiento (sb, sh,
sw), el byte menos significativo se almacena en la direccio n ma s baja.


**3.3. Ciclo de Ejecución**

Cada instruccio n se ejecuta en un u nico ciclo de reloj siguiendo las etapas:

1. **Fetch:** Se lee la instruccio n desde Instruction Memory usando el PC.
2. **Decode:** Se decodifica la instruccio n y se leen los registros necesarios del Register File.
3. **Execute:** La ALU realiza la operacio n correspondiente.
4. **Memory:** Si aplica, se accede a Data Memory (lectura o escritura).
5. **Writeback:** Si aplica, se escribe el resultado en el Register File.
6. **PC Update:** Se actualiza el PC (PC+4 o direccio n de salto).

## 4. Set de Instrucciones

Deben implementar las siguientes **43 instrucciones** del conjunto MIPS32:

**4.1. Instrucciones Tipo R (opcode = 0x00)**

Instruccio n Funct Operacio n Descripcio n

nop 0x00 — No operacio n (codificada
como sll $zero, $zero,
0 )

sll 0x00 rd = rt << shamt Shift left logical
(constante)

srl 0x02 rd = rt >> shamt Shift right logical
(constante)

sra 0x03 rd = rt >>> shamt Shift right arithmetic

```
(constante)
```
sllv 0x04 rd = rt << rs[4:0] Shift left logical (variable)

srlv 0x06 rd = rt >> rs[4:0] Shift right logical
(variable)

srav 0x07 rd = rt >>>
rs[4:0]

```
Shift right arithmetic
(variable)
```
jr 0x08 PC = rs Salto a direccio n en
registro

mult 0x18 HI:LO = rs × rt
(con signo)

```
Multiplicacio n con signo
```
multu 0x19 HI:LO = rs × rt (sin

```
signo)
```
```
Multiplicacio n sin signo
```
div 0x1A LO = rs / rt; HI =
rs % rt (con signo)

```
Divisi o n con signo
```
divu 0x1B LO = rs / rt; HI =
rs % rt (sin signo)

```
Divisi o n sin signo
```
add 0x20 rd = rs + rt Suma con deteccio n de
overflow

addu 0x21 rd = rs + rt Suma sin overflow


Instruccio n Funct Operacio n Descripcio n

sub 0x22 rd = rs - rt Resta con deteccio n de

```
overflow
```
subu 0x23 rd = rs - rt Resta sin overflow

and 0x24 rd = rs & rt AND l o gico

or 0x25 rd = rs \| rt OR l o gico

xor 0x26 rd = rs ^ rt XOR l o gico

slt 0x2A rd = (rs < rt)? 1
: 0

```
Set on less than (con
signo)
```
sltu 0x2B rd = (rs < rt)? 1
: 0

```
Set on less than (sin
signo)
```
**4.2. Instrucciones Tipo J (Jump)**

Instruccio n Opcode Operacio n Descripcio n

j 0x02 PC = {PC[31:28],
address, 2'b00}

```
Salto incondicional
```
jal 0x03 $ra = PC+4; PC =
{PC[31:28],
address, 2'b00}

```
Salto y enlace
```
**4.3. Instrucciones Tipo I (Inmediato / Branch / Memoria)**

Instruccio n Opcode Operacio n Descripcio n

beq 0x04 if (rs == rt) PC =
PC + 4 + offset

```
Branch if equal
```
bne 0x05 if (rs != rt) PC =
PC + 4 + offset

```
Branch if not equal
```
blez 0x06 if (rs <= 0) PC =
PC + 4 + offset

```
Branch if ≤ zero
```
bgtz 0x07 if (rs > 0) PC =
PC + 4 + offset

```
Branch if > zero
```
addi 0x08 rt = rs +
sign_ext(imm)

```
Suma con inmediato (con
overflow)
```
addiu 0x09 rt = rs +
sign_ext(imm)

```
Suma con inmediato (sin
overflow)
```
slti 0x0A rt = (rs <
sign_ext(imm))? 1
: 0

```
Set on less than
immediate
```
sltiu 0x0B rt = (rs <
sign_ext(imm))? 1
: 0

```
Set on less than
immediate unsigned
```
andi 0x0C rt = rs &
zero_ext(imm)

```
AND con inmediato
```
ori 0x0D rt = rs |
zero_ext(imm)

```
OR con inmediato
```

Instruccio n Opcode Operacio n Descripcio n

xori 0x0E rt = rs ^
zero_ext(imm)

```
XOR con inmediato
```
lui 0x0F rt = imm << 16 Load upper immediate

lb 0x20 rt =
sign_ext(MEM[rs+im
m][7:0])

```
Load byte (con extensio n
de signo)
```
lh 0x21 rt =
sign_ext(MEM[rs+im
m][15:0])

```
Load halfword (con
extensio n de signo)
```
lw 0x23 rt = MEM[rs+imm] Load word

lbu 0x24 rt =
zero_ext(MEM[rs+im
m][7:0])

```
Load byte unsigned
```
lhu 0x25 rt =
zero_ext(MEM[rs+im
m][15:0])

```
Load halfword unsigned
```
sb 0x28 MEM[rs+imm][7:0] =
rt[7:0]

```
Store byte
```
sh 0x29 MEM[rs+imm][15:0]
= rt[15:0]

```
Store halfword
```
sw 0x2B MEM[rs+imm] = rt Store word

**4.4. Registros Especiales: HI y LO**

Las instrucciones mult, multu, div y divu utilizan los registros especiales **HI** y **LO** :

- **mult rs, rt** : Resultado de 64 bits → HI (32 bits altos) y LO (32 bits bajos). Multiplicacio n
    con signo.
- **multu rs, rt** : Igual que mult, pero multiplicacio n sin signo.
- **div rs, rt** : LO = rs / rt, HI = rs % rt. Divisio n con signo.
- **divu rs, rt** : LO = rs / rt, HI = rs % rt. Divisio n sin signo.
    **Nota:** Para leer los resultados de HI y LO se utilizan las instrucciones mfhi rd (opcode
    0x00, funct 0x10) y mflo rd (opcode 0x00, funct 0x12). Deben implementar ambas.

## 5. Memory-Mapped I/O (MMIO)

El sistema utiliza **MMIO** para la comunicacio n entre la CPU y los dispositivos de E/S. Las
direcciones especiales se encuentran dentro del espacio de memoria de datos.

**5.1. Mapa de Memoria (con valores por defecto)**

```
Nota: Las direcciones de Instruction Memory, Data Memory y Stack dependen de los
tama nA os configurados. La tabla muestra los valores por defecto.
```
Direccio n TamanA o Dispositivo Descripcio n

0x00004000 – 16 KB Instruction Memory Programa en MIPS


Direccio n TamanA o Dispositivo Descripcio n

0x00007FFF

0x10000000 –
0x10001FFF

```
8 KB Data Memory Datos del programa
```
0x7FFFCFFD –
0x7FFFEFFC

```
8 KB Stack Pila (crece hacia abajo,
$sp inicial = 0x7FFFEFFC)
```
0x0000B800 –
0x0000CABF

```
~5 KB VGA Text Buffer Pantalla 80×30 (texto)
```
0xFFFF0004 4 bytes Keypad Estado del teclado (
bytes bajos)

0xFFFF0008 4 bytes Timer Contador de
milissegundos

_Cálculo de direcciones según tamaños configurados_

Dados IM_SIZE, DM_SIZE y STACK_SIZE (en bytes):

Regi o n Direccio n base Direccio n final

Instruction Memory 0x00004000 0x00004000 + IM_SIZE^

- 1

Data Memory 0x10000000 0x10000000 + DM_SIZE^

- 1

Stack 0x7FFFEFFC -
STACK_SIZE + 1

```
0x7FFFEFFC
```
El puntero de pila ($sp) se inicializa a 0x7FFFEFFC (la direccio n ma s alta del rango de Stack). La
pila crece hacia direcciones decrecientes.

**5.2. Detalle del Keypad (Dirección 0xFFFF0004)**

El keypad proporciona **16 teclas** , cada una representada por un bit en los **dos bytes menos
significativos** de la palabra de 32 bits:

Bit: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
K16 K15 K14 K13 K12 K11 K10 K9 K8 K7 K6 K5 K4 K3 K
K

- 0 = tecla suelta
- 1 = tecla presionada

Para leer el estado del keypad:

lui $t0, 0xFFFF # $t0 = 0xFFFF
lw $t1, 0x0004($t0) # $t1 = estado del keypad
andi $t1, $t1, 0xFFFF # Solo los 2 bytes bajos ( 16 teclas)

**5.3. Detalle del Timer (Dirección 0xFFFF0008)**

El timer almacena un valor de 32 bits que representa los **milissegundos transcurridos** desde
que el sistema fue iniciado. Se lee mediante:


lui $t0, 0xFFFF # $t0 = 0xFFFF
lw $t1, 0x0008($t0) # $t1 = milissegundos desde el inicio

**5.4. Detalle de la VGA Text Mode (Direcciones 0xB800 – 0xCABF)**

La pantalla VGA opera en **modo texto** con las siguientes caracterí sticas:

- **Resolución:** 80 columnas × 30 filas
- **Paleta:** 16 colores
- **Cada carácter** ocupa **2 bytes (1 halfword)** :
Byte alto (bits 15-8): Atributo de color
┌──────────────────────────────────────┐
│ Bit 15-12: Color de fondo (4 bits) │
│ Bit 11-8: Color de texto (4 bits) │
└──────────────────────────────────────┘

Byte bajo (bits 7-0): Código ASCII del carácter
┌──────────────────────────────────────┐
│ Bit 7-0: Carácter ASCII │
└──────────────────────────────────────┘

_Colores disponibles (4 bits)_

Co digo Color Co digo Color

0x0 Negro 0x8 Gris oscuro

0x1 Azul 0x9 Azul claro

0x2 Verde 0xA Verde claro

0x3 Cian 0xB Cian claro

0x4 Rojo 0xC Rojo claro

0x5 Magenta 0xD Magenta claro

0x6 Marr o n/Naranja0xE Amarillo

0x7 Gris claro 0xF Blanco

_Dirección de un carácter en la VGA_
dirección = 0xB800 + (fila × 80 + columna) × 2

**Ejemplo:** Escribir la letra 'A' con texto blanco sobre fondo negro en la posicio n (0, 0):

lui $t0, 0x0000 # Parte alta
ori $t0, $t0, 0xB800 # $t0 = 0x0000B800 (base VGA)
li $t1, 0x0F41 # Fondo negro (0x0), Texto blanco (0xF), 'A' = 0x
sh $t1, 0 ($t0) # Escribir en posición ( 0 , 0 )


## 6. Requisitos de Implementación

**6.1. Lenguaje y Herramientas**

Requisito Detalle

**Lenguaje** C++20 (usar std::format, std::span,
constexpr, enum class, structured
bindings, etc.)

**Compilador** GCC 12+ o Clang 15+ con soporte C++

**Build system** CMake 3.22+

**Librería gráfica** Proporcionada (para la ventana del
monitor VGA)

**Plataforma** Linux, macOS o Windows (con WSL
recomendado)

**6.2. Arquitectura del Código**

El framework del proyecto les proporciona las interfaces (clases abstractas) de los mo dulos que
**no** deben implementar. Ustedes deben implementar **exclusivamente la clase CPU**.

Se espera que la implementacio n de la CPU siga una estructura modular que refleje los
componentes reales del procesador. A continuacio n se sugiere una organizacio n (pueden
adaptarla):

src/
├── cpu/
│ ├── cpu.h / cpu.cpp ← Clase principal del CPU
│ ├── alu.h / alu.cpp ← Unidad Aritmético-Lógica
│ ├── register_file.h ← Banco de registros (32 registros × 32 bits +
HI/LO)
│ ├── instruction_decoder.h ← Decodificador de instrucciones
│ ├── control_unit.h ← Señales de control
│ └── memory.h / memory.cpp ← Instruction Memory + Data Memory
├── soc/
│ ├── soc.h / soc.cpp ← Integración del SoC (proporcionado)
│ ├── vga_driver.h ← Controlador VGA (proporcionado)
│ ├── timer.h ← Temporizador (proporcionado)
│ └── keypad.h ← Controlador de teclado (proporcionado)
├── vga/
│ └── monitor_window.h ← Ventana del monitor (proporcionado)
└── main.cpp

## 7. Formato de los Archivos de Programa

Los programas MIPS32 compilara n a formato binario. Se les proporcionara una herramienta
(elf2bin) que extrae el co digo ma quina y datos de un archivo ELF y genera dos archivos binarios
(uno para Instruction Memory y otro para Data Memory).


El CPU debe cargar los archivos en las memorias correspondientes al inicio. Los **nombres de los
archivos de salida** ser a n determinados por el usuario al ejecutar la herramienta, no esta n
predefinidos. Su simulador debe aceptar rutas de archivo como para metros de entrada o a trave s
de un archivo de configuracio n.

## 8. Entregables

**Entrega 1: CPU Básica — Instrucciones Aritméticas y Lógicas**

**Fecha:** _(por definir)_

Implementar el ciclo ba sico de la CPU con las siguientes instrucciones:

- nop, add, addu, sub, subu, and, or, xor, slt, sltu
- addi, addiu, slti, sltiu, andi, ori, xori, lui
- sll, srl, sra, sllv, srlv, srav
- j, jal, jr

**Prueba:** Se proporcionara un programa de prueba que valide todas las instrucciones con valores
conocidos.

**Entrega 2: Acceso a Memoria y E/S**

**Fecha:** _(por definir)_

Extender la CPU con:

- Instrucciones de memoria: lw, lb, lbu, lh, lhu, sw, sb, sh
- Soporte para MMIO: teclado, timer y VGA
- Soporte para beq, bne, blez, bgtz

**Prueba:** Programa que lea el keypad y muestre la tecla presionada en la pantalla VGA.

**Entrega 3: Multiplicación, División y Ensamblador Completo**

**Fecha:** _(por definir)_

Extender la CPU con:

- Instrucciones de multiplicacio n y divisio n: mult, multu, div, divu
- Instrucciones auxiliares: mfhi, mflo
- Validacio n completa de las 43+ instrucciones

**Prueba:** Suite de pruebas automatizada que verifique todas las instrucciones.


**Entrega 4: Prueba de Fuego — Correr un Juego Precompilado**

**Fecha:** _(por definir)_

En esta entrega **no implementarán un juego**. Esta es la **prueba definitiva del proyecto** : su CPU
debe ser capaz de ejecutar correctamente un juego precompilado (proporcionado por el
instructor) que corre sobre el SoC completo.

El juego:

- Renderiza gra ficos en la pantalla VGA en modo texto.
- Responde a la entrada del teclado (16 teclas).
- Usa el timer para animaciones, control de velocidad y retardos.
- Ha sido compilado a co digo MIPS32 desde C y/o ensamblador.

Su simulador debera cargar el programa del juego y ejecutarlo sin modificaciones. El juego se
considerara exitoso si:

1. La pantalla VGA muestra correctamente el juego.
2. Las teclas responden adecuadamente.
3. El timer funciona correctamente (animaciones fluidas, sin aceleracio n/desaceleracio n
    anormal).
4. No hay errores de ejecucio n (instrucciones no implementadas, crashes, etc.).

## 9. Consideraciones Técnicas

**9.1. Manejo de Overflows**

Las instrucciones add, addu, sub, subu y addi deben **detectar overflow aritmético** en
complemento a dos. Las versiones con sufijo u simplemente ignoran el overflow.

**9.2. Extensiones de Inmediato**

- **Extensión de signo (sign_ext):** Usada por addi, addiu, slti, sltiu, lb, lh, lw, lbu, lhu,
    sb, sh, sw, beq, bne, blez, bgtz.
- **Extensión con cero (zero_ext):** Usada por andi, ori, xori.

**9.3. Alineación de Memoria**

- Las instrucciones esta n alineadas a **4 bytes** (word-aligned).
- lw / sw requieren direcciones alineadas a **4 bytes**.
- lh / lhu / sh requieren direcciones alineadas a **2 bytes**.
- lb / lbu / sb no tienen restriccio n de alineacio n.
    **Opcional (bonus):** Implementar soporte para accesos de memoria no alineados
    (misaligned access) emulando el comportamiento real del MIPS32.

**9.4. El Registro $zero**

El registro $0 ($zero) siempre debe contener el valor 0. Cualquier escritura a $0 debe ser
ignorada.


**9.5. Instrucción nop**

La instruccio n nop se codifica como sll $zero, $zero, 0 (todos los bits en 0: 0x00000000). Su
ejecucio n simplemente avanza el PC a PC+4 sin efectos secundarios.

**9.6. Convención de Llamadas (Calling Convention)**

Para la integracio n con co digo C compilado, deben respetar la convencio n de llamadas MIPS32:

Registro Nombre Uso

$0 $zero Constante 0

$1 $at Reservado para el

```
ensamblador
```
$2-$3 $v0-$v1 Valores de retorno

$4-$7 $a0-$a3 Argumentos de
funciones

$8-$15 $t0-$t7 Temporales (no

```
preservados)
```
$16-$23 $s0-$s7 Salvados (preservados)

$24-$25 $t8-$t9 Temporales
adicionales

$26-$27 $k0-$k1 Reservados para el

```
kernel
```
$28 $gp Global pointer

$29 $sp Stack pointer

$30 $fp Frame pointer

$31 $ra Return address

## 10. Criterios de Evaluación

Criterio Peso

**Funcionalidad de la CPU** — Todas las 43 instrucciones
implementadas y verificadas

### 35%

**Calidad del código** — DisenA o modular, C++ moderno,
naming conventions, documentacio n

### 20%

**Manejo de E/S (MMIO)** — Integracio n correcta con
teclado, timer y VGA

### 15%

**Juego precompilado** — Ejecucio n correcta del juego
proporcionado

### 20%

**Pruebas** — Suite de pruebas unitarias para la CPU y sus
componentes

### 10%


## 11. Recursos

- **Referencia principal:** _Computer Organization and Design: The Hardware/Software_
    _Interface_ — Patterson & Hennessy (MIPS Edition)
- **Documentación MIPS32:** _MIPS32 Architecture for Programmers Vol. II: The MIPS_
    _Instruction Set_
- **Framework del proyecto:** _(se proporcionará enlace al repositorio)_
- **Herramienta elf2bin:** _(incluida en el framework)_
- **Compilador C para MIPS32:** GCC (target mips-linux-gnu-gcc o mipsel-linux-gnu-gcc)
    o Clang (target mipsel-linux-gnu). Pueden instalar un cross-compiler mediante el gestor
    de paquetes de su distribucio n o descargar prebuilt toolchains como _MIPS GNU/Linux_
    _Toolchain_.

_Documento generado para el curso CCC403 — Organización de Computadoras, Q2 2026._


