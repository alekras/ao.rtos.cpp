IRQ_Handler:

  SUB     lr, lr, #4
  SRSFD    sp!, #0x1f      @ use SRS to save LR_irq and SPSR_irq in one step onto the SVC mode stack
  CPS        #0x1f           @ Use CPS to switch to SVC mode
  PUSH       {r0-r3, r12}    @S tore remaining AAPCS registers on the System mode stack
  AND        r1, sp, #4      @ Ensure stack is 8-byte aligned.
  SUB        sp, sp, r1
  PUSH       {r1, lr}        @ Store adjustment and LR_svc to stack
  BL                         @ identify_and_clear_source
  CPSIE       i              @ Enable IRQ with CPS
  BL          C_irq_handler
  CPSID       i              @ Disable IRQ with CPS
  POP         {r1, lr}       @ Restore LR_svc
  ADD         sp, sp, r1     @ Unadjust stack
  POP         {r0-r3, r12}   @ Restore AAPCS registers
  RFEFD       sp!            @ Return using RFE from the SVC mode stack.
