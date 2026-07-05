# MIPS32 SoC Simulator

Simulador de un **System-on-Chip (SoC) MIPS32** escrito en **C++20**. Emula un sistema embebido completo: un procesador MIPS32 de ciclo simple, una pantalla VGA en modo texto (80×30), un temporizador de milisegundos y un teclado de 16 teclas, todos interconectados mediante **Memory-Mapped I/O (MMIO)**.

> Proyecto desarrollado para el curso **CCC403 — Organización de Computadoras** (Q2 2026).

---

## Características

- **CPU MIPS32 de ciclo simple** con las etapas Fetch → Decode → Execute → Memory → Writeback → PC Update.
- **43 instrucciones** del set MIPS32 implementadas (formatos R, I y J), incluyendo `mult`/`div` con registros especiales `HI`/`LO`.
- **Banco de 32 registros** de 32 bits + `HI`, `LO` y `PC`, respetando la convención de llamadas MIPS32 (`$zero` siempre en 0).
- **Memoria little-endian** con regiones separadas para instrucciones, datos y pila (tamaños configurables).
- **VGA en modo texto** 80×30 con paleta de 16 colores, renderizada en una ventana SDL2.
- **Keypad de 16 teclas** y **Timer** de milisegundos accesibles vía MMIO.
- **Ejecución multihilo**: la CPU corre en un hilo productor de frames mientras el hilo principal maneja los eventos SDL y el renderizado.
- Carga de programas en **formato binario** (código máquina MIPS32) desde línea de comandos.

## Arquitectura del sistema

```
┌─────────────────────────────────────────────────────┐
│                   PROCESSOR CHIP                    │
│  ┌───────────────────────────────────────────────┐  │
│  │                     CPU                       │  │
│  │  ┌─────────────┐        ┌─────────────┐       │  │
│  │  │ Instruction │        │    Data     │       │  │
│  │  │   Memory    │        │   Memory    │       │  │
│  │  │   (16 KB)   │        │   (8 KB)    │       │  │
│  │  └─────────────┘        └─────────────┘       │  │
│  │  ┌─────────────┐   ┌───────────────────────┐  │  │
│  │  │  Register   │   │    Program Counter    │  │  │
│  │  │ File (32)   │   │         (PC)          │  │  │
│  │  └─────────────┘   └───────────────────────┘  │  │
│  └────────────────────────┬──────────────────────┘  │
│                           │  System Bus             │
│          ┌────────────────┼────────────────┐        │
│  ┌───────┴───────┐  ┌─────┴──────┐  ┌──────┴─────┐  │
│  │   VGA Text    │  │   Timer    │  │   Keypad   │  │
│  │ Driver 80×30  │  │    (ms)    │  │  16 keys   │  │
│  └───────┬───────┘  └────────────┘  └────────────┘  │
└──────────┼──────────────────────────────────────────┘
     ┌─────┴───────┐
     │ VGA Monitor │
     │  (ventana)  │
     └─────────────┘
```

## Mapa de memoria

| Dirección                 | Tamaño  | Dispositivo        | Descripción                                    |
| ------------------------- | ------- | ------------------ | ---------------------------------------------- |
| `0x00004000 – 0x00007FFF` | 16 KB   | Instruction Memory | Programa MIPS32                                |
| `0x0000B800 – 0x0000CABF` | ~5 KB   | VGA Text Buffer    | Pantalla 80×30 (2 bytes por carácter)          |
| `0x10000000 – 0x10001FFF` | 8 KB    | Data Memory        | Datos del programa                             |
| `0x7FFFCFFD – 0x7FFFEFFC` | 8 KB    | Stack              | Pila (crece hacia abajo, `$sp` = `0x7FFFEFFC`) |
| `0xFFFF0004`              | 4 bytes | Keypad             | Estado de las 16 teclas (bits 0–15)            |
| `0xFFFF0008`              | 4 bytes | Timer              | Milisegundos desde el inicio del sistema       |

Cada carácter de la VGA ocupa un *halfword*: el byte bajo es el código ASCII y el byte alto codifica color de texto (bits 8–11) y color de fondo (bits 12–15).

## Set de instrucciones

| Formato | Instrucciones |
| ------- | ------------- |
| **R**   | `nop`, `sll`, `srl`, `sra`, `sllv`, `srlv`, `srav`, `jr`, `mfhi`, `mflo`, `mult`, `multu`, `div`, `divu`, `add`, `addu`, `sub`, `subu`, `and`, `or`, `xor`, `slt`, `sltu` |
| **I**   | `beq`, `bne`, `blez`, `bgtz`, `addi`, `addiu`, `slti`, `sltiu`, `andi`, `ori`, `xori`, `lui`, `lb`, `lh`, `lw`, `lbu`, `lhu`, `sb`, `sh`, `sw` |
| **J**   | `j`, `jal` |

