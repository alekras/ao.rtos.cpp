.extern main

;@-------------------------------------------------------------------------

.globl _start
_start:
  ;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
  movw r0,#0x1D2 ;@ IRQ mode 0b1 1101 0010
  msr cpsr_c,r0
//    movw sp, #0
//    movt sp, #0x3FFF
  mov sp,#0x3F400000

  ;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
  movw r0,#0x1D1  ;@ FIQ mode 0b111010001
  msr cpsr_c,r0
  mov sp,#0x3F800000

  ;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
  movw r0,#0x1D3 ;@ Suvervisor mode 0b111010011
  msr cpsr_c,r0
  mov sp,#0x3F000000

  ;@ SVC MODE, IRQ ENABLED, FIQ DIS
  ;@movw r0,#0x153 ;@ Suvervisor mode 0b101010011
  ;@msr cpsr_c, r0

  bl main
hang:
  b hang

.globl GOTO
GOTO:
;@  push {r1-r12, lr}
  blx r0
;@  pop {r1-r12, pc}
 b hang

.globl exception_vectors

exception_vectors:

  ldr pc, reset_irq_entry      ;@  Processor Reset handler         -- we will have to force this on the raspi!
  ldr pc, undefined_irq_entry  ;@  Undefined instruction handler   -- processors that don't have thumb can emulate thumb!
  ldr pc, swi_irq_entry        ;@  Software interrupt / TRAP (SVC) -- system SVC handler for switching to kernel mode.
  ldr pc, prefetch_irq_entry   ;@  Prefetch/abort handler.
  ldr pc, data_irq_entry       ;@  Data abort handler/
  ldr pc, unused_irq_entry     ;@  -- Historical from 26-bit addressing ARMs -- was invalid address handler.
  ldr pc, irq_entry            ;@  IRQ handler
  ldr pc, fiq_entry            ;@  Fast interrupt handler.

.globl exception_handlers_table
exception_handlers_table:

reset_irq_entry:      .word reset_handler
undefined_irq_entry:  .word undefined_instruction_handler
swi_irq_entry:        .word swi_handler
prefetch_irq_entry:   .word prefetch_abort_handler
data_irq_entry:       .word data_abort_handler
unused_irq_entry:     .word unused_handler
irq_entry:            .word irq_handler
fiq_entry:            .word fiq_handler

.globl enable_irq
enable_irq:
  cpsie i
  bx lr

.globl disable_irq
disable_irq:
  cpsid i
  bx lr

.globl get_sp
get_sp:
  mov r0, sp
  bx lr

.globl set_sp
set_sp:
  mov sp, r0
  bx lr

.globl get_cpsr
get_cpsr:
  mrs r0, cpsr
  bx lr

.globl get_spsr
get_spsr:
  mrs r0, spsr
  bx lr

irq_handler:
  sub lr, lr, #4
  srsdb sp!, #0x13
  cps #0x13
  push {r0 - r12, lr}

  mov r0, sp
  bl isr

  mov sp, r0
  pop  {r0 - r12, lr}
  rfeia sp!

swi_handler:
  srsdb sp!, #0x13
  push {r0 - r12, lr}

  mov r0, #16
  mov r1, sp
  bl processInterrupt

  mov sp, r0
  pop  {r0 - r12, lr}
  rfeia sp!

undefined_instruction_handler:
  sub lr, lr, #4
  srsdb sp!, #0x13
  cps #0x13
  push {r0 - r12, lr}
  mov r0, #0
  bl unexpected_exeption
  pop  {r0 - r12, lr}
  rfeia sp!

prefetch_abort_handler:
  sub lr, lr, #4
  srsdb sp!, #0x13
  cps #0x13
  push {r0 - r12, lr}
  mov r0, #1
  bl unexpected_exeption
  pop  {r0 - r12, lr}
  rfeia sp!

data_abort_handler:
  sub lr, lr, #4
  srsdb sp!, #0x13
  cps #0x13
  push {r0 - r12, lr}
  mov r0, #2
  bl unexpected_exeption
  pop  {r0 - r12, lr}
  rfeia sp!

reset_handler:
unused_handler:
fiq_handler:
  b fiq_handler