## Requisitos

| Herramienta  | Versión mínima                  |
| ------------ | ------------------------------- |
| Compilador   | GCC 12+ / Clang 15+ (C++20)     |
| CMake        | 3.22+                           |
| SDL2         | Librería de desarrollo instalada |
| Threads      | pthreads o equivalente del SO   |

## Compilación

```bash
git clone https://github.com/DavidZelaya12/Mips32_SOC.git
cd Mips32_SOC

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

El ejecutable resultante es `vga_test` (dentro del directorio `build/`).

## Uso

```bash
./build/vga_test --program <ruta_al_binario> --font <ruta_a_la_fuente> [--data <archivo_de_datos>]
```

| Opción       | Requerida | Descripción                                        |
| ------------ | --------- | -------------------------------------------------- |
| `--program`  | Sí        | Archivo binario con el código máquina MIPS32       |
| `--font`     | Sí        | ROM de la fuente VGA (ej. `font_rom_7b.mif`)       |
| `--data`     | No        | Archivo binario con la sección de datos            |

**Ejemplo** (usando los programas de prueba incluidos):

```bash
./build/vga_test --program Bin_samples/keypad_ui.bin --font font_rom_7b.mif
```

### Programas de ejemplo incluidos (`Bin_samples/`)

| Archivo         | Descripción                                          |
| --------------- | ---------------------------------------------------- |
| `vga_test.bin`  | Prueba de escritura en la pantalla VGA               |
| `keypad_ui.bin` | Lee el keypad y muestra la tecla presionada en VGA   |
| `box_move.bin`  | Caja que se mueve por pantalla usando teclado y timer |
| `inst.bin`      | Programa de validación del set de instrucciones      |

## Controles

La ventana SDL mapea el teclado físico a las 16 teclas del keypad simulado:

| Tecla física | Keypad | Tecla física | Keypad |
| ------------ | ------ | ------------ | ------ |
| `←`          | K1     | `Page Up`    | K9     |
| `→`          | K2     | `Page Down`  | K10    |
| `↓`          | K3     | `Home`       | K11    |
| `↑`          | K4     | `End`        | K12    |
| `Q`          | K5     | `Insert`     | K13    |
| `P`          | K6     | `Delete`     | K14    |
| `B`          | K7     | `F1`         | K15    |
| `Espacio`    | K8     | `F2`         | K16    |

Teclas de control adicionales: `R` (Reset) y `S` (Stop). Cerrar la ventana finaliza la simulación e imprime el estado final de los registros.

## Estructura del proyecto

```
Mips32_SOC/
├── src/
│   ├── main.cpp                  # Punto de entrada: carga, hilos y bucle SDL
│   ├── File_loader.cpp           # Carga de binarios a memoria
│   ├── Timer.h                   # Temporizador (MMIO 0xFFFF0008)
│   ├── CPU/
│   │   ├── CPU.h / CPU.cpp               # Núcleo del procesador (ciclo simple)
│   │   ├── InstructionHandler.h / .cpp   # Decodificador de instrucciones (R/I/J)
│   │   ├── RegisterFile.h / .cpp         # Banco de registros + HI/LO/PC
│   │   └── Memory.h / Memory.cpp         # Memorias + bus MMIO
│   ├── CLI_args/
│   │   └── CliArgs.h / .cpp      # Parseo de argumentos de línea de comandos
│   └── vga/
│       ├── VGATextWindow.h / .cpp    # Ventana SDL2 y manejo de eventos
│       ├── VGAFramebuffer.h / .cpp   # Framebuffer de doble búfer
│       ├── VGAFont.h / .cpp          # Carga de la ROM de fuente
│       ├── VGAColor.h / VGAConstants.h
│       └── Keypad.h                  # Registro de estado del keypad
├── Bin_samples/                  # Programas de ejemplo precompilados (.bin)
├── CMakeLists.txt
└── font_rom_7b.mif               # ROM de fuente VGA (7×9 px por glifo)
```

## Referencias

- Patterson & Hennessy — *Computer Organization and Design: The Hardware/Software Interface* (MIPS Edition).
- *MIPS32 Architecture for Programmers Vol. II: The MIPS Instruction Set*.
